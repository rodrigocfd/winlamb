/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <functional>
#include <vector>
#include <Windows.h>
#include <process.h>

namespace wl {

class thread final {
private:
	thread() = delete;

public:
	static void run_detached(std::function<void()> func) {
		// Cheap alternative to std::thread([](){ ... }).detach().
		struct cb_pack final {
			std::function<void()> func;
		};
		cb_pack* pack = new cb_pack{std::move(func)};

		uintptr_t hThread = _beginthreadex(nullptr, 0, [](void* ptr)->unsigned int {
			cb_pack* pPack = reinterpret_cast<cb_pack*>(ptr);
			try {
				pPack->func(); // invoke user callback
			} catch (const std::exception& e) {
				OutputDebugStringA("*** Error from within thread ***\n"); // can't call MessageBox outside UI thread
				OutputDebugStringA(e.what());
				OutputDebugStringA("\n");
			}
			delete pPack;
			_endthreadex(0); // http://www.codeproject.com/Articles/7732/A-class-to-synchronise-thread-completions/
			return 0;
		}, pack, 0, nullptr);

		CloseHandle(reinterpret_cast<HANDLE>(hThread));
	}

	static size_t num_processors() {
		SYSTEM_INFO si{};
		GetSystemInfo(&si);
		return si.dwNumberOfProcessors;
	}
};

}//namespace wl
