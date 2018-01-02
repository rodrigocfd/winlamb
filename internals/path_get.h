/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "../com.h"
#include "../str.h"
#include <ShlObj.h>

namespace wl {
namespace wli {

struct path_get final {
private:
	path_get() = delete;

public:
	static std::wstring my_documents()      { return _get_shell_folder(CSIDL_MYDOCUMENTS); }
	static std::wstring my_music()          { return _get_shell_folder(CSIDL_MYMUSIC); }
	static std::wstring my_pictures()       { return _get_shell_folder(CSIDL_MYPICTURES); }
	static std::wstring my_videos()         { return _get_shell_folder(CSIDL_MYVIDEO); }
	static std::wstring desktop()           { return _get_shell_folder(CSIDL_DESKTOPDIRECTORY); }
	static std::wstring app_data()          { return _get_shell_folder(CSIDL_APPDATA); }
	static std::wstring app_data_local()    { return _get_shell_folder(CSIDL_LOCAL_APPDATA); }
	static std::wstring app_data_common()   { return _get_shell_folder(CSIDL_COMMON_APPDATA); }
	static std::wstring program_files()     { return _get_shell_folder(CSIDL_PROGRAM_FILES); }
	static std::wstring program_files_x86() { return _get_shell_folder(CSIDL_PROGRAM_FILESX86); }

	static std::wstring temp() {
		wchar_t buf[MAX_PATH + 1]{};
		if (!GetTempPathW(ARRAYSIZE(buf), buf)) { // will have trailing backslash
			throw std::system_error(GetLastError(), std::system_category(),
				"GetTempPath failed");
		}
		std::wstring ret = buf;
		ret.resize(ret.length() - 1); // remove trailing backslash
		return ret;
	}

	static std::wstring exe_itself() {
		wchar_t buf[MAX_PATH + 1]{};
		if (!GetModuleFileNameW(nullptr, buf, ARRAYSIZE(buf))) { // full path name
			throw std::system_error(GetLastError(), std::system_category(),
				"GetModuleFileName failed");
		}
		std::wstring ret = buf;
		ret.resize(ret.find_last_of(L'\\')); // truncate removing EXE filename and trailing backslash
#ifdef _DEBUG
		ret.resize(ret.find_last_of(L'\\')); // bypass "Debug" folder, remove trailing backslash too
#ifdef _WIN64
		ret.resize(ret.find_last_of(L'\\')); // bypass "x64" folder, remove trailing backslash again
#endif
#endif
		return ret;
	}

private:
	static std::wstring _get_shell_folder(int clsId) {
		wchar_t buf[MAX_PATH + 1]{};
		com::check_hr(
			SHGetFolderPathW(nullptr, clsId, nullptr, 0, buf), // won't have trailing backslash
			"SHGetFolderPath failed");
		return buf;
	}
};

}//namespace wli
}//namespace wl
