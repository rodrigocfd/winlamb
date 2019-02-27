/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <cwctype>
#include <string>
#include <vector>
#include <Windows.h>

namespace wl {

// Utilities to std::wstring.
class str final {
private:
	str() = delete;

public:
	// Trims the string using std::iswspace to validate spaces.
	static std::wstring& trim(std::wstring& s) {
		if (s.empty()) return s;
		trim_nulls(s);

		size_t len = s.length();
		size_t iFirst = 0, iLast = len - 1; // bounds of trimmed string
		bool onlySpaces = true; // our string has only spaces?

		for (size_t i = 0; i < len; ++i) {
			if (!std::iswspace(s[i])) {
				iFirst = i;
				onlySpaces = false;
				break;
			}
		}
		if (onlySpaces) {
			s.clear();
			return s;
		}

		for (size_t i = len; i-- > 0; ) {
			if (!std::iswspace(s[i])) {
				iLast = i;
				break;
			}
		}

		std::copy(s.begin() + iFirst, // move the non-space chars back
			s.begin() + iLast + 1, s.begin());
		s.resize(iLast - iFirst + 1); // trim container size
		return s;
	}

	// Removes any padding zeroes after the string, making size correct.
	static std::wstring& trim_nulls(std::wstring& s) {
		// When a std::wstring is initialized with any length, possibly to be used as a buffer,
		// the string length may not match the size() method, after the operation.
		// This function fixes this.
		if (!s.empty()) {
			s.resize( lstrlenW(s.c_str()) );
		}
		return s;
	}

private:
	template<typename T>
	static T _format_arg(T val) noexcept {
		static_assert(!std::is_same<T, const char*>::value,
			"Non-wide char* being used on str::format(), str::to_wstring() can fix it.");
		static_assert(!std::is_same<T, std::string>::value,
			"Non-wide std::string being used on str::format(), str::to_wstring() can fix it.");
		return val;
	}

	static const wchar_t* _format_arg(const std::wstring& val) noexcept {
		return val.c_str(); // so format() will also accept wstring in formatting list
	}

	template<typename ...argsT>
	static std::wstring _raw_format(size_t strFormatLen, const wchar_t* strFormat, const argsT&... args) {
		// https://msdn.microsoft.com/en-us/magazine/dn913181.aspx
		// https://stackoverflow.com/a/514921/6923555
		size_t len = swprintf(nullptr, 0, strFormat, _format_arg(args)...);
		std::wstring ret(len + 1, L'\0'); // room for terminating null
		swprintf(&ret[0], len + 1, strFormat, _format_arg(args)...);
		ret.resize(len); // remove terminating null
		return ret;
	}

public:
	// Type-safe sprintf.
	template<typename ...argsT>
	static std::wstring format(const wchar_t* strFormat, const argsT&... args) {
		return _raw_format(lstrlenW(strFormat), strFormat, std::forward<const argsT&>(args)...);
	}

	// Type-safe sprintf.
	template<typename ...argsT>
	static std::wstring format(const std::wstring& strFormat, const argsT&... args) {
		return _raw_format(strFormat.length(), strFormat.c_str(), std::forward<const argsT&>(args)...);
	}

	// Compares two strings, case insensitive.
	static bool eqi(const std::wstring& s, const wchar_t* what) noexcept {
		return !lstrcmpiW(s.c_str(), what); // str::eq() would be just operator==(), that's why there's no str::eq()
	}

	// Compares two strings, case insensitive.
	static bool eqi(const std::wstring& s, const std::wstring& what) noexcept {
		return eqi(s.c_str(), what.c_str());
	}

private:
	static bool _ends_begins_first_check(const std::wstring& s, const wchar_t* what, size_t& whatLen) noexcept {
		if (s.empty()) return false;

		whatLen = lstrlenW(what);
		if (!whatLen || whatLen > s.length()) {
			return false;
		}
		return true;
	}

public:
	// Checks, case sensitive, if the string ends with the given text.
	static bool ends_with(const std::wstring& s, const wchar_t* what) noexcept {
		size_t whatLen = 0;
		if (!_ends_begins_first_check(s, what, whatLen)) {
			return false;
		}
		return !lstrcmpW(s.c_str() + s.length() - whatLen, what);
	}

