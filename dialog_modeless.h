/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "internals/dialog.h"
#include "internals/loop.h"
#include "internals/w_text.h"

/**
 * hwnd_base
 *  w_inventory
 *   w_thread_capable
 *    dialog
 *     w_text
 *      dialog_modeless
 */

namespace wl {

// Inherit from this class to have a dialog modeless popup.
class dialog_modeless :
	public wli::w_text<
		dialog_modeless, wli::dialog_thread_capable>
{
protected:
	struct setup_vars final : public wli::dialog_thread_capable::setup_vars { };

private:
	wli::loop* _pLoop = nullptr; // pointer to parent's loop instance

protected:
	setup_vars setup;

	dialog_modeless() {
		this->on_message(WM_CLOSE, [this](params) noexcept->INT_PTR {
			DestroyWindow(this->hwnd());
			return TRUE;
		});
		this->on_message(WM_NCDESTROY, [this](params)->INT_PTR {
			if (this->_pLoop) {
				this->_pLoop->remove_modeless(this->hwnd());
			}
			return TRUE;
		});
	}

public:
	template<typename has_loopT>
	void show(has_loopT* parent) {
		this->_basic_initial_checks(this->setup);

		this->_parent = parent;
		HINSTANCE hInst = reinterpret_cast<HINSTANCE>(
			GetWindowLongPtrW(parent->hwnd(), GWLP_HINSTANCE));

		if (!CreateDialogParamW(hInst, MAKEINTRESOURCEW(this->setup.dialogId),
			parent->hwnd(), _dialog_proc, reinterpret_cast<LPARAM>(this)) )
		{
			throw std::system_error(GetLastError(), std::system_category(),
				"CreateDialogParam failed for modeless dialog");
		}

		this->_pLoop = parent->_loop; // keep parent's wli::loop instance; won't compile if parent doesn't have it
		this->_pLoop->add_modeless(this->hwnd());
		ShowWindow(this->hwnd(), SW_SHOW);
	}

	template<typename has_loopT>
	void show(has_loopT* parent, POINT clientPos) {
		this->show(parent);
		POINT parentPos = clientPos;
		ClientToScreen(parent->hwnd(), &parentPos); // now relative to parent
		SetWindowPos(this->hwnd(), nullptr,
			parentPos.x, parentPos.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}
};

}//namespace wl
