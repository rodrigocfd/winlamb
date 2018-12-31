/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "internals/base_msg_impl.h"
#include "internals/base_thread_impl.h"
#include "internals/base_user_ctrl.h"
#include "internals/base_window.h"
#include "internals/styler.h"
#include "wnd.h"

namespace wl {

// Inherit from this class to have an user-custom window control.
class window_control :
	public wnd,
	public wli::base_msg_impl<LRESULT>,
	public wli::base_thread_impl<LRESULT, 0>
{
private:
	HWND                            _hWnd = nullptr;
	wli::base_msg<LRESULT>          _baseMsg{_hWnd};
	wli::base_thread<LRESULT, 0>    _baseThread{_baseMsg};
	wli::base_window                _baseWindow{_hWnd, _baseMsg};
	wli::base_user_ctrl<LRESULT, 0> _baseUserCtrl{_baseMsg};

public:
	// Defines window creation parameters.
	wli::base_window::setup_vars setup;

	// Wraps window style changes done by Get/SetWindowLongPtr.
	wli::styler<window_control> style{this};

protected:
	window_control() :
		wnd(_hWnd), base_msg_impl(_baseMsg), base_thread_impl(_baseThread)
	{
		this->_init_setup_styles();
	}

public:
	window_control(window_control&&) = default;
	window_control& operator=(window_control&&) = default; // movable only

	// Returns the control ID.
	int ctrl_id() const noexcept {
		return GetDlgCtrlID(this->_hWnd);
	}

	// Creates the control window, returning immediately.
	void create(HWND hParent, int ctrlId, POINT position, SIZE size) {
		this->setup.position = position;
		this->setup.size = size;
		this->setup.menu = reinterpret_cast<HMENU>(static_cast<INT_PTR>(ctrlId));
		this->_baseWindow.register_create(this->setup, hParent);
	}

	// Creates the control window, returning immediately.
	void create(const wnd* parent, int ctrlId, POINT position, SIZE size) {
		this->create(parent->hwnd(), ctrlId, position, size);
	}

private:
	void _init_setup_styles() noexcept {
		this->setup.wndClassEx.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
		this->setup.wndClassEx.style = wc::DBLCLKS;
		this->setup.style = ws::CHILD | ws::VISIBLE | ws::CLIPCHILDREN | ws::CLIPSIBLINGS;

		// Useful styles to be added by user:
		// WS_TABSTOP will receive focus on Tab key rotation
		// WS_HSCROLL adds horizontal scrollbar
		// WS_VSCROLL adds vertical scrollbar
		// WS_EX_CLIENTEDGE adds border (extended style, add on exStyle)
	}
};

}//namespace wl
