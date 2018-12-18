/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <stdexcept>
#include <Windows.h>

namespace wl {
namespace wli {

// Common ground to all native controls.
class base_native_ctrl final {
private:
	HWND& _hWnd;

public:
	base_native_ctrl(HWND& hWnd) noexcept :
		_hWnd(hWnd) { }

	// Returns the window handle.
	HWND hwnd() const noexcept {
		return this->_hWnd;
	}

	// Ties this class instance to an existing native control.
	void assign(HWND hCtrl) {
		if (this->_hWnd) {
			throw std::logic_error("Trying to assign or create a native control twice.");
		}
		this->_hWnd = hCtrl;
	}

	// Calls CreateWindowEx, then assign.
	void create(HWND hParent, int ctrlId, const wchar_t* title,
		POINT pos, SIZE size, const wchar_t* className,
		DWORD styles = (WS_CHILD | WS_VISIBLE), DWORD exStyles = 0)
	{
		return this->assign(CreateWindowExW(exStyles, className, title, styles,
			pos.x, pos.y, size.cx, size.cy, hParent,
			reinterpret_cast<HMENU>(static_cast<UINT_PTR>(ctrlId)),
			reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(hParent, GWLP_HINSTANCE)),
			nullptr));
	}
};

}//namespace wli
}//namespace wl