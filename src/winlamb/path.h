#pragma once
#include <functional>
#include <vector>
#include "lib-include-win.h"
#include "str.h"

/** @brief Filepath utilities. */
namespace wl::path {

	/** Returns the full directory of `p`, without the trailing backslash. */
	[[nodiscard]] std::wstring dir_from(WStrPtr p);

	/// Invokes `cb` for each file and folder within `dirPath`.
	///
	/// Does not search the folders recursively.
	///
	/// Example:
	///
	/// ```cpp
	/// wl::path::dir_list(L"C:\\Temp", [](const std::wstring &p) -> void {
	///     // ...
	/// });
	/// ```
	void dir_list(WStrPtr dirPath, std::function<void(const std::wstring &p)> &&cb);

	/// Returns a newly allocated vector with each file and folder within `dirPath`.
	///
	/// Does not search the folders recursively.
	[[nodiscard]] std::vector<std::wstring> dir_list(WStrPtr dirPath);

	/// Invokes `cb` for each file within `dirPath`, searching the folders recursively.
	///
	/// Example:
	///
	/// ```cpp
	/// wl::path::dir_walk(L"C:\\Temp", [](const std::wstring &p) -> void {
	///     // ...
	/// });
	/// ```
	void dir_walk(WStrPtr dirPath, std::function<void(const std::wstring &p)> &&cb);

	/** Returns a newly allocated vector with each file within `dirPath`, searching folders recursively. */
	[[nodiscard]] std::vector<std::wstring> dir_walk(WStrPtr dirPath);

	/// Returns the path of the current executable.
	///
	/// In debug mode, goes up another level, returning the root project path.
	[[nodiscard]] std::wstring exe_dir();

	/** Returns true if the folder or file `p` exists. */
	[[nodiscard]] bool exists(WStrPtr p);

	/** Returns the file name from `p`. */
	[[nodiscard]] std::wstring file_from(WStrPtr p);

	/** Returns true if the `p` has the `ext` extension, case-insensitive. */
	[[nodiscard]] bool has_extension(const std::wstring &p, WStrPtr ext);

	/** Returns true if the `p` has one of the `ext` extensions, case-insensitive. */
	[[nodiscard]] bool has_extension(const std::wstring &p, std::initializer_list<WStrPtr> exts);

	/** Returns true if `p` is a directory. */
	[[nodiscard]] bool is_dir(WStrPtr p);

	/** Returns true if `p` is hidden. */
	[[nodiscard]] bool is_hidden(WStrPtr p);

	/** Returns true if `p` is read-only. */
	[[nodiscard]] bool is_read_only(WStrPtr p);

	/** Returns a new string swapping the extension of `p` by `newExt`. */
	[[nodiscard]] std::wstring swap_extension(const std::wstring &p, WStrPtr newExt);

	/** Trims any trailing backslashes, if any, in-place. */
	void trim_backslash(std::wstring &p);

}
