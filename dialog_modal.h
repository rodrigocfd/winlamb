/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "internals/dialog.h"
#include "internals/w_text.h"

/**
 * hwnd_base
 *  w_message
 *   w_thread_capable
 *    dialog
 *     w_text
 *      dialog_modal
 */

namespace wl {

// Inherit from this class to have a modal dialog popup.
class dialog_modal :
	public wli::w_text<
		dialog_modal, wli::dialog_thread_capable>
{
protected:
	struct setup_vars final : public wli::dialog_thread_capable::setup_vars { };

	setup_vars setup;

	dialog_modal() noexcept {
		this->on_message(WM_CLOSE, [this](params) noexcept->INT_PTR {
			EndDialog(this->hwnd(), IDOK);
			return TRUE;
		});
	}

public:
	// Shows the modal dialog, returning only after the modal is closed.
	int show(HWND hParent) {
		this->_basic_initial_checks(this->setup);

		INT_PTR ret = DialogBoxParamW(
			reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(hParent, GWLP_HINSTANCE)),
			MAKEINTRESOURCEW(this->setup.dialogId), hParent, _dialog_proc,
			reinterpret_cast<LPARAM>(this));

		if (!ret) {
			throw std::invalid_argument("DialogBoxParam failed for modal dialog, invalid parent.");
		} else if (ret == -1) {
			throw std::system_error(GetLastError(), std::system_category(),
				"DialogBoxParam failed for modal dialog");
		}
		return static_cast<int>(ret); // value passed to EndDialog()
	}

	// Shows the modal dialog, returning only after the modal is closed.
	int show(const hwnd_base* parent) {
		return this->show(parent->hwnd());
	}

protected:
	// Centers the modal dialog onto its parent.
	void center_on_parent() const noexcept {
		RECT rc{}, rcParent{};
		GetWindowRect(this->hwnd(), &rc);
		GetWindowRect(GetParent(this->hwnd()), &rcParent); // both relative to screen
		SetWindowPos(this->hwnd(), nullptr,
			rcParent.left + (rcParent.right - rcParent.left)/2 - (rc.right - rc.left)/2,
			rcParent.top + (rcParent.bottom - rcParent.top)/2 - (rc.bottom - rc.top)/2,
			0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}
};

}//namespace wl
