/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <string>
#include <system_error>
#include <Windows.h>
#include <winhttp.h>
#pragma comment(lib, "Winhttp.lib")

namespace wl {
namespace wli {

class download_url final {
private:
	URL_COMPONENTS _uc{};
	wchar_t        _scheme[16]{}, _host[64]{}, _user[64]{}, _pwd[64]{}, _path[256]{}, _extra[256]{};

public:
	download_url& crack(const wchar_t* address) {
		// This helper class simply breaks an URL address into several parts.
		SecureZeroMemory(&this->_uc, sizeof(this->_uc));
		this->_uc.dwStructSize = sizeof(this->_uc);

		this->_uc.lpszScheme    = this->_scheme; this->_uc.dwSchemeLength    = ARRAYSIZE(this->_scheme);
		this->_uc.lpszHostName  = this->_host;   this->_uc.dwHostNameLength  = ARRAYSIZE(this->_host);
		this->_uc.lpszUserName  = this->_user;   this->_uc.dwUserNameLength  = ARRAYSIZE(this->_user);
		this->_uc.lpszPassword  = this->_pwd;    this->_uc.dwPasswordLength  = ARRAYSIZE(this->_pwd);
		this->_uc.lpszUrlPath   = this->_path;   this->_uc.dwUrlPathLength   = ARRAYSIZE(this->_path);
		this->_uc.lpszExtraInfo = this->_extra;  this->_uc.dwExtraInfoLength = ARRAYSIZE(this->_extra);

		if (!WinHttpCrackUrl(address, 0, 0, &this->_uc)) {
			throw std::system_error(GetLastError(), std::system_category(),
				"WinHttpCrackUrl failed");
		}

		return *this;
	}

	download_url& crack(const std::wstring& address) {
		return this->crack(address.c_str());
	}

	const wchar_t* scheme() const   { return this->_scheme; }
	const wchar_t* host() const     { return this->_host; }
	const wchar_t* user() const     { return this->_user; }
	const wchar_t* pwd() const      { return this->_pwd; }
	const wchar_t* path() const     { return this->_path; }
	const wchar_t* extra() const    { return this->_extra; }
	int            port() const     { return this->_uc.nPort; }
	bool           is_https() const { return this->_uc.nScheme == INTERNET_SCHEME_HTTPS; }

	std::wstring path_and_extra() const noexcept {
		std::wstring ret = this->_path;
		ret.append(this->_extra);
		return ret;
	}
};

}//namespace wli
}//namespace wl
