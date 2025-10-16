#pragma once
#include <optional>
#include <span>
#include <string>
#include <vector>
#include "lib-include-win.h"

namespace wl {

	// Holds a pointer to a null-terminated string, being constructible from LPCWSTR or const std::wstring&.
	// Analog to std::wstring_view, but null-terminated.
	class WStrPtr final {
	public:
		constexpr WStrPtr() = default;
		constexpr WStrPtr(const WStrPtr&) = default;
		constexpr WStrPtr(WStrPtr&&) = default;
		constexpr WStrPtr& operator=(const WStrPtr&) = default;
		constexpr WStrPtr& operator=(WStrPtr&&) = default;

		constexpr WStrPtr(LPCWSTR p) : _p{p} { }
		constexpr WStrPtr(const std::wstring &s) : _p{s.c_str()} { }

		constexpr operator LPCWSTR() const { return _p; }
		[[nodiscard]] constexpr WCHAR operator[](size_t index) const { return _p[index]; }
		[[nodiscard]] constexpr LPWSTR lpwstr() const { return const_cast<LPWSTR>(_p); }

		[[nodiscard]] bool empty() const { return !_p || !length(); }
		[[nodiscard]] size_t length() const;

	private:
		LPCWSTR _p = nullptr;
	};

}

// String utilities.
namespace wl::str {

	// Default number of characters of a string allocated with Short String Optimization.
	constexpr size_t SSO_LEN = std::string{}.capacity();

	// Calls lstrcmp() to compare the strings lexographically, case-sensitive.
	[[nodiscard]] int cmp(WStrPtr a, WStrPtr b);

	// Calls lstrcmpi() to compare the strings lexographically, case-insensitive.
	[[nodiscard]] int cmp_i(WStrPtr a, WStrPtr b);

	// Returns true if s contains what, starting from offset off.
	[[nodiscard]] bool contains(WStrPtr s, WStrPtr what, size_t off = 0);

	// Returns true if s ends with theEnd, case-sensitive.
	[[nodiscard]] bool ends_with(WStrPtr s, WStrPtr theEnd);

	// Returns true if s ends with theEnd, case-insensitive.
	[[nodiscard]] bool ends_with_i(WStrPtr s, WStrPtr theEnd);

	// Calls lstrcmp() for case-sensitive equality.
	[[nodiscard]] bool eq(WStrPtr a, WStrPtr b);

	// Calls lstrcmpi() for case-insensitive equality.
	[[nodiscard]] bool eq_i(WStrPtr a, WStrPtr b);

	// Calls std::vswprintf().
	// Prefer using C++20's std::format(), which is safer.
	[[nodiscard]] std::wstring fmt(WStrPtr format, ...);

	// Converts numBytes into a string with the highest unit, up to exabytes. Two decimal places.
	[[nodiscard]] std::wstring fmt_bytes(size_t numBytes);

	// Guesses the linebreak characters: CR, CRLF, LF or LFCR.
	[[nodiscard]] LPCWSTR guess_line_break(WStrPtr s);

	// Returns the index of the first occurrence of what in s, if any.
	// Starts searching from the offset off.
	[[nodiscard]] std::optional<size_t> index(WStrPtr s, WStrPtr what, size_t off = 0);

	// Returns the index of the last occurrence of what in s, if any.
	// Starts searching from the offset off.
	[[nodiscard]] std::optional<size_t> index_rev(WStrPtr s, WStrPtr what, size_t off = std::wstring::npos);

	// Returns a new string by joining the strings in all with separator.
	[[nodiscard]] std::wstring join(std::span<std::wstring> all, WStrPtr separator = L"");

	// Returns a new wstring with numReserve reserved chars.
	[[nodiscard]] std::wstring new_reserved(size_t numReserve);

	// Returns a new wstring resized with numResize occurrences of ch.
	[[nodiscard]] std::wstring new_resized(size_t numResize, WCHAR ch = L'\0');

	// Guesses the encoding and parses src into a wstring.
	[[nodiscard]] std::wstring parse(std::span<BYTE> src);

	// Removes the diacritics from s, in-place.
	void remove_diacritics(std::wstring &s);

	// Returns a vector with substrings of s, delimited by delimiter.
	[[nodiscard]] std::vector<std::wstring> split(WStrPtr s, WStrPtr delimiter);

	// Returns a vector with each line of s as a string.
	[[nodiscard]] std::vector<std::wstring> split_lines(WStrPtr s);

	// Returns true if s starts with theStart, case-sensitive.
	[[nodiscard]] bool starts_with(WStrPtr s, WStrPtr theStart);

	// Returns true if s starts with theStart, case-insensitive.
	[[nodiscard]] bool starts_with_i(WStrPtr s, WStrPtr theStart);

	// Converts wstring to string. The inverse is done by to_wide().
	[[nodiscard]] std::string to_ansi(WStrPtr s);

	// Returns a new string, converted to lowercase.
	[[nodiscard]] std::wstring to_lower(WStrPtr s);

	// Returns a new string, converted to uppercase.
	[[nodiscard]] std::wstring to_upper(WStrPtr s);

	// Converts s into UTF-8 bytes with WideCharToMultiByte().
	[[nodiscard]] std::vector<BYTE> to_utf8_blob(WStrPtr s, bool writeBom = false);

	// Converts string to wstring. The inverse is done by to_ansi().
	[[nodiscard]] std::wstring to_wide(WStrPtr s);

	// Calls iswspace() to remove all spaces from beginning and end of the string.
	// Also calls trim_nulls().
	void trim(std::wstring &s);

	// Calls lstrlen() and resizes the wstring, so that its size() will match
	// the actual string length, not counting any terminating nulls.
	void trim_nulls(std::wstring &s);

}

// Encoding-related utilities.
namespace wl::str::enc {

	// Type of character encoding.
	enum class Type { Unknown, Ansi, Win1252, Utf8, Utf16be, Utf16le, Utf32be, Utf32le, Scsu, Bocu1 };

	// Character encoding information.
	struct Info final {
		Type encType = Type::Unknown;
		BYTE bomSize = 0;
	};

	// Guesses the encoding of the given binary blob.
	[[nodiscard]] Info guess(std::span<BYTE> src);

}
