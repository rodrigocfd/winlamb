/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <Windows.h>
#include <CommCtrl.h>
#pragma comment(lib, "Comctl32.lib")
#pragma comment(linker, \
	"\"/manifestdependency:type='Win32' " \
	"name='Microsoft.Windows.Common-Controls' " \
	"version='6.0.0.0' " \
	"processorArchitecture='*' " \
	"publicKeyToken='6595b64144ccf1df' " \
	"language='*'\"")

template<typename wnd_mainT>
int _winlamb_run_main(HINSTANCE hInst, int cmdShow) {
	int ret = 0;
	try { // any exception which was not caught, except those from within message lambdas
		wnd_mainT wndMain;
		ret = wndMain.winmain_run(hInst, cmdShow);
	} catch (const std::exception& e) {
		MessageBoxA(nullptr, e.what(), "Oops... an exception was thrown", MB_ICONERROR);
		ret = -1;
	}
	_ASSERT(!_CrtDumpMemoryLeaks());
	return ret;
}

#define RUN(wnd_mainT) \
int APIENTRY wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, int cmdShow) { \
	return _winlamb_run_main<wnd_mainT>(hInst, cmdShow); \
}
