/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <functional>
#include <fstream>
#include "internals/download_session.h"
#include "internals/download_url.h"
#include "insert_order_map.h"
#include "str.h"

namespace wl {

// Automates internet download operations.
class download final {
public:
	using session = _wli::download_session;
	using url_crack = _wli::download_url;

private:
	const session& _session;
	HINTERNET      _hConnect = nullptr, _hRequest = nullptr;
	DWORD          _statusCode = 0;
	size_t         _contentLength = 0, _totalGot = 0;
	std::wstring   _url, _verb, _referrer;
	insert_order_map<std::wstring, std::wstring> _requestHeaders;
	insert_order_map<std::wstring, std::wstring> _responseHeaders;
	std::function<void()> _startCallback, _progressCallback;
	std::function<void(BYTE*, DWORD)> _dataCallback;
	std::ofstream* _stream = nullptr;

public:
	std::vector<BYTE> data;

	~download() {
		this->abort();
	}

	download(const session& sess, std::wstring url, std::wstring verb = L"GET") :
		_session{sess}, _url{url}, _verb{verb} { }

	download& abort() noexcept {
		if (this->_hRequest) {
			WinHttpCloseHandle(this->_hRequest);
			this->_hRequest = nullptr;
		}
		if (this->_hConnect) {
			WinHttpCloseHandle(this->_hConnect);
			this->_hConnect = nullptr;
		}
		this->_contentLength = this->_totalGot = 0;
		if (this->_stream != nullptr) {
			delete this->_stream;
			this->_stream = nullptr;
		}
		return *this;
	}

	download& add_request_header(const wchar_t* name, const wchar_t* value) {
		this->_requestHeaders[name] = value;
		return *this;
	}

	download& set_referrer(const std::wstring& referrer) {
		this->_referrer = referrer;
		return *this;
	}

	// Defines a lambda to be called once, right after the download starts.
	download& on_start(std::function<void()> callback) noexcept {
		this->_startCallback = std::move(callback);
		return *this;
	}

	// Defines a lambda do be called each time a chunk of bytes is received.
	download& on_progress(std::function<void()> callback) noexcept {
		this->_progressCallback = std::move(callback);
		return *this;
	}

	// Defines a lambda do be called each time a chunk of bytes is received.
	download& on_data(std::function<void(BYTE*, DWORD)> callback) noexcept {
		this->_dataCallback = std::move(callback);
		return *this;
	}

	// Set a file to write to instead of the data buffer.
	download& to_file(const std::wstring& path) {
		if (this->_stream != nullptr) {
			throw std::logic_error("A file stream is already configured.");
		}
		this->_stream = new std::ofstream(path, std::wofstream::out | std::wofstream::binary);
		return *this;
	}

	// Effectively starts the download, returning only after it completes.
	download& start() {
		if (this->_hConnect) {
			throw std::logic_error("A download is already in progress.");
		} else if (this->_url.empty()) {
			throw std::invalid_argument("Blank URL.");
		}

		this->_statusCode = 0;
		this->_contentLength = this->_totalGot = 0;
		this->_init_handles();
		this->_contact_server();
		this->_parse_headers();
		this->data.clear(); // prepare buffer to receive data
		if (this->_contentLength) { // server informed content length?
			this->data.reserve(this->_contentLength);
		}

		if (this->_startCallback) this->_startCallback(); // run user callback

		if (this->_hConnect && this->_hRequest) { // user didn't call abort()
			for (;;) {
				DWORD incomingBytes = this->_get_incoming_byte_count(); // chunk size about to come
				if (!incomingBytes) break; // no more bytes remaining
				this->_receive_bytes(incomingBytes); // chunk will be appended into this->data
				if (this->_progressCallback) this->_progressCallback();
				if (!this->_hConnect && !this->_hRequest) break; // user called abort()
			}
		}

		return this->abort(); // cleanup
	}

	const insert_order_map<std::wstring, std::wstring>& get_request_headers() const noexcept  { return this->_requestHeaders; }
	const insert_order_map<std::wstring, std::wstring>& get_response_headers() const noexcept { return this->_responseHeaders; }
	DWORD get_status_code() const noexcept       { return this->_statusCode; }
	size_t get_content_length() const noexcept   { return this->_contentLength; }
	size_t get_total_downloaded() const noexcept { return this->_totalGot; }

	// If server informed content length, returns a value between 0 and 100.
	float get_percent() const noexcept {
		return this->_contentLength ?
			(static_cast<float>(this->_totalGot) / this->_contentLength) * 100 :
			0;
	}

private:
	void _abort_and_throw(DWORD err, const char* msg) const {
		throw std::system_error(err, std::system_category(), msg);
	}

	void _init_handles() {
		// Crack the URL.
		url_crack crackedUrl;
		crackedUrl.crack(_url);

		// Open the connection handle.
		this->_hConnect = WinHttpConnect(this->_session.hsession(), crackedUrl.host(), crackedUrl.port(), 0);
		if (!this->_hConnect) {
			this->_abort_and_throw(GetLastError(), "WinHttpConnect failed");
		}

		// Build the request handle.
		std::wstring fullPath = crackedUrl.path_and_extra();
		this->_hRequest = WinHttpOpenRequest(this->_hConnect, this->_verb.c_str(),
			fullPath.c_str(), nullptr,
			this->_referrer.empty() ? WINHTTP_NO_REFERER : this->_referrer.c_str(),
			WINHTTP_DEFAULT_ACCEPT_TYPES,
			crackedUrl.is_https() ? WINHTTP_FLAG_SECURE : 0);
		if (!this->_hRequest) {
			this->_abort_and_throw(GetLastError(), "WinHttpOpenRequest failed");
		}
	}

