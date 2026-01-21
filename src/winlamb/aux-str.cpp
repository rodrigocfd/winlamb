#include <algorithm>
#include <cwctype>
#include <stdexcept>
#include "aux-str.hpp"
using namespace wl;

size_t WStrView::length() const {
	return lstrlenW(_p);
}

////////////////////////////////////////////////////////////////////////////////

int wl::str::cmp(WStrView a, WStrView b) {
	return lstrcmpW(a.c_str(), b.c_str());
}

int wl::str::cmp_i(WStrView a, WStrView b) {
	return lstrcmpiW(a.c_str(), b.c_str());
}

bool wl::str::contains(WStrView s, WStrView what, size_t off) {
	std::wstring_view sv{s.c_str()};
	return sv.find(what.c_str(), off) != std::wstring::npos;
}

void wl::str::dbg(WStrView format, ...) {
	va_list args = nullptr;
	va_start(args, format);
	int len = std::vswprintf(nullptr, 0, format.c_str(), args); // won't include terminating null

	std::wstring buf(len + 2, L'\0'); // alloc receiving buffer with linebreak
	std::vswprintf(buf.data(), buf.size(), format.c_str(), args);
	va_end(args);
	buf.resize(len);
	buf.push_back(L'\n');
	OutputDebugStringW(buf.c_str());
}

bool wl::str::ends_with(WStrView s, WStrView theEnd) {
	std::wstring_view sv{s.c_str()};
	return sv.ends_with(theEnd.c_str());
}

bool wl::str::ends_with_i(WStrView s, WStrView theEnd) {
	size_t lenEnd = theEnd.length();
	if (!lenEnd) return true;
	if (s.empty() || lenEnd > s.length()) return false;
	return !lstrcmpiW(s.c_str() + s.length() - lenEnd, theEnd.c_str());
}

bool wl::str::eq(WStrView a, WStrView b) {
	return !lstrcmpW(a.c_str(), b.c_str());
}

bool wl::str::eq_i(WStrView a, WStrView b) {
	return !lstrcmpiW(a.c_str(), b.c_str());
}

std::wstring wl::str::fmt(WStrView format, ...) {
	va_list args = nullptr;
	va_start(args, format);
	int len = std::vswprintf(nullptr, 0, format.c_str(), args); // won't include terminating null

	std::wstring buf(len + 1, L'\0'); // alloc receiving buffer
	std::vswprintf(buf.data(), buf.size(), format.c_str(), args);
	va_end(args);
	buf.resize(len);
	return buf;
}

static constexpr size_t ten_pow(BYTE exponent) {
	size_t val = 1;
	for (BYTE i = 0; i < exponent; ++i)
		val *= 10;
	return val;
}

std::wstring wl::str::fmt_bytes(size_t numBytes) {
	constexpr size_t sz = 40;
	if (numBytes < ten_pow(3))
		return fmt(L"%zu bytes", numBytes);
	else if (numBytes < ten_pow(6))
		return fmt(L"%.2f KB", numBytes / static_cast<float>(ten_pow(3)));
	else if (numBytes < ten_pow(9))
		return fmt(L"%.2f MB", numBytes / static_cast<float>(ten_pow(6)));
	else if (numBytes < ten_pow(12))
		return fmt(L"%.2f GB", numBytes / static_cast<float>(ten_pow(9)));
	else if (numBytes < ten_pow(15))
		return fmt(L"%.2f TB", numBytes / static_cast<float>(ten_pow(12)));
	else if (numBytes < ten_pow(18))
		return fmt(L"%.2f PB", numBytes / static_cast<float>(ten_pow(15)));
	else
		return fmt(L"%.2f EB", numBytes / static_cast<float>(ten_pow(18)));
}

std::wstring wl::str::fmt_error(DWORD errorCode) {
	wchar_t *pBuf = nullptr;
	DWORD nChars = FormatMessageW( // without terminating null
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, errorCode, LANG_USER_DEFAULT, reinterpret_cast<LPWSTR>(&pBuf), 0, nullptr);

	if (!nChars) [[unlikely]] {
		std::wstring msg = str::fmt(L"FormatMessage failed with error %d.", GetLastError());
		throw std::runtime_error(str::to_ansi(msg));
	}

	std::wstring finalBuf(pBuf, nChars);
	LocalFree(pBuf);
	str::trim(finalBuf); // because \r\n is placed after the string
	return finalBuf;
}

const wchar_t* wl::str::guess_line_break(WStrView s) {
	for (size_t i = 0; i < s.length() - 1; ++i) {
		if (s[i] == L'\r') {
			return s[i + 1] == L'\n' ? L"\r\n" : L"\r"; // report the first one
		} else if (s[i] == L'\n') {
			return s[i + 1] == L'\r' ? L"\n\r" : L"\n";
		}
	}
	return nullptr; // unknown
}

