/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "internals/base_text_pubm.h"
#include "internals/base_native_ctrl_pubm.h"
#include "internals/styler.h"
#include "wnd.h"

namespace wl {

// Wrapper to native button control.
class button final :
	public wnd,
	public wli::base_native_ctrl_pubm<button>,
	public wli::base_text_pubm<button>
{
private:
	HWND                  _hWnd = nullptr;
	wli::base_native_ctrl _baseNativeCtrl{_hWnd};

public:
	// Wraps window style changes done by Get/SetWindowLongPtr.
	wli::styler<button> style{this};

	button() noexcept :
		wnd(_hWnd), base_native_ctrl_pubm(_baseNativeCtrl), base_text_pubm(_hWnd) { }

	button(button&&) = default;
	button& operator=(button&&) = default; // movable only

	button& create(HWND hParent, int ctrlId,
		const wchar_t* caption, POINT pos, SIZE size = {75, 23})
	{
		this->_baseNativeCtrl.create(hParent, ctrlId, caption, pos, size, L"Button");
		return *this;
	}

	button& create(const wnd* parent, int ctrlId,
		const wchar_t* caption, POINT pos, SIZE size = {75, 23})
	{
		return this->create(parent->hwnd(), ctrlId, caption, pos, size);
	}
};

}//namespace wl
