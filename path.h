/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "str.h"

namespace wl {

// Utilities to file path operations with std::wstring.
class path final {
private:
	path() = delete;

public:
	static std::wstring& trim_backslash(std::wstring& filePath) {
		while (filePath.back() == L'\\') {
			filePath.resize(filePath.length() - 1);
		}
		return filePath;
	}

	static bool is_same(const std::wstring& filePath, const wchar_t* other) noexcept {
		return !lstrcmpiW(filePath.c_str(), other);
	}

	static bool is_same(const std::wstring& filePath, const std::wstring& other) noexcept {
		return !lstrcmpiW(filePath.c_str(), other.c_str());
	}

	static bool has_extension(const std::wstring& filePath, const wchar_t* extension) noexcept {
		if (extension[0] == L'.') { // extension starts with dot, compare right away
			return str::ends_withi(filePath, extension);
		}

		wchar_t dotExtension[32] = L"."; // arbitrary buffer length
		lstrcatW(dotExtension, extension);
		return str::ends_withi(filePath, dotExtension);
	}

	static bool has_extension(const std::wstring& filePath, const std::wstring& extension) noexcept {
		return has_extension(filePath, extension.c_str());
	}

	static bool has_extension(const std::wstring& filePath, std::initializer_list<const wchar_t*> extensions) noexcept {
		for (const wchar_t* ext : extensions) {
			if (has_extension(filePath, ext)) {
				return true;
			}
		}
		return false;
	}

	static std::wstring& change_extension(std::wstring& filePath, const wchar_t* newExtension) {
		size_t dotIdx = filePath.find_last_of(L'.');
		if (dotIdx != std::wstring::npos) { // filePath already have an extension
			filePath.resize(dotIdx + 1); // truncate after the dot
		} else { // filePath doesn't have an extension
			filePath.append(1, L'.');
		}
		filePath.append(newExtension[0] == L'.' ? newExtension + 1 : newExtension);
		return filePath;
	}

	static std::wstring& change_extension(std::wstring& filePath, const std::wstring& newExtension) {
		return change_extension(filePath, newExtension.c_str());
	}

	static std::wstring folder_from(const std::wstring& filePath) {
		std::wstring ret = filePath;
		size_t found = ret.find_last_of(L'\\'); // won't include trailing backslash
		if (found != std::wstring::npos) {
			ret.resize(found);
		}
		return ret;
	}

	static std::wstring file_from(const std::wstring& filePath) {
		std::wstring ret = filePath;
		size_t found = ret.find_last_of(L'\\');
		if (found != std::wstring::npos) {
			ret.erase(0, found + 1);
		}
		return ret;
	}
};

}//namespace wl