std::optional<size_t> wl::str::index(WStrView s, WStrView what, size_t off) {
	std::wstring_view sv{s.c_str()};
	size_t pos = sv.find(what.c_str(), off);
	return pos == std::wstring::npos ? std::nullopt : std::make_optional(pos);
}

std::optional<size_t> wl::str::index_rev(WStrView s, WStrView what, size_t off) {
	std::wstring_view sv{s.c_str()};
	size_t pos = sv.rfind(what.c_str(), off);
	return pos == std::wstring::npos ? std::nullopt : std::make_optional(pos);
}

std::wstring wl::str::join(std::span<std::wstring> all, WStrView separator) {
	size_t count = 0;
	size_t lenSep = separator.length();
	bool first = true;

	for (auto &&s : all) {
		if (first) {
			first = false;
		} else {
			count += lenSep;
		}
		count += s.length();
	}

	std::wstring buf;
	buf.reserve(count);
	first = true;

	for (auto &&s : all) {
		if (first) {
			first = false;
		} else {
			buf.append(separator.c_str());
		}
		buf.append(s);
	}
	return buf;
}

static std::wstring parse_ansi(std::span<BYTE> src) {
	std::wstring ret;
	if (!src.empty()) {
		ret.resize(src.size());
		for (size_t i = 0; i < src.size(); ++i) {
			if (src[i] == 0x00) { // found terminating null
				ret.resize(i);
				return ret;
			}
			ret[i] = static_cast<wchar_t>(src[i]); // brute-force conversion
		}
	}
	return ret; // data didn't have a terminating null
}

static std::wstring parse_encoded(std::span<BYTE> src, UINT codePage) {
	std::wstring ret;
	if (!src.empty()) {
		int neededLen = MultiByteToWideChar(codePage, 0,
			reinterpret_cast<const char*>(src.data()), static_cast<int>(src.size()), nullptr, 0);
		ret.resize(neededLen);
		MultiByteToWideChar(codePage, 0, reinterpret_cast<const char*>(src.data()),
			static_cast<int>(src.size()), &ret[0], neededLen);
		wl::str::trim_nulls(ret);
	}
	return ret;
}

static std::wstring parse_utf16(std::span<BYTE> src, bool isLE) {
	std::span<WORD> wsrc{reinterpret_cast<WORD*>(src.data()), src.size() / 2}; // will discard an odd byte
	std::wstring ret;
	ret.reserve(wsrc.size());
	for (WCHAR ch : wsrc)
		ret.push_back(isLE ? ch : MAKEWORD(HIBYTE(ch), LOBYTE(ch)));
	return ret;
}

std::wstring wl::str::parse(std::span<BYTE> src) {
	if (src.empty()) return {};

	Encoding encInfo = Encoding::guess(src);
	src = src.subspan(encInfo.bomSize); // skip BOM, if any

	switch (encInfo.encType) {
	using enum Encoding::Type;
		case unknown:
		case ansi:     return parse_ansi(src);
		case win_1252: return parse_encoded(src, 1252);
		case utf8:     return parse_encoded(src, CP_UTF8);
		case utf16_be: return parse_utf16(src, false);
		case utf16_le: return parse_utf16(src, true);
		case utf32_be: throw std::invalid_argument("UTF-32 big endian: encoding not implemented.");
		case utf32_le: throw std::invalid_argument("UTF-32 little endian: encoding not implemented.");
		case scsu:     throw std::invalid_argument("Standard compression scheme for Unicode: encoding not implemented.");
		case bocu1:    throw std::invalid_argument("Binary ordered compression for Unicode: encoding not implemented.");
		default:       throw std::invalid_argument("Unknown encoding.");
	}
}

std::wstring wl::str::remove_accel_ampersands(WStrView s) {
	size_t len = s.length();
	if (!len)
		return std::wstring{};
	if (len == 1)
		return std::wstring(1, s[0]);

	std::wstring ret{};
	ret.reserve(len);
	for (size_t i = 0; i < len - 1; ++i) {
		if (s[i] == L'&' && s[i + 1] != L'&')
			continue;
		ret.push_back(s[i]);
	}
	if (s[len - 1] != L'&')
		ret.push_back(s[len - 1]);
	return ret;
}

