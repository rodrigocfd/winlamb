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

// Wrapper to native checkbox control.
class checkbox final :
	public wnd,
	public wli::base_native_ctrl_impl<checkbox>,
	public wli::base_text_impl<checkbox>
{
private:
	HWND                  _hWnd = nullptr;
	wli::base_native_ctrl _baseNativeCtrl{_hWnd};

public:
	// Wraps window style changes done by Get/SetWindowLongPtr.
	wli::styler<checkbox> style{this};

	checkbox() noexcept :
		wnd(_hWnd), base_native_ctrl_impl(_baseNativeCtrl), base_text_impl(_hWnd) { }

	checkbox(checkbox&&) = default;
	checkbox& operator=(checkbox&&) = default; // movable only

	checkbox& create(HWND hParent, int ctrlId,
		const wchar_t* caption, POINT pos, SIZE size)
	{
		this->_baseNativeCtrl.create(hParent, ctrlId, caption, pos, size,
			L"Button", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX);
		return *this;
	}

	checkbox& create(const wnd* parent, int ctrlId,
		const wchar_t* caption, POINT pos, SIZE size)
	{
		return this->create(parent->hwnd(), ctrlId, caption, pos, size);
	}

	bool is_checked() const noexcept {
		return SendMessageW(this->_hWnd, BM_GETCHECK, 0, 0) == BST_CHECKED;
	}

	checkbox& set_check(bool checked) noexcept {
		SendMessageW(this->_hWnd, BM_SETCHECK,
			checked ? BST_CHECKED : BST_UNCHECKED, 0);
		return *this;
	}

	checkbox& set_check_and_trigger(bool checked) noexcept {
		this->set_check(checked);
		SendMessageW(GetParent(this->_hWnd), WM_COMMAND,
			MAKEWPARAM(GetDlgCtrlID(this->_hWnd), 0),
			reinterpret_cast<LPARAM>(this->_hWnd) ); // emulate user click
		return *this;
	}
};

}//namespace wl
