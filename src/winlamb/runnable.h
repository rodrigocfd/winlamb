#pragma once
#include "lib-include-win.h"

namespace _wl_internal {

	/** Initializes the GUI environment. */
	struct GuiInit {
		~GuiInit();
		GuiInit();
	};

}

/// @brief Adjusts pixel values according to the current [system DPI].
///
/// These functions should be used every time you use pixels on the screen.
///
/// [system DPI]: https://learn.microsoft.com/en-us/windows/win32/hidpi/high-dpi-desktop-application-development-on-windows
namespace wl::dpi {

	/** Returns the value adjusted according to the current horizontal system DPI. */
	int x(int xVal);

	/** Returns the value adjusted according to the current vertical system DPI. */
	int y(int yVal);

	/// Returns a [`POINT`] with values adjusted according to the current horizontal and vertical system DPI.
	///
	/// [`POINT`]: https://learn.microsoft.com/en-us/windows/win32/api/windef/ns-windef-point
	POINT pt(int xVal, int yVal);

	/// Returns a [`POINT`] with values adjusted according to the current horizontal and vertical system DPI.
	///
	/// [`POINT`]: https://learn.microsoft.com/en-us/windows/win32/api/windef/ns-windef-point
	POINT pt(POINT value);

	/// Returns a [`SIZE`] with values adjusted according to the current horizontal and vertical system DPI.
	///
	/// [`SIZE`]: https://learn.microsoft.com/en-us/windows/win32/api/windef/ns-windef-size
	SIZE sz(int xVal, int yVal);

	/// Returns a [`SIZE`] with values adjusted according to the current horizontal and vertical system DPI.
	///
	/// [`SIZE`]: https://learn.microsoft.com/en-us/windows/win32/api/windef/ns-windef-size
	SIZE sz(SIZE value);

}

/** @brief Implements WinMain(), instantiates the main class, and calls run(). */
#define RUN_MAIN(usermainclass, mainwndobj) \
	int APIENTRY wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int cmdShow) { \
		int ret = -1; \
		{ \
			_wl_internal::GuiInit guiInit; \
			usermainclass w; \
			ret = w.mainwndobj.run(hInst, cmdShow); \
		} \
		if (_CrtDumpMemoryLeaks()) [[unlikely]] { \
			MessageBoxW(nullptr, L"A memory leak was found.", L"Memory leak", MB_ICONERROR); \
		} \
		return ret; \
	}
