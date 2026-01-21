#include <system_error>
#include "aux-path.hpp"

std::wstring wl::path::dir_from(WStrView p) {
	std::wstring ret{p.c_str()};
	size_t found = ret.find_last_of(L'\\'); // won't include trailing backslash
	if (found != std::wstring::npos)
		ret.resize(found);
	return ret;
}

void wl::path::dir_list(WStrView dirPath, std::function<void(const std::wstring &p)> &&cb) {
	if (!is_dir(dirPath)) [[unlikely]] {
		throw std::invalid_argument{"Not a directory: " + str::to_ansi(dirPath) + "."};
	}

	std::wstring userPath{};
	userPath.reserve(dirPath.length() + 2);
	userPath = dirPath.c_str();
	if (userPath.back() != L'\\') userPath.push_back(L'\\');
	std::wstring basePath{userPath}; // to concat the found file names
	userPath.push_back(L'*');

	struct Finder final {
		~Finder() { if (obj && obj != INVALID_HANDLE_VALUE) FindClose(obj); } // make sure the handle will be closed
		HANDLE obj = nullptr;
	} hFind{};

	WIN32_FIND_DATAW wfd{};
	hFind.obj = FindFirstFileW(userPath.c_str(), &wfd);
	if (hFind.obj == INVALID_HANDLE_VALUE) {
		DWORD err = GetLastError();
		if (err == ERROR_FILE_NOT_FOUND) [[likely]] {
			return; // no files found
		} else [[unlikely]] {
			throw std::system_error(err, std::system_category(), "FindFirstFile failed");
		}
	}

	std::wstring retFileBuf{};
	for (;;) {
		if (!str::eq(wfd.cFileName, L".") && !str::eq(wfd.cFileName, L"..")) { // skip these
			retFileBuf = basePath;
			retFileBuf.append(wfd.cFileName);
			cb(retFileBuf); // invoke user callback
		}

		if (!FindNextFileW(hFind.obj, &wfd)) {
			DWORD err = GetLastError();
			if (err == ERROR_NO_MORE_FILES) [[likely]] {
				return; // no more files found
			} else [[unlikely]] {
				throw std::system_error(err, std::system_category(), "FindNextFile failed");
			}
		}
	}
}

std::vector<std::wstring> wl::path::dir_list(WStrView dirPath) {
	std::vector<std::wstring> ret{};
	dir_list(dirPath, [&ret](const std::wstring &p) -> void {
		ret.emplace_back(p);
	});
	return ret;
}

void wl::path::dir_walk(WStrView dirPath, std::function<void(const std::wstring &p)> &&cb) {
	dir_list(dirPath, [&cb](const std::wstring &p) -> void {
		if (is_dir(p)) {
			dir_walk(p, [&cb](const std::wstring &p) -> void { // recursively
				cb(p); // invoke user callback
			});
		} else {
			cb(p); // invoke user callback
		}
	});
}

std::vector<std::wstring> wl::path::dir_walk(WStrView dirPath) {
	std::vector<std::wstring> ret{};
	dir_walk(dirPath, [&ret](const std::wstring &p) -> void {
		ret.emplace_back(p);
	});
	return ret;
}

std::wstring wl::path::exe_dir() {
	wchar_t buf[MAX_PATH] = {L'\0'};
	GetModuleFileNameW(nullptr, buf, ARRAYSIZE(buf));
	std::wstring p = dir_from(buf);
#ifdef _DEBUG
	p = dir_from(p); // in debug mode, go up another dir level
#endif
	return p;
}

bool wl::path::exists(WStrView p) {
	DWORD attr = GetFileAttributesW(p.c_str());
	return attr != INVALID_FILE_ATTRIBUTES;
}

std::wstring wl::path::file_from(WStrView p) {
	std::wstring ret{p.c_str()};
	size_t found = ret.find_last_of(L'\\');
	if (found != std::wstring::npos)
		ret.erase(0, found + 1);
	return ret;
}

bool wl::path::has_extension(const std::wstring &p, WStrView ext) {
	return str::ends_with_i(p, ext);
}

bool wl::path::has_extension(const std::wstring &p, std::initializer_list<WStrView> exts) {
	for (auto &&ext : exts) {
		if (has_extension(p, ext))
			return true;
	}
	return false;
}

static DWORD get_attrs(wl::WStrView p) {
	DWORD attr = GetFileAttributesW(p.c_str());
	if (attr == INVALID_FILE_ATTRIBUTES) [[unlikely]] {
		throw std::system_error(GetLastError(), std::system_category(), "GetFileAttributes failed");
	}
	return attr;
}

bool wl::path::is_dir(WStrView p) {
	return get_attrs(p) & FILE_ATTRIBUTE_DIRECTORY;
}

bool wl::path::is_hidden(WStrView p) {
	return get_attrs(p) & FILE_ATTRIBUTE_HIDDEN;
}

bool wl::path::is_read_only(WStrView p) {
	return get_attrs(p) & FILE_ATTRIBUTE_READONLY;
}

std::wstring wl::path::swap_extension(const std::wstring &p, WStrView newExt) {
	size_t idxDot = p.find_last_of(L'.');
	if (idxDot != std::wstring::npos) [[likely]] {
		std::wstring p2{p.substr(0, idxDot + (newExt[0] == L'.' ? 0 : 1))};
		p2.append(newExt.c_str());
		return p2;
	} else [[unlikely]] {
		throw std::logic_error{"File has no extension: " + str::to_ansi(p) + "."};
	}
}

void wl::path::trim_backslash(std::wstring &p) {
	while (p.back() == L'\\')
		p.resize(p.length() - 1);
}
