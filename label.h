/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "internals/base_text_impl.h"
#include "internals/base_native_ctrl_impl.h"
#include "internals/styler.h"
#include "wnd.h"

namespace wl {

// Wrapper to native label control.
class label final :
	public wnd,
	public wli::base_native_ctrl_impl<label>,
	public wli::base_text_impl<label>
{
private:
	HWND                  _hWnd = nullptr;
	wli::base_native_ctrl _baseNativeCtrl{_hWnd};

public:
	// Wraps window style changes done by Get/SetWindowLongPtr.
	wli::styler<label> style{this};

	label() noexcept :
		wnd(_hWnd), base_native_ctrl_impl(_baseNativeCtrl), base_text_impl(_hWnd) { }

	label(label&&) = default;
	label& operator=(label&&) = default; // movable only

	label& create(HWND hParent, int ctrlId,
		const wchar_t* caption, POINT pos, SIZE size)
	{
		this->_baseNativeCtrl.create(hParent, ctrlId, caption, pos, size, L"Static");
		return *this;
	}

	label& create(const wnd* parent, int ctrlId,
		const wchar_t* caption, POINT pos, SIZE size)
	{
		return this->create(parent->hwnd(), ctrlId, caption, pos, size);
	}
};

}//namespace wl
