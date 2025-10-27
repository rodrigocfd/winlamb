#pragma once
#include <stdexcept>
#include "lib-include-win.h"

namespace _wl_internal {

	// Initializes the GUI environment.
	struct Init {
		~Init();
		Init();
	};

	// Handles an uncaught exception.
	void uncaught_exception(const std::exception &e);

}

// Adjusts pixel values according to the current system DPI.
// These functions should be used every time you position something on the screen.
namespace wl::dpi {

	// Returns the value adjusted according to the current horizontal system DPI.
	int x(int xVal);

	// Returns the value adjusted according to the current vertical system DPI.
	int y(int yVal);

	// Returns a POINT with values adjusted according to the current horizontal and vertical system DPI.
	POINT pt(int xVal, int yVal);

	// Returns a SIZE with values adjusted according to the current horizontal and vertical system DPI.
	SIZE sz(int xVal, int yVal);

}

/** @brief Implements WinMain(), instantiates the main class, and calls run(). */
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
