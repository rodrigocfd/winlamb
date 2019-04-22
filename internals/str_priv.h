/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <cwctype>
#include <string>
#include <Windows.h>

namespace wl {
namespace _wli {
namespace str_priv {

template<typename T>
inline T format_raw_arg(T val) noexcept {
	static_assert(!std::is_same<T, const char*>::value,
		"Non-wide char* being used on str::format(), str::to_wstring() can fix it.");
	static_assert(!std::is_same<T, std::string>::value,
		"Non-wide std::string being used on str::format(), str::to_wstring() can fix it.");
	return val;
}

inline const wchar_t* format_raw_arg(const std::wstring& val) noexcept {
	return val.c_str(); // so format() will also accept wstring in formatting list
}

template<typename ...argsT>
inline std::wstring format_raw(size_t strFormatLen, const wchar_t* strFormat, const argsT&... args) {
	// https://msdn.microsoft.com/en-us/magazine/dn913181.aspx
	// https://stackoverflow.com/a/514921/6923555
	size_t len = swprintf(nullptr, 0, strFormat, format_raw_arg(args)...);
	std::wstring ret(len + 1, L'\0'); // room for terminating null
	swprintf(&ret[0], len + 1, strFormat, format_raw_arg(args)...);
	ret.resize(len); // remove terminating null
	return ret;
}

inline bool ends_begins_first_check(const std::wstring& s, const wchar_t* what, size_t& whatLen) noexcept {
	if (s.empty()) return false;

	whatLen = lstrlenW(what);
	if (!whatLen || whatLen > s.length()) {
		return false;
	}
	return true;
}

inline std::wstring parse_ascii(const BYTE* data, size_t sz) {
	std::wstring ret;
	if (data && sz) {
		ret.resize(sz);
		for (size_t i = 0; i < sz; ++i) {
			if (!data[i]) { // found terminating null
				ret.resize(i);
				return ret;
			}
			ret[i] = static_cast<wchar_t>(data[i]); // raw conversion
		}
	}
	return ret; // data didn't have a terminating null
}

inline std::wstring parse_encoded(const BYTE* data, size_t sz, UINT codePage) {
	std::wstring ret;
	if (data && sz) {
		int neededLen = MultiByteToWideChar(codePage, 0, reinterpret_cast<const char*>(data),
			static_cast<int>(sz), nullptr, 0);
		ret.resize(neededLen);
		MultiByteToWideChar(codePage, 0, reinterpret_cast<const char*>(data),
			static_cast<int>(sz), &ret[0], neededLen);
		ret.resize( lstrlenW(ret.c_str()) ); // trim_nulls()
	}
	return ret;
}

}//namespace str_priv
}//namespace wli
}//namespace wl