	// Checks, case insensitive, if the string ends with the given text.
	static bool ends_withi(const std::wstring& s, const wchar_t* what) noexcept {
		size_t whatLen = 0;
		if (!_ends_begins_first_check(s, what, whatLen)) {
			return false;
		}
		return !lstrcmpiW(s.c_str() + s.length() - whatLen, what);
	}

	// Checks, case sensitive, if the string begins with the given text.
	static bool begins_with(const std::wstring& s, const wchar_t* what) noexcept {
		size_t whatLen = 0;
		if (!_ends_begins_first_check(s, what, whatLen)) {
			return false;
		}
		return !wcsncmp(s.c_str(), what, whatLen);
	}

	// Checks, case insensitive, if the string begins with the given text.
	static bool begins_withi(const std::wstring& s, const wchar_t* what) noexcept {
		size_t whatLen = 0;
		if (!_ends_begins_first_check(s, what, whatLen)) {
			return false;
		}
		return !_wcsnicmp(s.c_str(), what, whatLen);
	}

	// Converts string to uppercase.
	static std::wstring upper(const std::wstring& s) {
		std::wstring ret = s;
		CharUpperBuffW(&ret[0], static_cast<DWORD>(ret.length()));
		return ret;
	}

	// Converts string to lowercase.
	static std::wstring lower(const std::wstring& s) {
		std::wstring ret = s;
		CharLowerBuffW(&ret[0], static_cast<DWORD>(ret.length()));
		return ret;
	}

	// Reverses the string.
	static std::wstring& reverse(std::wstring& s) noexcept {
		size_t lim = (s.length() - (s.length() % 2)) / 2;
		for (size_t i = 0; i < lim; ++i) {
			std::swap(s[i], s[s.length() - i - 1]);
		}
		return s;
	}

	// Reverses the string.
	static std::wstring reverse(const wchar_t* s) {
		std::wstring ret = s;
		return reverse(s);
	}

	// Simple diacritics removal.
	static std::wstring& remove_diacritics(std::wstring& s) noexcept {
		const wchar_t* diacritics   = L"ÁáÀàÃãÂâÄäÉéÈèÊêËëÍíÌìÎîÏïÓóÒòÕõÔôÖöÚúÙùÛûÜüÇçÅåÐðÑñØøÝý";
		const wchar_t* replacements = L"AaAaAaAaAaEeEeEeEeIiIiIiIiOoOoOoOoOoUuUuUuUuCcAaDdNnOoYy";
		for (wchar_t& ch : s) {
			const wchar_t* pDiac = diacritics;
			const wchar_t* pRepl = replacements;
			while (*pDiac) {
				if (ch == *pDiac) ch = *pRepl; // in-place replacement
				++pDiac;
				++pRepl;
			}
		}
		return s;
	}

	// Finds index of substring within string, case insensitive.
	static size_t findi(const std::wstring& haystack, const wchar_t* needle, size_t offset = 0) {
		std::wstring haystack2 = upper(haystack);
		std::wstring needle2 = needle;
		CharUpperBuffW(&needle2[0], static_cast<DWORD>(needle2.length()));
		return haystack2.find(needle2, offset);
	}

	// Finds index of substring within string, case insensitive.
	static size_t findi(const std::wstring& haystack, const std::wstring& needle, size_t offset = 0) {
		return findi(haystack, needle.c_str(), offset);
	}

	// Finds index of substring within string, case insensitive, reverse search.
	static size_t rfindi(const std::wstring& haystack, const wchar_t* needle, size_t offset = 0) {
		std::wstring haystack2 = upper(haystack);
		std::wstring needle2 = needle;
		CharUpperBuffW(&needle2[0], static_cast<DWORD>(needle2.length()));
		return haystack2.rfind(needle2, offset);
	}

