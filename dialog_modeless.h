/**
 * Part of WinLamb - Win32 API Lambda Library
 * @author Rodrigo Cesar de Freitas Dias
 * @see https://github.com/rodrigocfd/winlamb
 */

#pragma once
#include "base_dialog.h"
#include "base_loop.h"

/**
 *                                <-------- base_msgs <-- msg_[any] <-------+
 *             +-- base_inventory <--+                                      +-- [user]
 * base_wnd <--+                     +-- base_dialog <-- dialog_modeless <--+
 *             +---- base_wheel <----+
 */

namespace wl {

// Inherit from this class to have a dialog modeless popup.
class dialog_modeless : public base::dialog {
private:
	base::loop* _parent;
protected:
	base::dialog::setup_vars setup;

	explicit dialog_modeless(size_t msgsReserve = 0) : dialog(msgsReserve + 2) {
		this->on_message(WM_CLOSE, [&](params)->INT_PTR {
			DestroyWindow(this->hwnd());
			return TRUE;
		});
		this->on_message(WM_NCDESTROY, [&](params)->INT_PTR {
			this->_parent->_remove_modeless(this->hwnd());
			return TRUE;
		});
	}

public:
	void show(base::loop* parent) {
		if (!this->dialog::_basic_initial_checks(this->setup)) return;
		this->_parent = parent;
		HINSTANCE hInst = reinterpret_cast<HINSTANCE>(
			GetWindowLongPtrW(parent->hwnd(), GWLP_HINSTANCE));

		if (!CreateDialogParamW(hInst, MAKEINTRESOURCEW(this->setup.dialogId),
			parent->hwnd(), base::dialog::_dialog_proc,
			reinterpret_cast<LPARAM>(this)) )
		{
			OutputDebugStringW(L"ERROR: modeless dialog not created, CreateDialogParam failed.\n");
			return;
		}

		parent->_add_modeless(this->hwnd());
		ShowWindow(this->hwnd(), SW_SHOW);
	}

	void show(base::loop* parent, POINT clientPos) {
		this->show(parent);
		POINT parentPos = clientPos;
		ClientToScreen(parent->hwnd(), &parentPos); // now relative to parent
		SetWindowPos(this->hwnd(), nullptr,
			parentPos.x, parentPos.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}
};

}//namespace wl