/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "internals/window.h"
#include "internals/hover_scroll.h"
#include "internals/loop.h"
#include "internals/has_text.h"
#include "internals/run.h"

/**
 * hwnd_wrapper
 *  inventory
 *   ui_thread
 *    window
 *     has_text
 *      window_main
 */

namespace wl {
namespace wli { class dialog_modeless; } // friend forward declaration

// Inherit from this class to have an ordinary main window for your application.
class window_main :
	public wli::has_text<
		window_main, wli::window_ui_thread>
{
	friend class dialog_modeless;

protected:
	struct setup_vars final : public wli::window_ui_thread::setup_vars {
		HACCEL accelTable = nullptr;
	};

private:
	wli::loop _loop;

protected:
	setup_vars setup;

	window_main() {
		this->setup.wndClassEx.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
		this->setup.wndClassEx.style = CS_DBLCLKS;
		this->setup.position = {CW_USEDEFAULT, CW_USEDEFAULT};
		this->setup.size = {CW_USEDEFAULT, CW_USEDEFAULT};
		this->setup.style = WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN | WS_BORDER;

		// Useful styles to add:
		// WS_SIZEBOX resizable window
		// WS_MAXIMIZEBOX adds maximize button
		// WS_MINIMIZEBOX adds minimize button
		// WS_EX_ACCEPTFILES accepts dropped files (extended style, add on exStyle)

		this->on_message(WM_NCDESTROY, [](params)->LRESULT {
			PostQuitMessage(0);
			return 0;
		});
	}

public:
	int winmain_run(HINSTANCE hInst, int cmdShow) {
		InitCommonControls();
		this->_register_create(this->setup, nullptr, hInst);
		ShowWindow(this->hwnd(), cmdShow);
		if (!UpdateWindow(this->hwnd())) {
			throw std::system_error(GetLastError(), std::system_category(),
				"UpdateWindow failed");
		}
		wli::hover_scroll::apply_behavior(this->hwnd());
		return this->_loop.run_loop(this->hwnd(), this->setup.accelTable); // can be used as program return value
	}
};

}//namespace wl
