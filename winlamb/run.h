
#pragma once
#include <crtdbg.h>
#include <Windows.h>
#include <CommCtrl.h>

namespace _wli {

void lippincott(); // avoids #include "run_funcs.h"

template<typename wnd_mainT>
int run_main(HINSTANCE hinst, int cmd_show)
{
	int ret = 0;

	try { // any exception which was not caught, except those from within message lambdas
		wnd_mainT w;
		ret = w.run_main(hinst, cmd_show);
	} catch (...) {
		lippincott();
		ret = -1;
	}

	_ASSERT(!_CrtDumpMemoryLeaks()); // note: runs before global objects are freed
	return ret;
}

// Instantiates your main class into a generic WinMain function.
#define RUN(wnd_mainT) \
int APIENTRY wWinMain(_In_ HINSTANCE hinst, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int cmd_show) { \
	return _wli::run_main<wnd_mainT>(hinst, cmd_show); \
}

}
