/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <string>
#include <system_error>
#include <vector>
#include "datetime.h"
#include <Shellapi.h>

namespace wl {

// Wrapper to a low-level HANDLE of a file.
class file final {
public:
	enum class access { READONLY, READWRITE };

	struct dates final {
		datetime creation;
		datetime lastAccess;
		datetime lastWrite;
	};

private:
	HANDLE _hFile = nullptr;
	access _access = access::READONLY;
	size_t _sz = -1;

public:
	~file() {
		this->close();
	}

	file() = default;
	file(file&& other) { this->operator=(std::move(other)); }

	file& operator=(file&& other) {
		this->close();
		std::swap(this->_hFile, other._hFile);
		std::swap(this->_access, other._access);
		std::swap(this->_sz, other._sz);
		return *this;
	}

	HANDLE hfile() const {
		return this->_hFile;
	}

	access access_type() const {
		return this->_access;
	}

	file& close() {
		if (this->_hFile) {
			CloseHandle(this->_hFile);
			this->_hFile = nullptr;
			this->_access = access::READONLY;
			this->_sz = -1; // http://stackoverflow.com/a/19483690
		}
		return *this;
	}

	size_t size() {
		if (this->_sz == -1) {
			this->_sz = GetFileSize(this->_hFile, nullptr); // cache
		}
		return this->_sz;
	}

private:
	file& _raw_open(const std::wstring& filePath, DWORD desiredAccess,
		DWORD shareMode, DWORD creationDisposition)
	{
		if (filePath.empty()) {
			throw std::invalid_argument("No file path specified.");
		}

		this->close();
		bool isReadWrite = (desiredAccess & GENERIC_WRITE) != 0;

		this->_hFile = CreateFileW(filePath.c_str(), desiredAccess, shareMode,
			nullptr, creationDisposition, 0, nullptr);
		if (this->_hFile == INVALID_HANDLE_VALUE) {
			this->_hFile = nullptr;
			throw std::system_error(GetLastError(), std::system_category(),
				isReadWrite ?
					"CreateFile failed to open file as read-write" :
					"CreateFile failed to open file as read-only");
		}

		this->_access = isReadWrite ?
			access::READWRITE : access::READONLY; // keep for future checks
		return *this;
	}

public:
	file& open_existing(const wchar_t* filePath, access accessType) {
		if (!file::exists(filePath)) {
			throw std::invalid_argument("File doesn't exist.");
		}
		return this->_raw_open(filePath,
			GENERIC_READ | (accessType == access::READWRITE ? GENERIC_WRITE : 0),
			(accessType == access::READWRITE) ? 0 : FILE_SHARE_READ,
			OPEN_EXISTING); // fails if file doesn't exist
	}

	file& open_existing(const std::wstring& filePath, access accessType) {
		return this->open_existing(filePath.c_str(), accessType);
	}

	file& open_or_create(const wchar_t* filePath) {
		// Intended to be called when you want to save a file, so it's always READWRITE.
		return this->_raw_open(filePath, GENERIC_READ | GENERIC_WRITE, 0, OPEN_ALWAYS);
	}

	file& open_or_create(const std::wstring& filePath) {
		return this->open_or_create(filePath.c_str());
	}

private:
	void _check_file_opened() const {
		if (!this->_hFile) {
			throw std::logic_error("File has not been opened.");
		}
	}

	void _check_file_read_only() const {
		if (this->_access == access::READONLY) {
			throw std::logic_error("File was opened for read-only access.");
		}
	}

public:
	file& set_new_size(size_t newSize) {
		// This method will truncate or expand the file, according to the new size.
		// Size zero will empty the file.
		this->_check_file_opened();
		this->_check_file_read_only();
		if (this->size() == newSize) return *this; // nothing to do

		auto tooBad = [this](DWORD err, const char* msg)->void {
			this->close();
			throw std::system_error(err, std::system_category(), msg);
		};

		DWORD r = SetFilePointer(this->_hFile, static_cast<LONG>(newSize), nullptr, FILE_BEGIN);
		if (r == INVALID_SET_FILE_POINTER) {
			tooBad(GetLastError(), "SetFilePointer failed when setting new file size");
		}

		if (!SetEndOfFile(this->_hFile)) {
			tooBad(GetLastError(), "SetEndOfFile failed when setting new file size");
		}

		r = SetFilePointer(this->_hFile, 0, nullptr, FILE_BEGIN); // rewind
		if (r == INVALID_SET_FILE_POINTER) {
			tooBad(GetLastError(), "SetFilePointer failed to rewind the file pointer when setting new file size");
		}

		this->_sz = newSize; // update
		return *this;
	}

