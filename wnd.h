/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <Windows.h>

namespace wl {

// Common ground to everything which has a HWND.
// Actually, to follow the convention, it should be named base_wnd_pubm.
class wnd {
private:
	const HWND& _hWnd;

public:
	wnd(const HWND& hWnd) noexcept :
		_hWnd(hWnd) { }

	// Returns the window handle.
	HWND hwnd() const noexcept {
		return this->_hWnd;
	}
};

}//namespace wl
