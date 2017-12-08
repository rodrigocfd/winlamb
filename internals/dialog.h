/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "ui_thread.h"
#include "user_control.h"
#include "hover_scroll.h"
#include "styler.h"
#include "../font.h"

/**
 * hwnd_base
 *  inventory
 *   ui_thread
 *    [user_control]
 *     dialog
 */

namespace wl {
class dialog_control; // friend forward declarations
class dialog_main;
class dialog_modal;
class dialog_modeless;

namespace wli {

template<typename baseT>
class dialog : public baseT {
	friend class dialog_control;
	friend class dialog_main;
	friend class dialog_modal;
	friend class dialog_modeless;

private:
	class _styler final : public wli::styler<dialog> {
	public:
		explicit _styler(dialog* pDialog) noexcept : styler(pDialog) { }
	};

protected:
	struct setup_vars {
		int dialogId = 0;
	};

public:
	~dialog() {
		if (this->hwnd()) {
			SetWindowLongPtrW(this->hwnd(), GWLP_USERDATA, 0);
		}
	}

	dialog(const dialog&) = delete;
	dialog& operator=(const dialog&) = delete; // non-copyable, non-movable

protected:
	_styler style{this};

	dialog() = default;

private:
	void _basic_initial_checks(const setup_vars& setup) const {
		if (this->hwnd()) {
			throw std::logic_error("Tried to create dialog twice.");
		}
		if (!setup.dialogId) {
			throw std::logic_error("No dialog resource ID given on this->setup.dialogId.");
		}
	}

	static INT_PTR CALLBACK _dialog_proc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp) noexcept {
		dialog* pSelf = nullptr;
		INT_PTR ret = FALSE; // default for non-processed messages

		if (msg == WM_INITDIALOG) {
			pSelf = reinterpret_cast<dialog*>(lp);
			SetWindowLongPtrW(hDlg, DWLP_USER, reinterpret_cast<LONG_PTR>(pSelf));
			font::util::set_ui_on_children(hDlg); // if user creates controls manually, font must be set manually on them
			pSelf->_hWnd = hDlg; // store HWND
		} else {
			pSelf = reinterpret_cast<dialog*>(GetWindowLongPtrW(hDlg, DWLP_USER));
		}

		if (pSelf) {
			std::pair<bool, INT_PTR> procRet = pSelf->_process_msg(msg, wp, lp); // catches all message exceptions internally
			if (procRet.first) {
				ret = procRet.second; // message was processed
			}
		}

		if (msg == WM_INITDIALOG) {
			wli::hover_scroll::apply_behavior(pSelf->hwnd());
		} else if (msg == WM_NCDESTROY) { // cleanup
			SetWindowLongPtrW(hDlg, DWLP_USER, 0);
			if (pSelf) {
				pSelf->_hWnd = nullptr; // clear HWND
			}
		}

		return ret;
	}
};

using dialog_ui_thread = dialog<ui_thread<INT_PTR, TRUE>>;
using dialog_user_control = dialog<user_control<INT_PTR, TRUE>>;

}//namespace wli
}//namespace wl
