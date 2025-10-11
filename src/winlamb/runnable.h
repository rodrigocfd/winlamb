#pragma once
#include <stdexcept>
#include <Windows.h>

namespace _wl_internal {

	// Initializes the GUI environment.
	struct Init {
		~Init();
		Init();
	};

	// Handles an uncaught exception.
	void uncaught_exception(const std::exception &e);

}

namespace wl {

	// Returns the value adjusted according to the current horizontal system DPI.
	int dpi_x(int x);

	// Returns the value adjusted according to the current vertical system DPI.
	int dpi_y(int y);

}

// Implements WinMain(), instantiates the main class, and calls run().
#define RUN_MAIN(userwnd, mainwnd) \
	int APIENTRY wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int cmdShow) { \
		_wl_internal::Init init; \
		int ret = -1; \
		try { \
			userwnd w; \
			ret = w.mainwnd.run(hInst, cmdShow); \
		} catch (const std::exception &e) { \
			_wl_internal::uncaught_exception(e); \
		} \
		if (_CrtDumpMemoryLeaks()) MessageBoxW(nullptr, L"A memory leak was found.", L"Memory leak", MB_ICONERROR); \
		return ret; \
	}
