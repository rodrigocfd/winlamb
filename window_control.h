/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "internals/window.h"

/**
 * hwnd_base
 *  w_inventory
 *   w_thread_capable
 *    w_user_control
 *     window
 *      window_control
 */

namespace wl {

// Inherit from this class to have an user-custom window control.
class window_control : public wli::window_user_control {
protected:
	struct setup_vars final : public wli::window_user_control::setup_vars { };

	setup_vars setup;

	window_control() noexcept {
		this->setup.wndClassEx.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
		this->setup.wndClassEx.style = CS_DBLCLKS;
		this->setup.style = CS_DBLCLKS | WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

		// Useful styles to add:
		// WS_TABSTOP will receive focus on Tab key rotation
		// WS_HSCROLL adds horizontal scrollbar
		// WS_VSCROLL adds vertical scrollbar
		// WS_EX_CLIENTEDGE adds border (extended style, add on exStyle)
	}

public:
	void create(HWND hParent, int ctrlId, POINT position, SIZE size) {
		this->setup.position = position;
		this->setup.size = size;
		this->setup.menu = reinterpret_cast<HMENU>(static_cast<INT_PTR>(ctrlId));
		this->_register_create(this->setup, hParent);
	}

	void create(const hwnd_base* parent, int ctrlId, POINT position, SIZE size) {
		this->create(parent->hwnd(), ctrlId, position, size);
	}
};

}//namespace wl
