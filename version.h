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

class version final {
public:
	std::array<UINT, 4> num;

	version() = default;
	explicit version(UINT major, UINT minor = 0, UINT build = 0, UINT revision = 0)
		: num({major, minor, build, revision}) { }
	version(const version&) = default;

	version& operator=(const version&) = default;

	bool operator==(const version& other) const { return this->num == other.num; }
	bool operator!=(const version& other) const { return !this->operator==(other); }
	bool operator>=(const version& other) const { return this->operator>(other) || this->operator==(other); }
	bool operator<=(const version& other) const { return this->operator<(other) || this->operator==(other); }
	bool operator<(const version& other)  const { return other > *this; }
	bool operator>(const version& other)  const {
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
		std::vector<std::wstring> fields = str::explode(text, L".");
		for (size_t i = 0; i < fields.size() && i <= 4; ++i) {
			if (!str::is_uint(fields[i])) {
				return false;
			}
			this->num[i] = std::stoi(fields[i]);
		}
		return true;
	}

	bool read(const std::wstring& exeOrDll) {
		DWORD szVer = GetFileVersionInfoSizeW(exeOrDll.c_str(), nullptr);
		if (!szVer) {
			throw std::system_error(GetLastError(), std::system_category(),
				"GetFileVersionInfoSize failed");
		}

		std::vector<wchar_t> infoBlock(szVer, L'\0');
		if (!GetFileVersionInfoW(exeOrDll.c_str(), 0, szVer, &infoBlock[0])) {
			throw std::system_error(GetLastError(), std::system_category(),
				"GetFileVersionInfo failed");
		}

		BYTE* lpBuf = nullptr;
		UINT blockSize = 0;
		if (!VerQueryValueW(&infoBlock[0], L"\\",
			reinterpret_cast<void**>(&lpBuf), &blockSize) ||
			!blockSize)
		{
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
};

}//namespace wl
