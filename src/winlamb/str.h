#pragma once
#include <array>
#include <optional>
#include <span>
#include <string>
#include <vector>
#include "lib-include-win.h"

namespace wl {

	/// @brief Wraps a pointer to a null-terminated string.
	///
	/// Analog to [`std::wstring_view`], but null-terminated.
	///
	/// Constructible from:
	/// - `LPCWSTR`
	/// - `const std::wstring&`
	/// - `std::array<WCHAR, N>`
	///
	/// [`std::wstring_view`]: https://en.cppreference.com/w/cpp/string/basic_string_view.html
	class WStrPtr final {
	public:
		/** Constructs `WStrPtr` by wrapping the `LPCWSTR` pointer. */
		constexpr WStrPtr(LPCWSTR p) : _p{p} { }

		/** Constructs `WStrPtr` by wrapping the `wchar_t` pointer. */
		constexpr WStrPtr(const std::wstring &s) : _p{s.c_str()} { }

		/** Constructs `WStrPtr` by wrapping the `wchar_t` pointer from the `std::array`. */
		template<size_t N>
		constexpr WStrPtr(std::array<wchar_t, N> s) : _p{s.data()} { }

		/** Implicit conversion to `LPCWSTR`. */
		constexpr operator LPCWSTR() const { return _p; }

		/** Returns the character at `index`. */
		[[nodiscard]] constexpr wchar_t operator[](size_t index) const { return _p[index]; }

		/** Returns `true` if the wrapped pointer is `nullptr`, of if the string is empty. */
		[[nodiscard]] bool empty() const { return !_p || !length(); }

		/// Calls [`lstrlen`], returning the number of characters in the string, without counting the terminating null.
		///
		/// [`lstrlen`]: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-lstrlenw
		[[nodiscard]] size_t length() const;

		/// Returns the wrapped `LPCWSTR` converted to `LPWSTR` – that is, without the `const`.
		///
		/// This is required by some Win32 structs, which have a `LPWSTR` member.
		[[nodiscard]] constexpr LPWSTR lpwstr() const { return const_cast<LPWSTR>(_p); }

	private:
		LPCWSTR _p;
	};

}

/** @brief UTF-16 wide string utilities. */
namespace wl::str {

	/** Default number of characters of a string allocated with Short String Optimization. */
	constexpr size_t SSO_LEN = std::string{}.capacity();

	/// Calls [`lstrcmp`] to compare the strings lexographically, case-sensitive.
	///
	/// [`lstrcmp`]: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-lstrcmpw
	[[nodiscard]] int cmp(WStrPtr a, WStrPtr b);

	/// Calls [`lstrcmpi`] to compare the strings lexographically, case-insensitive.
	///
	/// [`lstrcmpi`]: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-lstrcmpiw
	[[nodiscard]] int cmp_i(WStrPtr a, WStrPtr b);

	/** Returns true if `s` contains `what`, starting from offset `off`. */
	[[nodiscard]] bool contains(WStrPtr s, WStrPtr what, size_t off = 0);

	/// Calls [`std::vswprintf`] to format the string, then passes it to [`OutputDebugString`].
	///
	/// Example:
	///
	/// ```cpp
	/// std::wstring foo{L"foo"};
	/// wl::str::dbg(L"this is %s", foo.c_str());
	/// ```
	///
	/// [`std::vswprintf`]: https://en.cppreference.com/w/c/io/vfwprintf
	/// [`OutputDebugString`]: https://learn.microsoft.com/en-us/windows/win32/api/debugapi/nf-debugapi-outputdebugstringw
	void dbg(WStrPtr format, ...);

	/** Returns true if `s` ends with `theEnd`, case-sensitive. */
	[[nodiscard]] bool ends_with(WStrPtr s, WStrPtr theEnd);

	/** Returns true if `s` ends with `theEnd`, case-insensitive. */
	[[nodiscard]] bool ends_with_i(WStrPtr s, WStrPtr theEnd);

	/// Calls [`lstrcmp`] for case-sensitive equality.
	///
	/// [`lstrcmp`]: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-lstrcmpw
	[[nodiscard]] bool eq(WStrPtr a, WStrPtr b);

	/// Calls [`lstrcmpi`] for case-insensitive equality.
	///
	/// [`lstrcmpi`]: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-lstrcmpiw
	[[nodiscard]] bool eq_i(WStrPtr a, WStrPtr b);

