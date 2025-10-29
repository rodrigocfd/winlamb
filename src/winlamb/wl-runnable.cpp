#include <system_error>
#include "lib-include-win.h"
#include <CommCtrl.h>
#include <VersionHelpers.h>
#include <ole2.h>
#include "runnable.h"
using namespace _wl_internal;

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
				throw std::system_error(err, std::system_category(), "SetUserObjectInformation failed");
			}
		}
	}

	HDC hdcScreen = GetDC(nullptr);
	logPixelsX = GetDeviceCaps(hdcScreen, LOGPIXELSX);
	logPixelsY = GetDeviceCaps(hdcScreen, LOGPIXELSY);
	ReleaseDC(nullptr, hdcScreen);
}

GuiInit::~GuiInit() {
	OleUninitialize();
}

GuiInit::GuiInit() {
	init_stuff();
	OleInitialize(nullptr);
}

int wl::dpi::x(int xVal) {
	init_stuff();
	return MulDiv(xVal, logPixelsX, 96);
}

int wl::dpi::y(int yVal) {
	init_stuff();
	return MulDiv(yVal, logPixelsY, 96);
}

POINT wl::dpi::pt(int xVal, int yVal) {
	return {.x = x(xVal), .y = y(yVal)};
}

SIZE wl::dpi::sz(int xVal, int yVal) {
	return {.cx = x(xVal), .cy = y(yVal)};
}
