#pragma once
#include <optional>
#include <span>
#include <string>
#include <vector>

#include <sdkddkver.h>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace wl {

	/// @brief Wraps a pointer to a null-terminated string.
	///
	/// Analog to [`std::wstring_view`], but null-terminated.
	///
	/// Constructible from:
	/// - `const wchar_t*`
	/// - `const std::wstring&`
	///
	/// [`std::wstring_view`]: https://en.cppreference.com/w/cpp/string/basic_string_view.html
	class WStrView final {
	public:
		/** @brief Iterator. */
		class Iterator final {
		public:
			constexpr explicit Iterator(const wchar_t *p) : _p{p} { }
			constexpr const wchar_t& operator*() const { return *_p; }
			constexpr bool operator!=(const Iterator &other) const { return _p != other._p; }

			constexpr Iterator& operator++() {
				++_p;
				return *this;
			}

		private:
			const wchar_t *_p;
		};

		/** Constructs `WStrView` by wrapping the `const wchar_t` pointer. */
		constexpr WStrView(const wchar_t *p) : _p{p} { }

		/** Constructs `WStrView` by wrapping the `const wchar_t` pointer from the `std::wstring`. */
		constexpr WStrView(const std::wstring &s) : _p{s.c_str()} { }

		/** Returns the wrapped null terminated wide string pointer. */
		[[nodiscard]] constexpr const wchar_t* c_str() const { return _p; }

		/** Returns the character at `index`. */
		[[nodiscard]] constexpr wchar_t operator[](size_t index) const { return _p[index]; }

		/** Returns `true` if the wrapped pointer is `nullptr`, of if the string is empty. */
		[[nodiscard]] bool empty() const { return !_p || !length(); }

		/// Calls [`lstrlen`], returning the number of characters in the string,
		/// without counting the terminating null.
		///
		/// [`lstrlen`]: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-lstrlenw
		[[nodiscard]] size_t length() const;

		/** Returns an iterator to the first character. */
		[[nodiscard]] constexpr Iterator begin() const { return Iterator{_p}; }

		/** Returns an iterator past the last character. */
		[[nodiscard]] Iterator end() const { return Iterator{_p + length()}; }

	private:
		const wchar_t *_p;
	};

}

/// @brief [UTF-16] wide string utilities.
///
/// [UTF-16]: https://learn.microsoft.com/en-us/windows/win32/intl/unicode-in-the-windows-api
namespace wl::str {

	/** Default number of characters of a string allocated with Short String Optimization. */
	constexpr size_t SSO_LEN = std::wstring{}.capacity();

	/// Calls [`lstrcmp`] to compare the strings lexographically, case-sensitive.
	///
	/// [`lstrcmp`]: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-lstrcmpw
	[[nodiscard]] int cmp(WStrView a, WStrView b);

	/// Calls [`lstrcmpi`] to compare the strings lexographically,
	/// case-insensitive.
	///
	/// [`lstrcmpi`]: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-lstrcmpiw
	[[nodiscard]] int cmp_i(WStrView a, WStrView b);

	/** Returns true if `s` contains `what`, starting from offset `off`. */
	[[nodiscard]] bool contains(WStrView s, WStrView what, size_t off = 0);

	/// Calls [`std::vswprintf`] to format the string, then passes it to
	/// [`OutputDebugString`].
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
	void dbg(WStrView format, ...);

	/** Returns true if `s` ends with `theEnd`, case-sensitive. */
	[[nodiscard]] bool ends_with(WStrView s, WStrView theEnd);

	/** Returns true if `s` ends with `theEnd`, case-insensitive. */
	[[nodiscard]] bool ends_with_i(WStrView s, WStrView theEnd);

	/// Calls [`lstrcmp`] for case-sensitive equality.
	///
	/// [`lstrcmp`]: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-lstrcmpw
	[[nodiscard]] bool eq(WStrView a, WStrView b);

	/// Calls [`lstrcmpi`] for case-insensitive equality.
	///
	/// [`lstrcmpi`]: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-lstrcmpiw
	[[nodiscard]] bool eq_i(WStrView a, WStrView b);

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
	[[nodiscard]] std::wstring fmt(WStrView format, ...);

	/** Converts `numBytes` into a string with the highest unit, up to exabytes., with two decimal places. */
	[[nodiscard]] std::wstring fmt_bytes(size_t numBytes);

	/// Calls [`FormatMessage`] to retrieve the description of a native Win32
	/// error code.
	///
	/// [`FormatMessage`]: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-formatmessagew
	[[nodiscard]] std::wstring fmt_error(DWORD errorCode);