void wl::str::remove_diacritics(std::wstring &s) {
	const wchar_t *diacritics   = L"ÁáÀàÃãÂâÄäÉéÈèÊêËëÍíÌìÎîÏïÓóÒòÕõÔôÖöÚúÙùÛûÜüÇçÅåÐðÑñØøÝýÿ";
	const wchar_t *replacements = L"AaAaAaAaAaEeEeEeEeIiIiIiIiOoOoOoOoOoUuUuUuUuCcAaDdNnOoYyy";

	for (wchar_t &ch : s) {
		const wchar_t *pDiac = diacritics;
		const wchar_t *pRepl = replacements;
		while (*pDiac) {
			if (ch == *pDiac) ch = *pRepl; // in-place replacement
			++pDiac;
			++pRepl;
		}
	}
}

std::vector<std::wstring> wl::str::split(WStrView s, WStrView delimiter) {
	if (s.empty()) return {};

	size_t lenDelim = delimiter.length();
	if (!lenDelim)
		return {std::wstring{s.c_str()}}; // one single element

	std::wstring_view sv{s.c_str()};

	size_t count = 1, base = 0, head = 0;
	for (;;) { // 1st pass counts the occurrences to prealloc; benchmarks proved that this is about 2.7x faster
		head = sv.find(delimiter.c_str(), head);
		if (head == std::wstring::npos) break;
		++count;
		head += lenDelim;
		base = head;
	}

	std::vector<std::wstring> ret;
	ret.reserve(count); // prealloc the number of substrings

	base = head = 0;
	for (;;) { // 2nd pass will append the substrings
		head = sv.find(delimiter.c_str(), head);
		if (head == std::wstring::npos) break;
		ret.emplace_back(); // append empty string to vector
		ret.back().insert(0, sv, base, head - base); // insert chars into last appended string
		head += lenDelim;
		base = head;
	}

	ret.emplace_back();
	ret.back().insert(0, sv, base, s.length() - base); // append the rest
	return ret;
}

std::vector<std::wstring> wl::str::split_lines(WStrView s) {
	return split(s, guess_line_break(s));
}

bool wl::str::starts_with(WStrView s, WStrView theStart) {
	std::wstring_view sv{s.c_str()};
	return sv.starts_with(theStart.c_str());
}

bool wl::str::starts_with_i(WStrView s, WStrView theStart) {
	size_t lenStart = theStart.length();
	if (!lenStart) return true;
	if (s.empty() || lenStart > s.length()) return false;
	std::wstring s2{s.c_str()};
	s2.resize(lenStart);
	return !lstrcmpiW(s2.c_str(), theStart.c_str());
}

std::string wl::str::to_ansi(WStrView s) {
	std::string ansi(s.length(), '\0'); // alloc with same length
	for (size_t i = 0; i < s.length(); ++i) {
		ansi[i] = static_cast<char>(s[i]); // brute-force conversion
	}
	return ansi;
}

std::wstring wl::str::to_lower(WStrView s) {
	std::wstring ret{s.c_str()};
	CharLowerBuffW(ret.data(), static_cast<DWORD>(ret.length()));
	return ret;
}

std::wstring wl::str::to_upper(WStrView s) {
	std::wstring ret{s.c_str()};
	CharUpperBuffW(ret.data(), static_cast<DWORD>(ret.length()));
	return ret;
}

std::vector<BYTE> wl::str::to_utf8_blob(WStrView s, bool writeBom) {
	std::vector<BYTE> buf;
	if (!s.empty()) {
		constexpr BYTE utf8bom[] = {0xef, 0xbb, 0xbf};
		size_t szBom = writeBom ? ARRAYSIZE(utf8bom) : 0; // zero if we won't write the BOM

		size_t neededLen = WideCharToMultiByte(CP_UTF8, 0,
			s.c_str(), static_cast<int>(s.length()),
			nullptr, 0, nullptr, 0);
		buf.resize(neededLen + szBom);

		if (writeBom)
			CopyMemory(buf.data(), utf8bom, szBom);

		WideCharToMultiByte(CP_UTF8, 0,
			s.c_str(), static_cast<int>(s.length()),
			reinterpret_cast<char*>(buf.data() + szBom),
			static_cast<int>(neededLen), nullptr, nullptr);
	}

	return buf;
}

std::wstring wl::str::to_wide(WStrView s) {
	std::wstring wide(s.length(), L'\0');
	for (size_t i = 0; i < s.length(); ++i) {
		wide[i] = s[i]; // brute-force conversion
	}
	return wide;
}

void wl::str::trim_nulls(std::wstring &s) {
	// When a std::wstring is initialized with any length, possibly to be used as a buffer,
	// the string length may not match the size() method, after the operation.
	// This function fixes this.
	if (!s.empty())
		s.resize( lstrlenW(s.c_str()) );
}

