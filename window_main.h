/**
 * Part of WinLamb - Win32 API Lambda Library
 * @author Rodrigo Cesar de Freitas Dias
 * @see https://github.com/rodrigocfd/winlamb
 */

#pragma once
#include "base_window.h"
#include "base_loop.h"
#include "base_wheel.h"
#include "base_run.h"

/**
 *                                   +----- base_msgs <-- msg_[any] <------+
 *             +-- base_inventory <--+                                     |
 *             |                     +-- base_window <--+                  +-- [user]
 *             |                                        |                  |
 * base_wnd <--+-------------- base_loop <--------------+                  |
 *             |                                        +-- window_main <--+
 *             +-------------- base_wheel <-------------+
 */

namespace wl {

// Inherit from this class to have the main window of your application.
class window_main :
	public    base::window,
	protected base::loop,
	protected base::wheel
{
public:
	struct setup_vars final : public base::window::setup_vars {
		HACCEL accelTable;
		setup_vars() : accelTable(nullptr) { }
	};

protected:
	setup_vars setup;

	explicit window_main(size_t msgsReserve = 0) : window(msgsReserve + 1) {
		this->setup.wndClassEx.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
		this->setup.wndClassEx.style = CS_DBLCLKS;
		this->setup.position = { CW_USEDEFAULT, CW_USEDEFAULT };
		this->setup.size = { CW_USEDEFAULT, CW_USEDEFAULT };
		this->setup.style = WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN | WS_BORDER;

		// Useful styles to add:
		// WS_SIZEBOX resizable window
		// WS_MAXIMIZEBOX adds maximize button
		// WS_MINIMIZEBOX adds minimize button
		// WS_EX_ACCEPTFILES accepts dropped files (extended style, add on exStyle)

		this->on_message(WM_NCDESTROY, [](const params&)->LRESULT {
			PostQuitMessage(0);
			return 0;
		});
	}

public:
	int run(HINSTANCE hInst, int cmdShow) {
		InitCommonControls();
		if (!this->window::_register_create(this->setup, nullptr, hInst)) return -1;

		ShowWindow(this->hwnd(), cmdShow);
		UpdateWindow(this->hwnd());
		this->wheel::_apply_wheel_hover_behavior();
		return this->loop::_msg_loop(this->setup.accelTable); // this can be used as program return value
	}
};

}//namespace wl