	/// Calls [`std::vswprintf`] to format the string.
	///
	/// Prefer using C++20's [`std::format`], which is safer.
	///
	/// Example:
	///
	/// ```cpp
	/// std::wstring foo{L"foo"};
	/// std::wstring formatted = wl::str::fmt(L"this is %s", foo.c_str());
	/// ```
	///
	/// [`std::vswprintf`]: https://en.cppreference.com/w/c/io/vfwprintf
	/// [`std::format`]: https://en.cppreference.com/w/cpp/utility/format/format.html
	[[nodiscard]] std::wstring fmt(WStrPtr format, ...);

	/** Converts `numBytes` into a string with the highest unit, up to exabytes., with two decimal places. */
	[[nodiscard]] std::wstring fmt_bytes(size_t numBytes);

	/// Calls [`FormatMessage`] to retrieve the description of a native Win32 error code.
	///
	/// [`FormatMessage`]: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-formatmessagew
	[[nodiscard]] std::wstring fmt_error(DWORD errorCode);

	/** Guesses the linebreak characters: CR, CRLF, LF or LFCR. */
	[[nodiscard]] LPCWSTR guess_line_break(WStrPtr s);

	/// Returns the index of the first occurrence of `what` in `s`, if any.
	///
	/// Starts searching from the offset `off`.
	[[nodiscard]] std::optional<size_t> index(WStrPtr s, WStrPtr what, size_t off = 0);

	/// Returns the index of the last occurrence of `what` in `s`, if any.
	///
	/// Starts searching from the offset `off`.
	[[nodiscard]] std::optional<size_t> index_rev(WStrPtr s, WStrPtr what, size_t off = std::wstring::npos);

	/** Returns a new string by joining the strings in `all` with `separator`. */
	[[nodiscard]] std::wstring join(std::span<std::wstring> all, WStrPtr separator = L"");

	/** Guesses the encoding and parses `src` into a `wstring`. */
	[[nodiscard]] std::wstring parse(std::span<BYTE> src);

	/** Returns a new string removing extra ampersands of keyboard accelerators: `"&He && she"` becomes `"He & she"`. */
	std::wstring remove_accel_ampersands(WStrPtr s);

	/** Removes the diacritics from `s`, in-place. */
	void remove_diacritics(std::wstring &s);

	/** Returns a vector with substrings of `s`, delimited by `delimiter`. */
	[[nodiscard]] std::vector<std::wstring> split(WStrPtr s, WStrPtr delimiter);

	/** Returns a vector with each line of `s` as a string. */
	[[nodiscard]] std::vector<std::wstring> split_lines(WStrPtr s);

	/** Returns true if `s` starts with `theStart`, case-sensitive. */
	[[nodiscard]] bool starts_with(WStrPtr s, WStrPtr theStart);

	/** Returns true if `s` starts with `theStart`, case-insensitive. */
	[[nodiscard]] bool starts_with_i(WStrPtr s, WStrPtr theStart);

	/** Converts `wstring` to `string`. The inverse is done by `to_wide`. */
	[[nodiscard]] std::string to_ansi(WStrPtr s);

	/** Returns a new string, converted to lowercase. */
	[[nodiscard]] std::wstring to_lower(WStrPtr s);

	/** Returns a new string, converted to uppercase. */
	[[nodiscard]] std::wstring to_upper(WStrPtr s);

	/// Converts `s` into UTF-8 bytes by calling [`WideCharToMultiByte`].
	///
	/// [`WideCharToMultiByte`]: https://learn.microsoft.com/en-us/windows/win32/api/stringapiset/nf-stringapiset-widechartomultibyte
	[[nodiscard]] std::vector<BYTE> to_utf8_blob(WStrPtr s, bool writeBom = false);

	/** Converts `string` to `wstring`. The inverse is done by `to_ansi`. */
	[[nodiscard]] std::wstring to_wide(WStrPtr s);

	/// Calls [`std::iswspace`] to remove all spaces from beginning and end of the string.
	///
	/// Also calls `trim_nulls`.
	///
	/// [`std::iswspace`]: https://en.cppreference.com/w/cpp/string/wide/iswspace.html
	void trim(std::wstring &s);

	/// Calls [`lstrlen`] and resizes `s`, so that the return of `size()` will match
	/// the actual string length, not counting any terminating nulls.
	///
	/// [`lstrlen`]: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-lstrlenw
	void trim_nulls(std::wstring &s);

}

namespace wl {

	/** @brief Recognized character encodings. */
	enum class Encoding { unknown, ansi, win_1252, utf8, utf16_be, utf16_le, utf32_be, utf32_le, scsu, bocu1 };

	/** @brief Caracter encoding information. */
	struct EncodingInfo final {
		Encoding encType = Encoding::unknown;
		BYTE bomSize = 0;

		/// Static method.
		///
		/// Guesses the encoding of the given binary blob.
		[[nodiscard]] static EncodingInfo guess(std::span<BYTE> src);
	};

}