	/** Guesses the linebreak characters: CR, CRLF, LF or LFCR. */
	[[nodiscard]] const wchar_t* guess_line_break(WStrView s);

	/// Returns the index of the first occurrence of `what` in `s`, if any.
	///
	/// Starts searching from the offset `off`.
	[[nodiscard]] std::optional<size_t> index(WStrView s, WStrView what, size_t off = 0);

	/// Returns the index of the last occurrence of `what` in `s`, if any.
	///
	/// Starts searching from the offset `off`.
	[[nodiscard]] std::optional<size_t> index_rev(WStrView s, WStrView what, size_t off = std::wstring::npos);

	/** Returns a new string by joining the strings in `all` with `separator`. */
	[[nodiscard]] std::wstring join(std::span<std::wstring> all, WStrView separator = L"");

	/** Guesses the `Encoding` and parses `src` into a `wstring`. */
	[[nodiscard]] std::wstring parse(std::span<BYTE> src);

	/// Returns a new string removing extra ampersands of keyboard accelerators:
	/// `"&He && she"` becomes `"He & she"`.
	std::wstring remove_accel_ampersands(WStrView s);

	/** Removes the diacritics from `s`, in-place. */
	void remove_diacritics(std::wstring &s);

	/// Returns a [`std::vector`] with substrings of `s`, delimited by
	/// `delimiter`.
	///
	/// [`std::vector`]: https://en.cppreference.com/w/cpp/container/vector.html
	[[nodiscard]] std::vector<std::wstring> split(WStrView s, WStrView delimiter);

	/// Returns a [`std::vector`] with at most `maxSubstrs` substrings of `s`,
	/// delimited by `delimiter`.
	///
	/// [`std::vector`]: https://en.cppreference.com/w/cpp/container/vector.html
	[[nodiscard]] std::vector<std::wstring> split_n(WStrView s, WStrView delimiter, size_t maxSubstrs);

	/// Guesses the linebreak with `guess_line_break` and returns a
	/// [`std::vector`] with each line of `s` as a string.
	///
	/// [`std::vector`]: https://en.cppreference.com/w/cpp/container/vector.html
	[[nodiscard]] std::vector<std::wstring> split_lines(WStrView s);

	/** Returns true if `s` starts with `theStart`, case-sensitive. */
	[[nodiscard]] bool starts_with(WStrView s, WStrView theStart);

	/** Returns true if `s` starts with `theStart`, case-insensitive. */
	[[nodiscard]] bool starts_with_i(WStrView s, WStrView theStart);

	/** Converts `wstring` to `string`. The inverse is done by `to_wide`. */
	[[nodiscard]] std::string to_ansi(WStrView s);

	/** Returns a new `std::wstring` converted to lowercase. */
	[[nodiscard]] std::wstring to_lower(WStrView s);

	/** Returns a new string, converted to uppercase. */
	[[nodiscard]] std::wstring to_upper(WStrView s);

	/// Converts `s` into a [`std::vector`] of UTF-8 bytes by calling
	/// [`WideCharToMultiByte`].
	///
	/// [`std::vector`]: https://en.cppreference.com/w/cpp/container/vector.html
	/// [`WideCharToMultiByte`]: https://learn.microsoft.com/en-us/windows/win32/api/stringapiset/nf-stringapiset-widechartomultibyte
	[[nodiscard]] std::vector<BYTE> to_utf8_blob(WStrView s, bool writeBom = false);

	/** Converts `string` to `wstring`. The inverse is done by `to_ansi`. */
	[[nodiscard]] std::wstring to_wide(const char *s);

	/// Calls [`std::iswspace`] to remove all spaces from beginning and end of
	/// the string.
	///
	/// Also calls `trim_nulls`.
	///
	/// [`std::iswspace`]: https://en.cppreference.com/w/cpp/string/wide/iswspace.html
	void trim(std::wstring &s);

	/// Calls [`lstrlen`] and resizes `s`, so that the return of `size()` will
	/// match the actual string length, not counting any terminating nulls.
	///
	/// [`lstrlen`]: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-lstrlenw
	void trim_nulls(std::wstring &s);

}

namespace wl {

	/** @brief Character encoding information. */
	struct Encoding final {
		/** @brief Recognized character encodings. */
		enum class Type { unknown, ansi, win_1252, utf8, utf16_be, utf16_le, utf32_be, utf32_le, scsu, bocu1 };

		Type encType = Type::unknown;
		BYTE bomSize = 0;

		/// Static method; guesses the encoding of the given binary blob.
		///
		/// You usually don't need to call this method directly, since
		/// `wl::str::parse` already calls it.
		[[nodiscard]] static Encoding guess(std::span<BYTE> src);
	};

}
