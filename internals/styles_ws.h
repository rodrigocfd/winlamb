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

// Window styles.
enum class ws : DWORD {
	// No style flags.
	NONE = 0L,
	// The window has a thin-line border.
	BORDER = WS_BORDER,
	// The window has a title bar (includes the WS_BORDER style).
	CAPTION = WS_CAPTION,
	// The window is a child window. A window with this style cannot have
	// a menu bar. This style cannot be used with the WS_POPUP style.
	CHILD = WS_CHILD,
	// Excludes the area occupied by child windows when drawing occurs
	// within the parent window. This style is used when creating the
	// parent window.
	CLIPCHILDREN = WS_CLIPCHILDREN,
	// Clips child windows relative to each other; that is, when a particular
	// child window receives a WM_PAINT message, the WS_CLIPSIBLINGS style
	// clips all other overlapping child windows out of the region of the
	// child window to be updated. If WS_CLIPSIBLINGS is not specified and
	// child windows overlap, it is possible, when drawing within the client
	// area of a child window, to draw within the client area of a neighboring
	// child window.
	CLIPSIBLINGS = WS_CLIPSIBLINGS,
	// The window is initially disabled. A disabled window cannot receive
	// input from the user. To change this after a window has been created,
	// use the EnableWindow function.
	DISABLED = WS_DISABLED,
	// The window has a border of a style typically used with dialog boxes.
	// A window with this style cannot have a title bar.
	DLGFRAME = WS_DLGFRAME,
	// The window is the first control of a group of controls. The group
	// consists of this first control and all controls defined after it, up
	// to the next control with the <c>WS_GROUP</c> style. The first control in
	// each group usually has the WS_TABSTOP style so that the user can move from
	// group to group. The user can subsequently change the keyboard focus
	// from one control in the group to the next control in the group by using
	// the direction keys.
	// You can turn this style on and off to change dialog box navigation. To
	// change this style after a window has been created, use the SetWindowLong
	// function.
	GROUP = WS_GROUP,
	// The window has a horizontal scroll bar.
	HSCROLL = WS_HSCROLL,
	// The window is initially maximized.
	MAXIMIZE = WS_MAXIMIZE,
	// The window has a maximize button. Cannot be combined with the
	// WS_EX_CONTEXTHELP style. The WS_SYSMENU style must also be specified. 
	MAXIMIZEBOX = WS_MAXIMIZEBOX,
	// The window is initially minimized. Same as the WS_ICONIC style.
	MINIMIZE = WS_MINIMIZE,
	// The window has a minimize button. Cannot be combined with the
	// WS_EX_CONTEXTHELP style. The WS_SYSMENU style must also be specified. 
	MINIMIZEBOX = WS_MINIMIZEBOX,
	// The window is an overlapped window. An overlapped window has a title bar
	// and a border. Same as the WS_TILED style.
	OVERLAPPED = WS_OVERLAPPED,
	// The window is an overlapped window. Same as the WS_TILEDWINDOW style.
	OVERLAPPEDWINDOW = WS_OVERLAPPEDWINDOW,
	// The windows is a pop-up window. This style cannot be used with the
	// WS_CHILD style.
	POPUP = WS_POPUP,
	// The window is a pop-up window. The WS_CAPTION and WS_POPUPWINDOW
	// styles must be combined to make the window menu visible.
	POPUPWINDOW = WS_POPUPWINDOW,
	// The window has a sizing border. Same as the WS_THICKFRAME style.
	SIZEBOX = WS_SIZEBOX,
	// The window has a window menu on its title bar. The WS_CAPTION style
	// must also be specified.
	SYSMENU = WS_SYSMENU,
	// The window is a control that can receive the keyboard focus when the
	// user presses the TAB key. Pressing the TAB key changes the keyboard
	// focus to the next control with the WS_TABSTOP style.
	// You can turn this style on and off to change dialog box navigation. To
	// change this style after a window has been created, use the SetWindowLong
	// function. For user-created windows and modeless dialogs to work with tab
	// stops, alter the message loop to call the IsDialogMessage function.
	TABSTOP = WS_TABSTOP,
	// The window is initially visible. This style can be turned on and off by
	// using the ShowWindow or SetWindowPos function.
	VISIBLE = WS_VISIBLE,
	// The window has a vertical scroll bar.
	VSCROLL = WS_VSCROLL
};

ENABLE_BITMASK_OPERATORS(ws);

}//namespace wl