	file& rewind() {
		this->_check_file_opened();
		if (SetFilePointer(this->_hFile, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
			throw std::system_error(GetLastError(), std::system_category(),
				"SetFilePointer failed to rewind the file");
		}
		return *this;
	}

	file& read_to_buffer(std::vector<BYTE>& buf) {
		this->_check_file_opened();
		buf.resize(this->size());
		DWORD bytesRead = 0;
		if (!ReadFile(this->_hFile, &buf[0], static_cast<DWORD>(buf.size()), &bytesRead, nullptr)) {
			throw std::system_error(GetLastError(), std::system_category(),
				"ReadFile failed");
		}
		return *this;
	}

	std::vector<BYTE> read() {
		std::vector<BYTE> buf;
		this->read_to_buffer(buf);
		return buf;
	}

	file& write(const BYTE* pData, size_t sz) {
		this->_check_file_opened();
		this->_check_file_read_only();

		// File boundary will be expanded if needed.
		// Internal file pointer will move forward.
		DWORD dwWritten = 0;
		if (!WriteFile(this->_hFile, pData, static_cast<DWORD>(sz), &dwWritten, nullptr)) {
			throw std::system_error(GetLastError(), std::system_category(),
				"WriteFile failed");
		}
		return *this;
	}

	file& write(const std::vector<BYTE>& data) {
		return this->write(&data[0], data.size());
	}

	dates get_dates() const {
		this->_check_file_opened();
		FILETIME ftCreation{}, ftLastAccess{}, ftLastWrite{};
		if (!GetFileTime(this->_hFile, &ftCreation, &ftLastAccess, &ftLastWrite)) {
			throw std::system_error(GetLastError(), std::system_category(),
				"GetFileTime failed");
		}
		return {ftCreation, ftLastAccess, ftLastWrite};
	}

public:
	static void quick_read_to_buffer(const wchar_t* filePath, std::vector<BYTE>& buf) {
		file fin;
		fin.open_existing(filePath, access::READONLY);
		fin.read_to_buffer(buf);
	}

	static std::vector<BYTE> quick_read(const wchar_t* filePath) {
		std::vector<BYTE> buf;
		quick_read_to_buffer(filePath, buf);
		return buf;
	}

	static void              quick_read_to_buffer(const std::wstring& filePath, std::vector<BYTE>& buf) { quick_read_to_buffer(filePath.c_str(), buf); }
	static std::vector<BYTE> quick_read(const std::wstring& filePath)                                   { return quick_read(filePath.c_str()); }

	static void quick_write(const wchar_t* filePath, const BYTE* pData, size_t sz) {
		file fout;
		fout.open_or_create(filePath);
		fout.set_new_size(sz);
		fout.write(pData, sz);
	}

	static void quick_write(const std::wstring& filePath, const BYTE* pData, size_t sz)  { quick_write(filePath.c_str(), pData, sz); }
	static void quick_write(const wchar_t* filePath, const std::vector<BYTE>& data)      { quick_write(filePath, &data[0], data.size()); }
	static void quick_write(const std::wstring& filePath, const std::vector<BYTE>& data) { quick_write(filePath.c_str(), &data[0], data.size()); }

	static bool exists(const wchar_t* fileOrFolder) {
		return GetFileAttributesW(fileOrFolder) != INVALID_FILE_ATTRIBUTES;
	}

	static bool is_dir(const wchar_t* thePath) {
		return (GetFileAttributesW(thePath) & FILE_ATTRIBUTE_DIRECTORY) != 0;
	}

	static bool is_hidden(const wchar_t* thePath) {
		return (GetFileAttributesW(thePath) & FILE_ATTRIBUTE_HIDDEN) != 0;
	}

	static bool exists(const std::wstring& fileOrFolder) { return exists(fileOrFolder.c_str()); }
	static bool is_dir(const std::wstring& thePath)      { return is_dir(thePath.c_str()); }
	static bool is_hidden(const std::wstring& thePath)   { return is_hidden(thePath.c_str()); }

	static void del(const std::wstring& fileOrFolder) {
		if (is_dir(fileOrFolder)) {
			// http://stackoverflow.com/q/1468774/6923555
			wchar_t szDir[MAX_PATH + 1]{}; // +1 for the double null terminate
			lstrcpyW(szDir, fileOrFolder.c_str());

			SHFILEOPSTRUCTW fos{};
			fos.wFunc  = FO_DELETE;
			fos.pFrom  = szDir;
			fos.fFlags = FOF_NO_UI;

			if (SHFileOperationW(&fos)) {
				throw std::system_error(ERROR_INVALID_FUNCTION, std::system_category(), // arbitrary error code
					"SHFileOperation failed to recursively delete directory, can't specify error");
			}
		} else {
			if (!DeleteFileW(fileOrFolder.c_str())) {
				throw std::system_error(GetLastError(), std::system_category(),
					"DeleteFile failed");
			}
		}
	}

	static void create_dir(const wchar_t* thePath) {
		if (!CreateDirectoryW(thePath, nullptr)) {
			throw std::system_error(GetLastError(), std::system_category(),
				"CreateDirectory failed");
		}
	}

	static void create_dir(const std::wstring& thePath) {
		create_dir(thePath.c_str());
	}

	static std::vector<std::wstring> list_dir(const std::wstring& pathAndPattern) {
		// Entry example: "C:\\myfolder\\*.mp3"
		std::vector<std::wstring> files;

		WIN32_FIND_DATA wfd{};
		HANDLE hFind = FindFirstFileW(pathAndPattern.c_str(), &wfd);
		if (hFind == INVALID_HANDLE_VALUE) {
			DWORD err = GetLastError();
			if (err == ERROR_FILE_NOT_FOUND) {
				return files;
			} else {
				throw std::system_error(err, std::system_category(),
					"FindFirstFile failed");
			}
		}

		std::wstring pathPat = pathAndPattern.substr(0,
			pathAndPattern.find_last_of(L'\\')); // no trailing backslash
		do {
			if (*wfd.cFileName) {
				files.emplace_back(pathPat);
				files.back().append(L"\\").append(wfd.cFileName);
			}
		} while (FindNextFileW(hFind, &wfd));

		FindClose(hFind);
		return files;
	}

	static std::vector<std::wstring> list_dir(const std::wstring& dirPath, const std::wstring& pattern) {
		std::wstring pathAndPattern = dirPath;
		if (pathAndPattern.back() != L'\\') pathAndPattern.append(L"\\");
		pathAndPattern.append(pattern);
		return list_dir(pathAndPattern);
	}
};

}//namespace wl
