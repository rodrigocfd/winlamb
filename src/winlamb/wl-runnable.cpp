#include <system_error>
#include <Windows.h>
#include <CommCtrl.h>
#include <VersionHelpers.h>
#include "runnable.h"
using namespace _wl_internal;

void _wl_internal::uncaught_exception(const std::exception& e) {
	MessageBoxA(nullptr, e.what(), "Uncaught exception", MB_ICONERROR);
	PostQuitMessage(-1);
}

static int logPixelsX = 0, logPixelsY = 0;

static void init_stuff() {
	if (logPixelsX) return; // run only once

	InitCommonControls();

	if (IsWindows8OrGreater()) [[likely]] {
		HANDLE hProcess = GetCurrentProcess();
		BOOL bVal = FALSE;
		if (!SetUserObjectInformationW(hProcess, UOI_TIMERPROC_EXCEPTION_SUPPRESSION, &bVal, sizeof(BOOL))) [[unlikely]] {
			DWORD err = GetLastError();
			if (err == ERROR_INVALID_PARAMETER) {
				// Do nothing: Wine doesn't support SetUserObjectInformation for now.
				// https://bugs.winehq.org/show_bug.cgi?id=54951
			} else {
				throw std::system_error(err, std::system_category(), "SetUserObjectInformation failed.");
			}
		}
	}

	HDC hdcScreen = GetDC(nullptr);
	logPixelsX = GetDeviceCaps(hdcScreen, LOGPIXELSX);
	logPixelsY = GetDeviceCaps(hdcScreen, LOGPIXELSY);
	ReleaseDC(nullptr, hdcScreen);
}

Init::~Init() {
	OleUninitialize();
}

Init::Init() {
	init_stuff();
	OleInitialize(nullptr);
}

int wl::dpi_x(int x) {
	init_stuff();
	return MulDiv(x, logPixelsX, 96);
}

int wl::dpi_y(int y) {
	init_stuff();
	return MulDiv(y, logPixelsY, 96);
}
