/**
 * Part of WinLamb - Win32 API Lambda Library
 * @author Rodrigo Cesar de Freitas Dias
 * @see https://github.com/rodrigocfd/winlamb
 */

#pragma once
#include "base_window.h"
#include "base_user_control.h"

/**
 *                                +--------- base_msgs <-- msg_[any] <-----------+
 *                                |                                              +-- [user]
 * base_wnd <-- base_inventory <--+----- base_window <-----+                     |
 *                                |                        +-- window_control <--+
 *                                +-- base_user_control <--+
 */

namespace wl {

// Inherit from this class to have an user-custom window control.
class window_control :
	public base::window,
	public base::user_control
{
protected:
	base::window::setup_vars setup;

	explicit window_control(size_t msgsReserve = 0) : window(msgsReserve) {
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
	bool create(HWND hParent, int controlId, POINT position, SIZE size) {
		this->setup.position = position;
		this->setup.size = size;
		this->setup.menu = reinterpret_cast<HMENU>(static_cast<INT_PTR>(controlId));
		return this->window::_register_create(this->setup, hParent);
	}

	bool create(const base::wnd* parent, int controlId, POINT position, SIZE size) {
		return this->create(parent->hwnd(), controlId, position, size);
	}
};

}//namespace wl