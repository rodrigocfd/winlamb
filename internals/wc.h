/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <Windows.h>
#include "enable_bitmask_operators.h"

namespace wl {

// Window class styles.
enum class wc : UINT {
	// No class style flags.
	NONE = 0L,
	// Aligns the window's client area on a byte boundary (in
	// the x direction). This style affects the width of the
	// window and its horizontal placement on the display.
	BYTEALIGNCLIENT = CS_BYTEALIGNCLIENT,
	// Aligns the window on a byte boundary (in the x direction).
	// This style affects the width of the window and its
	// horizontal placement on the display.
	BYTEALIGNWINDOW = CS_BYTEALIGNWINDOW,
	// Allocates one device context to be shared by all windows
	// in the class. Because window classes are process specific,
	// it is possible for multiple threads of an application to
	// create a window of the same class. It is also possible for
	// the threads to attempt to use the device context simultaneously.
	// When this happens, the system allows only one thread to
	// successfully finish its drawing operation. 
	CLASSDC = CS_CLASSDC,
	// Sends a double-click message to the window procedure when the
	// user double-clicks the mouse while the cursor is within a
	// window belonging to the class. 
	DBLCLKS = CS_DBLCLKS,
	// Enables the drop shadow effect on a window. The effect is turned
	// on and off through SPI_SETDROPSHADOW. Typically, this is enabled
	// for small, short-lived windows such as menus to emphasize their
	// Z-order relationship to other windows. Windows created from a
	// class with this style must be top-level windows; they may not be
	// child windows.
	DROPSHADOW = CS_DROPSHADOW,
	// Redraws the entire window if a movement or size adjustment changes
	// the width of the client area.
	HREDRAW = CS_HREDRAW,
	// Disables Close on the window menu.
	NOCLOSE = CS_NOCLOSE,
	// Allocates a unique device context for each window in the class.
	OWNDC = CS_OWNDC,
	// Sets the clipping rectangle of the child window to that of the
	// parent window so that the child can draw on the parent. A window
	// with the CS_PARENTDC style bit receives a regular device context
	// from the system's cache of device contexts. It does not give the
	// child the parent's device context or device context settings.
	// Specifying CS_PARENTDC enhances an application's performance. 
	PARENTDC = CS_PARENTDC,
	// Saves, as a bitmap, the portion of the screen image obscured by a
	// window of this class. When the window is removed, the system uses
	// the saved bitmap to restore the screen image, including other
	// windows that were obscured. Therefore, the system does not send
	// WM_PAINT messages to windows that were obscured if the memory used
	// by the bitmap has not been discarded and if other screen actions
	// have not invalidated the stored image. 
	// This style is useful for small windows (for example, menus or dialog
	// boxes) that are displayed briefly and then removed before other
	// screen activity takes place. This style increases the time required
	// to display the window, because the system must first allocate memory
	// to store the bitmap.
	SAVEBITS = CS_SAVEBITS,
	// Redraws the entire window if a movement or size adjustment changes
	// the height of the client area.
	VREDRAW = CS_VREDRAW
};

ENABLE_BITMASK_OPERATORS(wc);

}//namespace wl
