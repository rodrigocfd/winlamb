#include <algorithm>
#include <system_error>
#include "path.h"

std::wstring wl::path::dir_from(WStrPtr p) {
	std::wstring ret{p};
	size_t found = ret.find_last_of(L'\\'); // won't include trailing backslash
	if (found != std::wstring::npos)
		ret.resize(found);
	return ret;
}

std::vector<std::wstring> wl::path::dir_list(WStrPtr pathAndFilter) {
	WIN32_FIND_DATAW wfd{};
	HANDLE hFind = FindFirstFileW(pathAndFilter, &wfd);
	if (hFind == INVALID_HANDLE_VALUE) {
		DWORD err = GetLastError();
		FindClose(hFind);
		if (err == ERROR_FILE_NOT_FOUND) [[likely]] {
			return {}; // no files found
		} else [[unlikely]] {
			throw std::system_error(err, std::system_category(), "FindFirstFile failed.");
		}
	}

	std::vector<std::wstring> entries;
	for (;;) {
		if (!str::eq(wfd.cFileName, L".") && !str::eq(wfd.cFileName, L"..")) { // skip these
			std::wstring strPathAndFilter{pathAndFilter};
			size_t idxBackslash = strPathAndFilter.find_last_of(L"\\");
			if (idxBackslash != std::wstring::npos) [[likely]] {
				std::wstring fullPath{strPathAndFilter.substr(0, idxBackslash + 1)};
				fullPath.append(wfd.cFileName);
				entries.emplace_back(std::move(fullPath));
			} else [[unlikely]] {
				std::wstring faultyPath{wfd.cFileName};
				throw std::logic_error("No backslash in found file " + str::to_ansi(faultyPath) + ".");
			}
		}

		if (!FindNextFileW(hFind, &wfd)) {
			DWORD err = GetLastError();
			FindClose(hFind);
			if (err == ERROR_NO_MORE_FILES) [[likely]] {
				std::sort(entries.begin(), entries.end(), [](const std::wstring& a, const std::wstring& b) -> bool {
					return lstrcmpiW(a.c_str(), b.c_str()) < 1;
				});
				return entries; // no more files found
			} else [[unlikely]] {
				throw std::system_error(err, std::system_category(), "FindNextFile failed.");
			}
		}
	}
}

void dir_walk_buf(wl::WStrPtr pathAndFilter, std::vector<std::wstring>& outBuf) {
	std::vector<std::wstring> entries = wl::path::dir_list(pathAndFilter);
	for (const auto& entry : entries) {
		if (!wl::path::is_dir(entry))
			outBuf.push_back(entry);
	}
	for (const auto& entry : entries) {
		if (wl::path::is_dir(entry)) {
			std::wstring subPath{entry};
			subPath.append(L"\\*");
			dir_walk_buf(subPath, outBuf); // recursively, deep last
		}
	}
}

std::vector<std::wstring> wl::path::dir_walk(WStrPtr pathAndFilter) {
	std::vector<std::wstring> entries;
	dir_walk_buf(pathAndFilter, entries);
	return entries;
}

std::wstring wl::path::exe_dir() {
	WCHAR buf[MAX_PATH] = {L'\0'};
	GetModuleFileNameW(nullptr, buf, ARRAYSIZE(buf));
	std::wstring p = dir_from(buf);
#ifdef _DEBUG
	p = dir_from(p); // in debug mode, go up another dir level
#endif
	return p;
}

bool wl::path::exists(WStrPtr p) {
	DWORD attr = GetFileAttributesW(p);
	return attr != INVALID_FILE_ATTRIBUTES;
}

std::wstring wl::path::file_from(WStrPtr p) {
	std::wstring ret{p};
	size_t found = ret.find_last_of(L'\\');
	if (found != std::wstring::npos)
		ret.erase(0, found + 1);
	return ret;
}

bool wl::path::has_extension(const std::wstring& p, WStrPtr ext) {
	return str::ends_with_i(p, ext);
}

bool wl::path::has_extension(const std::wstring& p, std::initializer_list<WStrPtr> exts) {
	for (auto&& ext : exts) {
		if (has_extension(p, ext))
			return true;
	}
	return false;
}

static DWORD get_attrs(wl::WStrPtr p) {
	DWORD attr = GetFileAttributesW(p);
	if (attr == INVALID_FILE_ATTRIBUTES) [[unlikely]] {
		throw std::system_error(GetLastError(), std::system_category(), "GetFileAttributes failed.");
	}
	return attr;
}

bool wl::path::is_dir(WStrPtr p) {
	return get_attrs(p) & FILE_ATTRIBUTE_DIRECTORY;
}

bool wl::path::is_hidden(WStrPtr p) {
	return get_attrs(p) & FILE_ATTRIBUTE_HIDDEN;
}

bool wl::path::is_read_only(WStrPtr p) {
	return get_attrs(p) & FILE_ATTRIBUTE_READONLY;
}

std::wstring wl::path::swap_extension(const std::wstring& p, WStrPtr newExt) {
	size_t idxDot = p.find_last_of(L'.');
	if (idxDot != std::wstring::npos) [[likely]] {
		std::wstring p2{p.substr(0, idxDot + (newExt[0] == L'.' ? 0 : 1))};
		p2.append(newExt);
		return p2;
	} else [[unlikely]] {
		throw std::logic_error("File has no extension: " + str::to_ansi(p) + ".");
	}
}

void wl::path::trim_backslash(std::wstring& p) {
	while (p.back() == L'\\')
		p.resize(p.length() - 1);
}
