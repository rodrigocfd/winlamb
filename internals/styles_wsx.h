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

// Window extended styles.
enum class wsx : DWORD {
	// No extended style flags.
	NONE = 0L,
	// The window accepts drag-drop files.
	ACCEPTFILES = WS_EX_ACCEPTFILES,
	// Forces a top-level window onto the taskbar when the window
	// is visible. 
	APPWINDOW = WS_EX_APPWINDOW,
	// The window has a border with a sunken edge.
	CLIENTEDGE = WS_EX_CLIENTEDGE,
	// Paints all descendants of a window in bottom-to-top painting order
	// using double-buffering. This cannot be used if the window has a class
	// style of either CS_OWNDC or CS_CLASSDC.
	// Windows 2000: This style is not supported.
	COMPOSITED = WS_EX_COMPOSITED,
	// The title bar of the window includes a question mark. When the user
	// clicks the question mark, the cursor changes to a question mark with
	// a pointer. If the user then clicks a child window, the child receives a
	// WM_HELP message. The child window should pass the message to the parent
	// window procedure, which should call the WinHelp function using the
	// HELP_WM_HELP command. The Help application displays a pop-up window that
	// typically contains help for the child window.
	// Cannot be used with the WS_MAXIMIZEBOX or WS_MINIMIZEBOX styles.
	CONTEXTHELP = WS_EX_CONTEXTHELP,
	// The window itself contains child windows that should take part in dialog
	// box navigation. If this style is specified, the dialog manager recurses
	// into children of this window when performing navigation operations such
	// as handling the TAB key, an arrow key, or a keyboard mnemonic.
	CONTROLPARENT = WS_EX_CONTROLPARENT,
	// The window has a double border; the window can, optionally, be created
	// with a title bar by specifying the WS_CAPTION style in the dwStyle parameter.
	DLGMODALFRAME = WS_EX_DLGMODALFRAME,
	// The window is a layered window. This style cannot be used if the window has a
	// class style of either CS_OWNDC or CS_CLASSDC.
	// Windows 8: The WS_EX_LAYERED style is supported for top-level windows and child
	// windows. Previous Windows versions support WS_EX_LAYERED only for top-level windows.
	LAYERED = WS_EX_LAYERED,
	// If the shell language is Hebrew, Arabic, or another language that supports reading
	// order alignment, the horizontal origin of the window is on the right edge.
	// Increasing horizontal values advance to the left. 
	LAYOUTRTL = WS_EX_LAYOUTRTL,
	// The window has generic left-aligned properties. This is the default.
	LEFT = WS_EX_LEFT,
	// If the shell language is Hebrew, Arabic, or another language that supports reading
	// order alignment, the vertical scroll bar (if present) is to the left of the client
	// area. For other languages, the style is ignored.
	LEFTSCROLLBAR = WS_EX_LEFTSCROLLBAR,
	// The window text is displayed using left-to-right reading-order properties.
	// This is the default.
	LTRREADING = WS_EX_LTRREADING,
	// The window is a MDI child window.
	MDICHILD = WS_EX_MDICHILD,
	// A top-level window created with this style does not become the foreground window when
	// the user clicks it. The system does not bring this window to the foreground when the
	// user minimizes or closes the foreground window.
	// The window should not be activated through programmatic access or via keyboard navigation
	// by accessible technology, such as Narrator.
	// To activate the window, use the SetActiveWindow or SetForegroundWindow function.
	// The window does not appear on the taskbar by default. To force the window to appear on
	// the taskbar, use the WS_EX_APPWINDOW style.
	NOACTIVATE = WS_EX_NOACTIVATE,
	// The window does not pass its window layout to its child windows.
	NOINHERITLAYOUT = WS_EX_NOINHERITLAYOUT,
	// The child window created with this style does not send the WM_PARENTNOTIFY message to
	// its parent window when it is created or destroyed.
	NOPARENTNOTIFY = WS_EX_NOPARENTNOTIFY,

#if(WINVER >= 0x0602)
	// The window does not render to a redirection surface. This is for windows that do not have
	// visible content or that use mechanisms other than surfaces to provide their visual.
	NOREDIRECTIONBITMAP = WS_EX_NOREDIRECTIONBITMAP,
#endif

	// The window is an overlapped window.
	OVERLAPPEDWINDOW = WS_EX_OVERLAPPEDWINDOW,
	// The window is palette window, which is a modeless dialog box that presents an
	// array of commands. 
	PALETTEWINDOW = WS_EX_PALETTEWINDOW,
	// The window has generic "right-aligned" properties. This depends on the window class. This
	// style has an effect only if the shell language is Hebrew, Arabic, or another language
	// that supports reading-order alignment; otherwise, the style is ignored.
	// Using the WS_EX_RIGHT style for static or edit controls has the same effect as using the
	// SS_RIGHT or ES_RIGHT style, respectively. Using this style with button controls has the
	// same effect as using BS_RIGHT and BS_RIGHTBUTTON styles.
	RIGHT = WS_EX_RIGHT,
	// The vertical scroll bar (if present) is to the right of the client area.
	// This is the default.
	RIGHTSCROLLBAR = WS_EX_RIGHTSCROLLBAR,
	// If the shell language is Hebrew, Arabic, or another language that supports reading-order
	// alignment, the window text is displayed using right-to-left reading-order properties. For
	// other languages, the style is ignored.
	RTLREADING = WS_EX_RTLREADING,
	// The window has a three-dimensional border style intended to be used for items that do
	// not accept user input.
	STATICEDGE = WS_EX_STATICEDGE,
	// The window is intended to be used as a floating toolbar. A tool window has a title bar that
	// is shorter than a normal title bar, and the window title is drawn using a smaller font. A
	// tool window does not appear in the taskbar or in the dialog that appears when the user
	// presses ALT+TAB. If a tool window has a system menu, its icon is not displayed on the title
	// bar. However, you can display the system menu by right-clicking or by typing ALT+SPACE. 
	TOOLWINDOW = WS_EX_TOOLWINDOW,
	// The window should be placed above all non-topmost windows and should stay above them, even
	// when the window is deactivated. To add or remove this style, use the SetWindowPos function.
	TOPMOST = WS_EX_TOPMOST,
	// The window should not be painted until siblings beneath the window (that were created by the
	// same thread) have been painted. The window appears transparent because the bits of underlying
	// sibling windows have already been painted.
	// To achieve transparency without these restrictions, use the SetWindowRgn function.
	TRANSPARENT_ = WS_EX_TRANSPARENT,
	// The window has a border with a raised edge.
	WINDOWEDGE = WS_EX_WINDOWEDGE
};

ENABLE_BITMASK_OPERATORS(wsx);

}//namespace wl
