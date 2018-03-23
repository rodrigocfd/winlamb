/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "internals/dialog.h"

/**
 * hwnd_base
 *  w_inventory
 *   w_thread_capable
 *    w_user_control
 *     dialog
 *      dialog_control
 */

namespace wl {

// Inherit from this class to have a dialog to be used as a control within a parent window.
class dialog_control : public wli::dialog_user_control {
protected:
	struct setup_vars final : public wli::dialog_user_control::setup_vars { };

	setup_vars setup;

public:
	void create(HWND hParent, int ctrlId, POINT position, SIZE size) {
		// Dialog styles to be set on the resource editor:
		// - Border: none
		// - Control: true
		// - Style: child
		// - Visible: true (otherwise will start invisible)
		// - Client Edge: true (if you want a border, will add WS_EX_CLIENTEDGE)

		this->_basic_initial_checks(this->setup);

		if (!CreateDialogParamW(
			reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(hParent, GWLP_HINSTANCE)),
			MAKEINTRESOURCEW(setup.dialogId), hParent, _dialog_proc,
			reinterpret_cast<LPARAM>(this) ))
		{
			throw std::system_error(GetLastError(), std::system_category(),
				"CreateDialogParam failed for control dialog");
		}

		this->_check_bad_styles();
		SetWindowLongPtrW(this->hwnd(), GWLP_ID, ctrlId);
		SetWindowPos(this->hwnd(), nullptr,
			position.x, position.y,
			size.cx, size.cy, SWP_NOZORDER);
	}

	void create(const hwnd_base* parent, int ctrlId, POINT position, SIZE size) {
		this->create(parent->hwnd(), ctrlId, position, size);
	}

private:
	void _check_bad_styles() const {
		DWORD style = static_cast<DWORD>(GetWindowLongPtrW(this->hwnd(), GWL_STYLE));
		if (!(style & DS_CONTROL)) {
			// https://blogs.msdn.microsoft.com/oldnewthing/20040730-00/?p=38293
			throw std::logic_error("Control dialog template doesn't have DS_CONTROL style.");
		}
		if (!(style & WS_CHILD)) {
			throw std::logic_error("Control dialog template doesn't have WS_CHILD style.");
		}
	}
};

}//namespace wl
