/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "internals/base_dialog.h"
#include "internals/base_loop.h"
#include "internals/base_msg_pubm.h"
#include "internals/base_text_pubm.h"
#include "internals/base_thread_pubm.h"
#include "internals/styler.h"
#include "wnd.h"

namespace wl {

// Inherit from this class to have a dialog modeless popup.
class dialog_modeless :
	public wnd,
	public _wli::base_msg_pubm<INT_PTR>,
	public _wli::base_thread_pubm<INT_PTR, TRUE>,
	public _wli::base_text_pubm<dialog_modeless>
{
private:
	HWND                             _hWnd = nullptr;
	_wli::base_msg<INT_PTR>          _baseMsg{_hWnd};
	_wli::base_thread<INT_PTR, TRUE> _baseThread{_baseMsg};
	_wli::base_dialog                _baseDialog{_hWnd, _baseMsg};
	_wli::base_loop*                 _pParentBaseLoop = nullptr;

public:
	// Defines window creation parameters.
	_wli::base_dialog::setup_vars setup;

	// Wraps window style changes done by Get/SetWindowLongPtr.
	_wli::styler<dialog_modeless> style{this};

protected:
	dialog_modeless() :
		wnd(_hWnd), base_msg_pubm(_baseMsg), base_thread_pubm(_baseThread), base_text_pubm(_hWnd)
	{
		this->base_msg_pubm::on_message(WM_CLOSE, [this](params) noexcept -> INT_PTR {
			DestroyWindow(this->_hWnd);
			return TRUE;
		});
		this->base_msg_pubm::on_message(WM_NCDESTROY, [this](params) -> INT_PTR {
			if (this->_pParentBaseLoop) {
				this->_pParentBaseLoop->remove_modeless(this->_hWnd);
			}
			return TRUE;
		});
	}

public:
	dialog_modeless(dialog_modeless&&) = default;
	dialog_modeless& operator=(dialog_modeless&&) = default; // movable only

	// Creates the modeless dialog, returning immediately.
	template<typename parentT>
	void create(parentT* parent) {
		if (!this->_baseDialog.create_dialog_param(this->setup, parent->hwnd())) {
			throw std::system_error(GetLastError(), std::system_category(),
				"CreateDialogParam failed for modeless dialog");
		}

		this->_pParentBaseLoop = parent->_baseLoop; // parent must have us as friend
		this->_pParentBaseLoop->add_modeless(this->_hWnd);
		ShowWindow(this->_hWnd, SW_SHOW);
	}

	// Creates the modeless dialog, returning immediately.
	template<typename parentT>
	void create(parentT* parent, POINT clientPos) {
		this->create(parent);
		POINT parentPos = clientPos;
		ClientToScreen(parent->hwnd(), &parentPos); // now relative to parent
		SetWindowPos(this->_hWnd, nullptr,
			parentPos.x, parentPos.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}
};

}//namespace wl