void wl::str::trim(std::wstring &s) {
	if (s.empty()) return;
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
		return;
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
}

////////////////////////////////////////////////////////////////////////////////

static constexpr bool guess_utf8(std::span<BYTE> src) {
	std::span<BYTE>::iterator p = src.begin(); // https://stackoverflow.com/a/1031773/6923555
	while (p != src.end() && *p) {
		if ( // ASCII
			// use p[0] <= 0x7f to allow ASCII control characters
			p[0] == 0x09 ||
			p[0] == 0x0a ||
			p[0] == 0x0d ||
			(0x20 <= p[0] && p[0] <= 0x7e)
		) {
			std::advance(p, 1);
			continue;
		}

		if ( // non-overlong 2-byte
			(0xc2 <= p[0] && p[0] <= 0xdf) &&
			(0x80 <= p[1] && p[1] <= 0xbf)
		) {
			std::advance(p, 2);
			continue;
		}

		if (( // excluding overlongs
			p[0] == 0xe0 &&
			(0xa0 <= p[1] && p[1] <= 0xbf) &&
			(0x80 <= p[2] && p[2] <= 0xbf)
		) || ( // straight 3-byte
			((0xe1 <= p[0] && p[0] <= 0xec) ||
				p[0] == 0xee ||
				p[0] == 0xef) &&
			(0x80 <= p[1] && p[1] <= 0xbf) &&
			(0x80 <= p[2] && p[2] <= 0xbf)
		) || ( // excluding surrogates
			p[0] == 0xed &&
			(0x80 <= p[1] && p[1] <= 0x9f) &&
			(0x80 <= p[2] && p[2] <= 0xbf)
		)) {
			std::advance(p, 3);
			continue;
		}

		if (( // planes 1-3
			p[0] == 0xf0 &&
			(0x90 <= p[1] && p[1] <= 0xbf) &&
			(0x80 <= p[2] && p[2] <= 0xbf) &&
			(0x80 <= p[3] && p[3] <= 0xbf)
		) || ( // planes 4-15
			(0xf1 <= p[0] && p[0] <= 0xf3) &&
			(0x80 <= p[1] && p[1] <= 0xbf) &&
			(0x80 <= p[2] && p[2] <= 0xbf) &&
			(0x80 <= p[3] && p[3] <= 0xbf)
		) || ( // plane 16
			p[0] == 0xf4 &&
			(0x80 <= p[1] && p[1] <= 0x8f) &&
			(0x80 <= p[2] && p[2] <= 0xbf) &&
			(0x80 <= p[3] && p[3] <= 0xbf)
		)) {
			std::advance(p, 4);
			continue;
		}

		return false; // none of the conditions were accepted, not UTF-8
	}
	return true; // all the conditions accepted through the whole byte source
}

Encoding Encoding::guess(std::span<BYTE> src) {
	auto match = [&](std::span<BYTE> bom) constexpr -> bool {
		return (src.size() >= bom.size())
			&& std::equal(src.begin(), src.begin() + bom.size(), bom.begin(), bom.end());
	};

	BYTE utf8[] = {0xef, 0xbb, 0xbf}; // UTF-8 BOM
	if (match(utf8)) return {Type::utf8, ARRAYSIZE(utf8)}; // BOM size in bytes

	BYTE utf16be[] = {0xfe, 0xff};
	if (match(utf16be)) return {Type::utf16_be, ARRAYSIZE(utf16be)};

	BYTE utf16le[] = {0xff, 0xfe};
	if (match(utf16le)) return {Type::utf16_le, ARRAYSIZE(utf16le)};

	BYTE utf32be[] = {0x00, 0x00, 0xfe, 0xff};
	if (match(utf32be)) return {Type::utf32_be, ARRAYSIZE(utf32be)};

	BYTE utf32le[] = {0xff, 0xfe, 0x00, 0x00};
	if (match(utf32le)) return {Type::utf32_le, ARRAYSIZE(utf32le)};

	BYTE scsu[] = {0x0e, 0xfe, 0xff};
	if (match(scsu)) return {Type::scsu, ARRAYSIZE(scsu)};

	BYTE bocu1[] = {0xfb, 0xee, 0x28};
	if (match(bocu1)) return {Type::bocu1, ARRAYSIZE(bocu1)};

	if (guess_utf8(src)) return {Type::utf8, 0}; // UTF-8 without BOM

	bool hasNonAnsiChar = std::any_of(src.begin(), src.end(), [](BYTE ch) -> bool { return ch > 0x7f; });
	return hasNonAnsiChar
		? Encoding{Type::win_1252, 0} // by exclusion, not assertive
		: Encoding{Type::ansi, 0};
}
