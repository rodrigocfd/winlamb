/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "com.h"
#include "str.h"

namespace wl {

// Utilities to executable-related stuff.
class executable final {
private:
	executable() = delete;

public:
	// Retrieves path to current running EXE itself.
	static std::wstring get_own_path() {
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

	// Retrieves the program's command line, tokenized.
	static std::vector<std::wstring> get_cmd_line() {
		return str::split_quoted(GetCommandLineW());
	}

	// Synchronous execution of a command line in a separated process.
	static DWORD exec(const std::wstring& cmdLine) {
		SECURITY_ATTRIBUTES sa{};
		sa.nLength = sizeof(sa);
		sa.bInheritHandle = TRUE;

		STARTUPINFO si{};
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_SHOW;

		PROCESS_INFORMATION pi{};
		DWORD dwExitCode = 1; // returned by executed program

		std::wstring cmdLine2 = cmdLine; // http://blogs.msdn.com/b/oldnewthing/archive/2009/06/01/9673254.aspx

		if (!CreateProcessW(nullptr, &cmdLine2[0], &sa, nullptr, FALSE,
			0, nullptr, nullptr, &si, &pi))
		{
			throw std::system_error(GetLastError(), std::system_category(),
				"CreateProcess failed");
		}

		WaitForSingleObject(pi.hProcess, INFINITE); // the program flow is stopped here to wait
		GetExitCodeProcess(pi.hProcess, &dwExitCode);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		return dwExitCode;
	}

	// Runs the shell-associated program to the given file, for example Notepad to a TXT.
	static void shell_exec(const std::wstring& file, INT showCmd = SW_SHOWNORMAL) {
		com::lib comLib{com::lib::init::NOW};
		int h = static_cast<int>(reinterpret_cast<INT_PTR>(ShellExecuteW(nullptr,
			L"open", file.c_str(), nullptr, nullptr, showCmd)));
		if (h <= 8) {
			throw std::system_error(h, std::system_category(),
				"ShellExecute failed");
		} else if (h <= 32) {
			throw std::runtime_error(str::to_ascii(
				str::format(L"ShellExecute failed: error %d.", h) ));
		}
	}
};

}//namespace wl
