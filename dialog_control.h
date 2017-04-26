/**
 * Part of WinLamb - Win32 API Lambda Library
 * @author Rodrigo Cesar de Freitas Dias
 * @see https://github.com/rodrigocfd/winlamb
 */

#pragma once
#include "base_dialog.h"
#include "base_user_control.h"

/**
 *                                   +----------- base_msgs <-- msg_[any] <----------+
 *                                <--+                                               +-- [user]
 *             +-- base_inventory <------ base_user_control <--+                     |
 *             |                  <--+                         +-- dialog_control <--+
 * base_wnd <--+                     +------ base_dialog <-----+
 *             +---- base_wheel <----+
 */

namespace wl {

// Inherit from this class to have a dialog to be used as a control within a parent window.
class dialog_control :
	public base::dialog,
	public base::user_control
{
protected:
	base::dialog::setup_vars setup;

	explicit dialog_control(size_t msgsReserve = 0) : dialog(msgsReserve) { }

public:
	bool create(HWND hParent, int controlId, POINT position, SIZE size) {
		// Dialog styles to be set on the resource editor:
		// - Border: none
		// - Control: true
		// - Style: child
		// - Visible: true (otherwise will start invisible)
		// - Client Edge: true (if you want a border, will add WS_EX_CLIENTEDGE)

		if (!this->dialog::_basic_initial_checks(this->setup)) return false;

		if (!CreateDialogParamW(
			reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(hParent, GWLP_HINSTANCE)),
			MAKEINTRESOURCEW(setup.dialogId), hParent, base::dialog::_dialog_proc,
			reinterpret_cast<LPARAM>(this) ))
		{
			OutputDebugStringW(L"ERROR: control dialog not created, CreateDialogParam failed.\n");
			return false;
		}

		this->_check_bad_styles();
		SetWindowLongPtrW(this->hwnd(), GWLP_ID, controlId);
		SetWindowPos(this->hwnd(), nullptr,
			position.x, position.y,
			size.cx, size.cy, SWP_NOZORDER);
		return true;
	}

	bool create(const base::wnd* parent, int controlId, POINT position, SIZE size) {
		return this->create(parent->hwnd(), controlId, position, size);
	}

private:
	void _check_bad_styles() {
		DWORD style = static_cast<DWORD>(GetWindowLongPtrW(this->hwnd(), GWL_STYLE));
		if (!(style & DS_CONTROL)) {
			OutputDebugStringW(L"ERROR: control template doesn't have DS_CONTROL style.\n");
		}
		if (!(style & WS_CHILD)) {
			OutputDebugStringW(L"ERROR: control template doesn't have WS_CHILD style.\n");
		}
	}
};

}//namespace wl