	// Finds index of substring within string, case insensitive, reverse search.
	static size_t rfindi(const std::wstring& haystack, const std::wstring& needle, size_t offset = 0) {
		return rfindi(haystack, needle, offset);
	}

	// Finds all occurrences of a substring, case sensitive, and replaces them all.
	static std::wstring& replace(std::wstring& haystack, const std::wstring& needle, const std::wstring& replacement) {
		if (haystack.empty() || needle.empty()) return haystack;

		std::wstring output;
		size_t base = 0;
		size_t found = 0;

		for (;;) {
			found = haystack.find(needle, found);
			output.insert(output.length(), haystack, base, found - base);
			if (found != std::wstring::npos) {
				output.append(replacement);
				base = found = found + needle.length();
			} else {
				break;
			}
		}

		haystack.swap(output); // behaves like an in-place operation
		return haystack;
	}

	// Finds all occurrences of a substring, case insensitive, and replaces them all.
	static std::wstring& replacei(std::wstring& haystack, const std::wstring& needle, const std::wstring& replacement) {
		if (haystack.empty() || needle.empty()) return haystack;

		std::wstring haystackU = upper(haystack);
		std::wstring needleU = upper(needle);

		std::wstring output;
		size_t base = 0;
		size_t found = 0;

		for (;;) {
			found = haystackU.find(needleU, found);
			output.insert(output.length(), haystack, base, found - base);
			if (found != std::wstring::npos) {
				output.append(replacement);
				base = found = found + needle.length();
			} else {
				break;
			}
		}

		haystack.swap(output); // behaves like an in-place operation
		return haystack;
	}

	// Does the string represent a signed int?
	static bool is_int(const std::wstring& s) noexcept {
		if (s.empty()) return false;
		if (s[0] != L'-' && !std::iswdigit(s[0]) && !std::iswblank(s[0])) return false;
		for (wchar_t ch : s) {
			if (!std::iswdigit(ch) && !std::iswblank(ch)) return false;
		}
		return true;
	}

	// Does the string represent an unsigned int?
	static bool is_uint(const std::wstring& s) noexcept {
		if (s.empty()) return false;
		for (wchar_t ch : s) {
			if (!std::iswdigit(ch) && !std::iswblank(ch)) return false;
		}
		return true;
	}

	// Does the string represent a hexadecimal int?
	static bool is_hex(const std::wstring& s) noexcept {
		if (s.empty()) return false;
		for (wchar_t ch : s) {
			if (!std::iswxdigit(ch) && !std::iswblank(ch)) return false;
		}
		return true;
	}

	// Does the string represent a float?
	static bool is_float(const std::wstring& s) noexcept {
		if (s.empty()) return false;
		if (s[0] != L'-' && s[0] != L'.' && !std::iswdigit(s[0]) && !std::iswblank(s[0])) return false;

		bool hasDot = false;
		for (wchar_t ch : s) {
			if (ch == L'.') {
				if (hasDot) {
					return false;
				} else {
					hasDot = true;
				}
			} else {
				if (!std::iswdigit(ch) && !std::iswblank(ch)) return false;
			}
		}
		return true;
	}

	// Splits the string at the given characters. The characters themselves
	// will be removed.
	static std::vector<std::wstring> split(const std::wstring& s, const wchar_t* delimiter) {
		std::vector<std::wstring> ret;
		if (s.empty()) return ret;

		if (!delimiter) {
			ret.emplace_back(s); // one single line
			return ret;
		}

		size_t base = 0, head = 0;

		for (;;) {
			head = s.find(delimiter, head);
			if (head == std::wstring::npos) break;
			ret.emplace_back();
			ret.back().insert(0, s, base, head - base);
			head += lstrlenW(delimiter);
			base = head;
		}

		ret.emplace_back();
		ret.back().insert(0, s, base, s.length() - base);
		return ret;
	}

