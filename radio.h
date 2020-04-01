/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "internals/base_focus_pubm.h"
#include "internals/base_text_pubm.h"
#include "internals/base_native_ctrl_pubm.h"
#include "internals/styler.h"
#include "wnd.h"

namespace wl {

// Wrapper to native radio button control.
class radio final :
	public wnd,
	public _wli::base_native_ctrl_pubm<radio>,
	public _wli::base_focus_pubm<radio>,
	public _wli::base_text_pubm<radio>
{
private:
	class _styler final : public _wli::styler<radio> {
	public:
		explicit _styler(radio* pRadio) noexcept : styler(pRadio) { }

		radio& first_in_group(bool doSet) noexcept {
			return this->set_style(doSet, WS_GROUP);
		}
	};

	HWND                   _hWnd = nullptr;
	_wli::base_native_ctrl _baseNativeCtrl{_hWnd};

public:
	// Wraps window style changes done by Get/SetWindowLongPtr.
	_styler style{this};

	radio() noexcept :
		wnd(_hWnd), base_native_ctrl_pubm(_baseNativeCtrl),
		base_focus_pubm(_hWnd), base_text_pubm(_hWnd) { }

	radio(radio&&) = default;
	radio& operator=(radio&&) = default; // movable only

	radio& create(HWND hParent, int ctrlId,
		const wchar_t* caption, POINT pos, SIZE size)
	{
		this->_baseNativeCtrl.create(hParent, ctrlId, caption, pos, size,
			L"Button", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTORADIOBUTTON);
		return *this;
	}

	radio& create(const wnd* parent, int ctrlId,
		const wchar_t* caption, POINT pos, SIZE size)
	{
		return this->create(parent->hwnd(), ctrlId, caption, pos, size);
	}

	bool is_checked() const noexcept {
		return SendMessageW(this->_hWnd, BM_GETCHECK, 0, 0) == BST_CHECKED;
	}

	radio& set_check(bool checked) noexcept {
		SendMessageW(this->_hWnd, BM_SETCHECK,
			checked ? BST_CHECKED : BST_UNCHECKED, 0);
		return *this;
	}

	radio& set_check_and_trigger(bool checked) noexcept {
		this->set_check(checked);
		SendMessageW(GetParent(this->_hWnd), WM_COMMAND,
			MAKEWPARAM(GetDlgCtrlID(this->_hWnd), 0),
			reinterpret_cast<LPARAM>(this->_hWnd) ); // emulate user click
		return *this;
	}
};

}//namespace wl