	void _contact_server() {
		// Add the request headers to request handle.
		std::wstring rhTmp;
		rhTmp.reserve(20);
		for (const insert_order_map<std::wstring, std::wstring>::entry& rh : this->_requestHeaders) {
			rhTmp = rh.key;
			rhTmp += L": ";
			rhTmp += rh.value;

			if (!WinHttpAddRequestHeaders(this->_hRequest, rhTmp.c_str(), static_cast<ULONG>(-1L), WINHTTP_ADDREQ_FLAG_ADD)) {
				this->_abort_and_throw(GetLastError(), "WinHttpAddRequestHeaders failed");
			}
		}

		// Send the request to server.
		if (!WinHttpSendRequest(this->_hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
			this->_abort_and_throw(GetLastError(), "WinHttpSendRequest failed");
		}

		// Receive the response from server.
		if (!WinHttpReceiveResponse(this->_hRequest, nullptr)) {
			this->_abort_and_throw(GetLastError(), "WinHttpReceiveResponse failed");
		}
	}

	void _parse_headers() {
		// Retrieve the status code.
		DWORD dwSize = sizeof(DWORD);
		WinHttpQueryHeaders(this->_hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
			WINHTTP_HEADER_NAME_BY_INDEX, &_statusCode, &dwSize, WINHTTP_NO_HEADER_INDEX);

		// Retrieve the response header.
		DWORD rehSize = 0;
		WinHttpQueryHeaders(this->_hRequest, WINHTTP_QUERY_RAW_HEADERS_CRLF,
			WINHTTP_HEADER_NAME_BY_INDEX, WINHTTP_NO_OUTPUT_BUFFER, &rehSize, WINHTTP_NO_HEADER_INDEX);

		std::wstring rawReh(rehSize / sizeof(wchar_t), L'\0'); // raw response headers

		if (!WinHttpQueryHeaders(this->_hRequest, WINHTTP_QUERY_RAW_HEADERS_CRLF,
			WINHTTP_HEADER_NAME_BY_INDEX, &rawReh[0], &rehSize, WINHTTP_NO_HEADER_INDEX))
		{
			this->_abort_and_throw(GetLastError(), "WinHttpQueryHeaders failed");
		}

		// Parse the raw response headers into an associative array.
		this->_responseHeaders.clear();
		std::vector<std::wstring> lines = str::split_lines(rawReh);

		for (const std::wstring& line : lines) {
			if (line.empty()) {
				continue;
			}
			size_t colonIdx = line.find_first_of(L':');
			if (colonIdx == std::wstring::npos) { // not a key/value pair, probably response line
				this->_responseHeaders[L""] = line; // empty key
			} else {
				std::wstring kk = line.substr(0, colonIdx);
				std::wstring vv = line.substr(colonIdx + 1, line.length() - (colonIdx + 1));
				this->_responseHeaders[str::trim(kk)] = str::trim(vv);
			}
		}

		// Retrieve content length, if informed by server.
		const std::wstring* contLen = this->_responseHeaders.get_if_exists(L"Content-Length");
		if (contLen && str::is_uint(*contLen)) { // yes, server informed content length
			this->_contentLength = std::stoul(*contLen);
		}
	}

	DWORD _get_incoming_byte_count() {
		DWORD count = 0;
		if (!WinHttpQueryDataAvailable(this->_hRequest, &count)) { // how many bytes are about to come
			this->_abort_and_throw(GetLastError(), "WinHttpQueryDataAvailable failed");
		}
		return count;
	}

	void _receive_bytes(UINT nBytesToRead) {
		void* pWriteTo = nullptr;
		size_t beforeSize;
		if (this->_stream != nullptr) {
			this->data.resize(nBytesToRead); // make room
			pWriteTo = static_cast<void*>(&this->data[0]);
		} else {
			beforeSize = this->data.size();
			this->data.resize(beforeSize + nBytesToRead); // make room
			pWriteTo = static_cast<void*>(&this->data[beforeSize]);
		}

		DWORD readCount = 0;
		if (!WinHttpReadData(this->_hRequest, pWriteTo, nBytesToRead, &readCount)) {
			this->_abort_and_throw(GetLastError(), "WinHttpReadData failed");
		}

		this->_totalGot += readCount; // update total downloaded count

		if (this->_stream != nullptr) {
			this->_stream->write(static_cast<const char*>(pWriteTo), readCount); // write to stream
		} else {
			this->data.resize(beforeSize + readCount); // resize buffer to whatever was read
		}

		// Call dataCallback with the bytes we just got
		if (this->_dataCallback) {
			this->_dataCallback(static_cast<BYTE*>(pWriteTo), readCount);
		}
	}
};

}//namespace wl