	// Splits the string at the given characters. The characters themselves
	// will be removed.
	static std::vector<std::wstring> split(const std::wstring& s, const std::wstring& delimiter) {
		return split(s, delimiter.c_str());
	}

	// Splits a string line by line.
	static std::vector<std::wstring> split_lines(const std::wstring& s) {
		return split(s, get_linebreak(s));
	}

	// Splits a zero-delimited multi-string.
	static std::vector<std::wstring> split_multi_zero(const wchar_t* s) {
		// Example multi-zero string:
		// L"first one\0second one\0third one\0"
		// Assumes a well-formed multiStr, which ends with two nulls.

		// Count number of null-delimited strings; string end with double null.
		size_t numStrings = 0;
		const wchar_t* pRun = s;
		while (*pRun) {
			++numStrings;
			pRun += lstrlenW(pRun) + 1;
		}

		// Alloc return array of strings.
		std::vector<std::wstring> ret;
		ret.reserve(numStrings);

		// Copy each string.
		pRun = s;
		for (size_t i = 0; i < numStrings; ++i) {
			ret.emplace_back(pRun);
			pRun += lstrlenW(pRun) + 1;
		}
		return ret;
	}

	// Splits string into tokens, which may be enclosed in double quotes.
	static std::vector<std::wstring> split_quoted(const wchar_t* s) {
		// Example quoted string:
		// "First one" NoQuoteSecond "Third one"

		// Count number of strings.
		size_t numStrings = 0;
		const wchar_t* pRun = s;
		while (*pRun) {
			if (*pRun == L'\"') { // begin of quoted string
				++pRun; // point to 1st char of string
				for (;;) {
					if (!*pRun) {
						break; // won't compute open-quoted
					} else if (*pRun == L'\"') {
						++pRun; // point to 1st char after closing quote
						++numStrings;
						break;
					}
					++pRun;
				}
			} else if (!std::iswspace(*pRun)) { // 1st char of non-quoted string
				++pRun; // point to 2nd char of string
				while (*pRun && !std::iswspace(*pRun) && *pRun != L'\"') ++pRun; // passed string
				++numStrings;
			} else {
				++pRun; // some white space
			}
		}

		// Alloc return array of strings.
		std::vector<std::wstring> ret;
		ret.reserve(numStrings);

		// Alloc and copy each string.
		pRun = s;
		const wchar_t* pBase;
		int i = 0;
		while (*pRun) {
			if (*pRun == L'\"') { // begin of quoted string
				++pRun; // point to 1st char of string
				pBase = pRun;
				for (;;) {
					if (!*pRun) {
						break; // won't compute open-quoted
					} else if (*pRun == L'\"') {
						ret.emplace_back();
						ret.back().insert(0, pBase, pRun - pBase); // copy to buffer
						++i; // next string

						++pRun; // point to 1st char after closing quote
						break;
					}
					++pRun;
				}
			} else if (!std::iswspace(*pRun)) { // 1st char of non-quoted string
				pBase = pRun;
				++pRun; // point to 2nd char of string
				while (*pRun && !std::iswspace(*pRun) && *pRun != L'\"') ++pRun; // passed string

				ret.emplace_back();
				ret.back().insert(0, pBase, pRun - pBase); // copy to buffer
				++i; // next string
			} else {
				++pRun; // some white space
			}
		}

		return ret;
	}

	// Splits string into tokens, which may be enclosed in double quotes.
	static std::vector<std::wstring> split_quoted(const std::wstring& s) {
		return split_quoted(s.c_str());
	}

	enum class encoding { UNKNOWN, ASCII, WIN1252, UTF8, UTF16BE, UTF16LE, UTF32BE, UTF32LE, SCSU, BOCU1 };
	struct encoding_info final {
		encoding encType = encoding::UNKNOWN;
		size_t   bomSize = 0;
	};

