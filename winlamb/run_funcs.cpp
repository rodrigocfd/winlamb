
#include <system_error>
#include "run_funcs.h"
#include <CommCtrl.h>
#include <VersionHelpers.h>
#pragma comment(lib, "Comctl32.lib")
using namespace wl;

void _wli::first_calls()
{
	if (IsWindowsVistaOrGreater()) {
		if (!SetProcessDPIAware()) {
			throw std::system_error(GetLastError(), std::system_category(),
				"SetProcessDPIAware failed");
		}
	}

	InitCommonControls();

	BOOL bval = FALSE;
	if (!SetUserObjectInformationW(GetCurrentProcess(),
		UOI_TIMERPROC_EXCEPTION_SUPPRESSION, &bval, sizeof(BOOL)))
	{
		throw std::system_error(GetLastError(), std::system_category(),
			"SetUserObjectInformation failed");
	}
}

void _wli::lippincott()
{
	const char* caption = "Oops... unknown error";
	const char* msg = "An unknown exception, not derived from std::exception, was thrown.";

	// https://stackoverflow.com/a/48036486/6923555
	try { throw; }
	catch (const std::invalid_argument& e) { msg = e.what(); caption = "Oops... invalid argument error"; }
	catch (const std::out_of_range& e)     { msg = e.what(); caption = "Oops... out of range error"; }
	catch (const std::logic_error& e)      { msg = e.what(); caption = "Oops... logic error"; }
	catch (const std::system_error& e)     { msg = e.what(); caption = "Oops... system error"; }
	catch (const std::runtime_error& e)    { msg = e.what(); caption = "Oops... runtime error"; }
	catch (const std::exception& e)        { msg = e.what(); caption = "Oops... generic error"; }

	MessageBoxA(nullptr, msg, caption, MB_ICONERROR);
}

const font& _wli::ui_font()
{
	static font ui_font;

	if (ui_font.hfont() == nullptr) { // not created yet?
		NONCLIENTMETRICSW ncm = { sizeof(NONCLIENTMETRICSW) };
		if (!SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0)) {
			throw std::system_error(GetLastError(), std::system_category(),
				"CreateFontIndirect failed");
		}
		ui_font = std::move(font(ncm.lfMenuFont));
	}

	return ui_font;
}

int _wli::run_main_loop(HWND hwnd, HACCEL haccel)
{
	MSG msg = {0};
	for (;;) {
		if (BOOL ret = GetMessageW(&msg, nullptr, 0, 0); ret == -1) {
			throw std::system_error(GetLastError(), std::system_category(),
				"GetMessage failed in main loop");
		} else if (ret == 0) {
			// WM_QUIT was sent, gracefully terminate the program.
			// wParam has the program exit code.
			// https://docs.microsoft.com/en-us/windows/win32/winmsg/using-messages-and-message-queues
			return static_cast<int>(msg.wParam);
		}

		// Check if modeless...

		// If a child window, will retrieve its top-level parent.
		// If a top-level, use itself.
		HWND htop_level = GetAncestor(hwnd, GA_ROOT);

		// If we have an accelerator table, try to translate the message.
		if (haccel != nullptr && TranslateAcceleratorW(hwnd, haccel, &msg) != 0) {
			continue; // message translated, no further processing needs to be done
		}

		if (IsDialogMessageW(hwnd, &msg)) {
			continue; // process all keyboard actions for child controls
		}

		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
}
