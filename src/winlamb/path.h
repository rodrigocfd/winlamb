#pragma once
#include "str.h"

// Path utilities.
namespace wl::path {

	// Returns the full directory of p, without the trailing backslash.
	[[nodiscard]] std::wstring dir_from(WStrPtr p);

	// Returns all files and folders within pathAndFilter, like "C:\\Temp\\*.mp3" or "C:\\Temp\\*".
	[[nodiscard]] std::vector<std::wstring> dir_list(WStrPtr pathAndFilter);

	// Returns, recursively on folders, all files within pathAndFilter, like "C:\\Temp\\*.mp3" or "C:\\Temp\\*".
	[[nodiscard]] std::vector<std::wstring> dir_walk(WStrPtr pathAndFilter);

	// Returns the path of the current executable. In debug mode, goes up another level, returning the project path.
	[[nodiscard]] std::wstring exe_dir();

	// Returns true if the folder or file exists.
	[[nodiscard]] bool exists(WStrPtr p);

	// Returns the file name from p.
	[[nodiscard]] std::wstring file_from(WStrPtr p);

	// Returns true if the p has the ext extension, case-insensitive.
	[[nodiscard]] bool has_extension(const std::wstring& p, WStrPtr ext);

	// Returns true if the p has one of the ext extensions, case-insensitive.
	[[nodiscard]] bool has_extension(const std::wstring& p, std::initializer_list<WStrPtr> exts);

	// Returns true if p is a directory.
	[[nodiscard]] bool is_dir(WStrPtr p);

	// Returns true if p is hidden.
	[[nodiscard]] bool is_hidden(WStrPtr p);

	// Returns true if p is read-only.
	[[nodiscard]] bool is_read_only(WStrPtr p);

	// Swaps the extension of p by newExt.
	[[nodiscard]] std::wstring swap_extension(const std::wstring& p, WStrPtr newExt);

	// Trims any trailing backslashes, if any, in-place.
	void trim_backslash(std::wstring& p);

}