	// Encoding information about the given string.
	static encoding_info get_encoding(const BYTE* data, size_t sz) noexcept {
		auto match = [&](const BYTE* pBom, int szBom) noexcept->bool {
			return (sz >= static_cast<size_t>(szBom)) &&
				!memcmp(data, pBom, sizeof(BYTE) * szBom);
		};

		// https://en.wikipedia.org/wiki/Byte_order_mark

		BYTE utf8[] = {0xEF, 0xBB, 0xBF}; // UTF-8 BOM
		if (match(utf8, 3)) return {encoding::UTF8, 3}; // BOM size in bytes

		BYTE utf16be[] = {0xFE, 0xFF};
		if (match(utf16be, 2)) return {encoding::UTF16BE, 2};

		BYTE utf16le[] = {0xFF, 0xFE};
		if (match(utf16le, 2)) return {encoding::UTF16LE, 2};

		BYTE utf32be[] = {0x00, 0x00, 0xFE, 0xFF};
		if (match(utf32be, 4)) return {encoding::UTF32BE, 4};

		BYTE utf32le[] = {0xFF, 0xFE, 0x00, 0x00};
		if (match(utf32le, 4)) return {encoding::UTF32LE, 4};

		BYTE scsu[] = {0x0E, 0xFE, 0xFF};
		if (match(scsu, 3)) return {encoding::SCSU, 3};

		BYTE bocu1[] = {0xFB, 0xEE, 0x28};
		if (match(bocu1, 3)) return {encoding::BOCU1, 3};

		// No BOM found, guess UTF-8 without BOM, or Windows-1252 (superset of ISO-8859-1).
		bool canBeWin1252 = false;
		for (size_t i = 0; i < sz; ++i) {
			if (data[i] > 0x7F) { // 127
				canBeWin1252 = true;
				if (i <= sz - 2 && (
					(data[i] == 0xC2 && (data[i+1] >= 0xA1 && data[i+1] <= 0xBF)) || // http://www.utf8-chartable.de
					(data[i] == 0xC3 && (data[i+1] >= 0x80 && data[i+1] <= 0xBF)) ))
				{
					return {encoding::UTF8, 0}; // UTF-8 without BOM
				}
			}
		}
		return {(canBeWin1252 ? encoding::WIN1252 : encoding::ASCII), 0};
	}

	// Encoding information about the given string.
	static encoding_info get_encoding(const std::vector<BYTE>& data) noexcept {
		return get_encoding(&data[0], data.size());
	}

	// What linebreak is being used on a given string (unknown, N, R, RN or NR). If different
	// linebreaks are used, only the first one is reported.
	static const wchar_t* get_linebreak(const std::wstring& s) noexcept {
		for (size_t i = 0; i < s.length() - 1; ++i) {
			if (s[i] == L'\r') {
				return s[i + 1] == L'\n' ? L"\r\n" : L"\r";
			} else if (s[i] == L'\n') {
				return s[i + 1] == L'\r' ? L"\n\r" : L"\n";
			}
		}
		return nullptr; // unknown
	}

	enum class write_bom { YES, NO };

	// Converts a string to an UTF-8 blob, ready to be written to a file.
	static std::vector<BYTE> to_utf8_blob(const std::wstring& s, write_bom writeBom) {
		std::vector<BYTE> ret;
		if (!s.empty()) {
			BYTE utf8bom[]{0xEF, 0xBB, 0xBF};
			int szBom = (writeBom == write_bom::YES) ? ARRAYSIZE(utf8bom) : 0;

			int neededLen = WideCharToMultiByte(CP_UTF8, 0,
				s.c_str(), static_cast<int>(s.length()),
				nullptr, 0, nullptr, 0);
			ret.resize(neededLen + szBom);

			if (writeBom == write_bom::YES) {
				memcpy(&ret[0], utf8bom, szBom);
			}

			WideCharToMultiByte(CP_UTF8, 0,
				s.c_str(), static_cast<int>(s.length()),
				reinterpret_cast<char*>(&ret[0 + szBom]),
				neededLen, nullptr, nullptr);
		}
		return ret;
	}

