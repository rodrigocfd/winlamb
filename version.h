/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <array>
#include <system_error>
#include "str.h"
#pragma comment(lib, "Version.lib")

namespace wl {

// Parses version information from an EXE or DLL.
class version final {
public:
	std::array<UINT, 4> num;

	version() = default;
	explicit version(UINT major, UINT minor = 0, UINT build = 0, UINT revision = 0) noexcept
		: num{major, minor, build, revision} { }

	version(const version&) = default;
	version& operator=(const version&) = default;

	bool operator==(const version& other) const noexcept { return this->num == other.num; }
	bool operator!=(const version& other) const noexcept { return !this->operator==(other); }
	bool operator>=(const version& other) const noexcept { return this->operator>(other) || this->operator==(other); }
	bool operator<=(const version& other) const noexcept { return this->operator<(other) || this->operator==(other); }
	bool operator<(const version& other)  const noexcept { return other > *this; }
	bool operator>(const version& other)  const noexcept {
		for (size_t i = 0; i < 4; ++i) {
			if (this->num[i] > other.num[i]) {
				return true;
			} else if (this->num[i] < other.num[i]) {
				return false;
			}
		}
		return false;
	}

	std::wstring to_string(BYTE numDigits = 4) const {
		std::wstring ret;
		if (numDigits) {
			ret.append(std::to_wstring(this->num[0]));
			for (size_t i = 1; i < numDigits && i <= 4; ++i) {
				ret.append(L".")
					.append(std::to_wstring(this->num[i]));
			}
		}
		return ret;
	}

	bool parse(const std::wstring& text) {
		std::vector<std::wstring> fields = str::split(text, L".");
		for (size_t i = 0; i < fields.size() && i <= 4; ++i) {
			if (!str::is_uint(fields[i])) {
				return false;
			}
			this->num[i] = std::stoi(fields[i]);
		}
		return true;
	}

	// Reads version of an executable or DLL file.
	bool read(const wchar_t* exeOrDll) {
		DWORD szVer = GetFileVersionInfoSizeW(exeOrDll, nullptr);
		if (!szVer) {
			throw std::system_error(GetLastError(), std::system_category(),
				"GetFileVersionInfoSize failed");
		}

		std::vector<wchar_t> infoBlock(szVer, L'\0');
		if (!GetFileVersionInfoW(exeOrDll, 0, szVer, &infoBlock[0])) {
			throw std::system_error(GetLastError(), std::system_category(),
				"GetFileVersionInfo failed");
		}

		BYTE* lpBuf = nullptr;
		UINT blockSize = 0;
		if (!VerQueryValueW(&infoBlock[0], L"\\",
			reinterpret_cast<void**>(&lpBuf), &blockSize) ||
			!blockSize)
		{
			this->num = {0, 0, 0, 0};
			return false; // no information available, not an error
		}

		VS_FIXEDFILEINFO* verInfo = reinterpret_cast<VS_FIXEDFILEINFO*>(lpBuf);
		this->num = {
			(verInfo->dwFileVersionMS >> 16) & 0xffff,
			(verInfo->dwFileVersionMS >>  0) & 0xffff,
			(verInfo->dwFileVersionLS >> 16) & 0xffff,
			(verInfo->dwFileVersionLS >>  0) & 0xffff
		};

		return true;
	}

	// Reads version of an executable or DLL file.
	bool read(const std::wstring& exeOrDll) {
		return this->read(exeOrDll.c_str());
	}

	// Reads version of current executable or DLL file itself.
	bool read_current_exe() {
		wchar_t buf[MAX_PATH + 1]{};
		if (!GetModuleFileNameW(nullptr, buf, ARRAYSIZE(buf))) {
			throw std::system_error(GetLastError(), std::system_category(),
				"GetModuleFileName failed");
		}
		return this->read(buf);
	}
};

}//namespace wl
