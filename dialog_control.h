/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "internals/base_dialog.h"
#include "internals/base_msg_impl.h"
#include "internals/base_thread_impl.h"
#include "internals/base_user_ctrl.h"
#include "internals/styler.h"
#include "wnd.h"

namespace wl {

// Inherit from this class to have a dialog to be used as a control within a parent window.
class dialog_control :
	public wnd,
	public wli::base_msg_impl<INT_PTR>,
	public wli::base_thread_impl<INT_PTR, TRUE>
{
private:
	HWND                               _hWnd = nullptr;
	wli::base_msg<INT_PTR>             _baseMsg{_hWnd};
	wli::base_thread<INT_PTR, TRUE>    _baseThread{_baseMsg};
	wli::base_dialog                   _baseDialog{_hWnd, _baseMsg};
	wli::base_user_ctrl<INT_PTR, TRUE> _baseUserCtrl{_baseMsg};

public:
	// Defines window creation parameters.
	wli::base_dialog::setup_vars setup;

	// Wraps window style changes done by Get/SetWindowLongPtr.
	wli::styler<dialog_control> style{this};

protected:
	dialog_control() :
		wnd(_hWnd), base_msg_impl(_baseMsg), base_thread_impl(_baseThread) { }

public:
	dialog_control(dialog_control&&) = default;
	dialog_control& operator=(dialog_control&&) = default; // movable only

	// Returns the control ID.
	int ctrl_id() const noexcept {
		return GetDlgCtrlID(this->_hWnd);
	}

	// Creates the control dialog, returning immediately.
	void create(HWND hParent, int ctrlId, POINT position, SIZE size) {
		// Dialog styles to be set on the resource editor:
		// - Border: none
		// - Control: true
		// - Style: child
		// - Visible: true (otherwise will start invisible)
		// - Client Edge: true (if you want a border, will add WS_EX_CLIENTEDGE)

		this->_check_bad_styles();

		if (!this->_baseDialog.create_dialog_param(this->setup, hParent)) {
			throw std::system_error(GetLastError(), std::system_category(),
				"CreateDialogParam failed for control dialog");
		}

		SetWindowLongPtrW(this->_hWnd, GWLP_ID, ctrlId); // so the control has an ID
		SetWindowPos(this->_hWnd, nullptr,
			position.x, position.y,
			size.cx, size.cy, SWP_NOZORDER);
	}

	// Creates the control dialog, returning immediately.
	void create(const wnd* parent, int ctrlId, POINT position, SIZE size) {
		this->create(parent->hwnd(), ctrlId, position, size);
	}

private:
	void _check_bad_styles() const {
		if (!this->style.has_style(DS_CONTROL)) {
			// https://blogs.msdn.microsoft.com/oldnewthing/20040730-00/?p=38293
			throw std::logic_error("Control dialog template doesn't have DS_CONTROL style.");
		}
		if (!this->style.has_style(WS_CHILD)) {
			throw std::logic_error("Control dialog template doesn't have WS_CHILD style.");
		}
	}
};

}//namespace wl