	// Converts wstring to string.
	static std::string to_ascii(const std::wstring& s) {
		std::string ret(s.length(), '\0');
		for (size_t i = 0; i < s.length(); ++i) {
			ret[i] = static_cast<char>(s[i]); // raw conversion
		}
		return ret;
	}

private:
	static std::wstring _parse_ascii(const BYTE* data, size_t sz) {
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

	static std::wstring _parse_encoded(const BYTE* data, size_t sz, UINT codePage) {
		std::wstring ret;
		if (data && sz) {
			int neededLen = MultiByteToWideChar(codePage, 0, reinterpret_cast<const char*>(data),
				static_cast<int>(sz), nullptr, 0);
			ret.resize(neededLen);
			MultiByteToWideChar(codePage, 0, reinterpret_cast<const char*>(data),
				static_cast<int>(sz), &ret[0], neededLen);
			trim_nulls(ret);
		}
		return ret;
	}

public:
	// Conversion to wstring.
	static std::wstring to_wstring(const BYTE* data, size_t sz) {
		if (!data || !sz) return {};

		encoding_info fileEnc = get_encoding(data, sz);
		data += fileEnc.bomSize; // skip BOM, if any

		switch (fileEnc.encType) {
		case encoding::UNKNOWN:
		case encoding::ASCII:   return _parse_ascii(data, sz);
		case encoding::WIN1252: return _parse_encoded(data, sz, 1252);
		case encoding::UTF8:    return _parse_encoded(data, sz, CP_UTF8);
		case encoding::UTF16BE: throw std::invalid_argument("UTF-16 big endian: encoding not implemented.");
		case encoding::UTF16LE: throw std::invalid_argument("UTF-16 little endian: encoding not implemented.");
		case encoding::UTF32BE: throw std::invalid_argument("UTF-32 big endian: encoding not implemented.");
		case encoding::UTF32LE: throw std::invalid_argument("UTF-32 little endian: encoding not implemented.");
		case encoding::SCSU:    throw std::invalid_argument("Standard compression scheme for Unicode: encoding not implemented.");
		case encoding::BOCU1:   throw std::invalid_argument("Binary ordered compression for Unicode: encoding not implemented.");
		default:                throw std::invalid_argument("Unknown encoding.");
		}
	}

	// Conversion to wstring.
	static std::wstring to_wstring(const std::vector<BYTE>& data) {
		return to_wstring(&data[0], data.size());
	}

	// Conversion to wstring.
	static std::wstring to_wstring(const char* s) {
		return _parse_ascii(reinterpret_cast<const BYTE*>(s), lstrlenA(s));
	}

	// Conversion to wstring.
	static std::wstring to_wstring(const std::string& s) {
		return to_wstring(s.c_str());
	}

	// Converts number to wstring, adding thousand separator.
	static std::wstring to_wstring_with_separator(int number, wchar_t separator = L',') {
		std::wstring ret;
		ret.reserve(32); // arbitrary

		int abso = abs(number);
		BYTE blocks = 0;
		while (abso >= 1000) {
			abso = (abso - (abso % 1000)) / 1000;
			++blocks;
		}

		abso = abs(number);
		bool firstPass = true;
		do {
			int num = abso % 1000;
			wchar_t buf[8]{};

			if (blocks) {
				if (num < 100) lstrcatW(buf, L"0");
				if (num < 10) lstrcatW(buf, L"0");
			}

#pragma warning (disable: 4996)
			_itow(num, buf + lstrlenW(buf), 10);
#pragma warning (default: 4996)

			if (firstPass) {
				firstPass = false;
			} else {
				ret.insert(0, 1, separator);
			}

			ret.insert(0, buf);
			abso = (abso - (abso % 1000)) / 1000;
		} while (blocks--);

		if (number < 0) ret.insert(0, 1, L'-'); // prepend minus signal
		return ret;
	}

	// Converts number to wstring, adding thousand separator.
	static std::wstring to_wstring_with_separator(size_t number, wchar_t separator = L',') {
		return to_wstring_with_separator(static_cast<int>(number), separator);
	}
};

}//namespace wl
