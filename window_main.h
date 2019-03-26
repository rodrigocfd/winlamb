/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "internals/base_loop.h"
#include "internals/base_msg_pubm.h"
#include "internals/base_scroll.h"
#include "internals/base_text_pubm.h"
#include "internals/base_thread_pubm.h"
#include "internals/base_window.h"
#include "internals/run.h"
#include "internals/styler.h"
#include "wnd.h"

namespace wl {
namespace wli { class dialog_modeless; } // friend forward declaration

// Inherit from this class to have an ordinary main window for your application.
class window_main :
	public wnd,
	public wli::base_msg_pubm<LRESULT>,
	public wli::base_thread_pubm<LRESULT, 0>,
	public wli::base_text_pubm<window_main>
{
	friend wli::dialog_modeless; // needs to access _baseLoop

protected:
	// Variables to be set by user, used only during window creation.
	struct setup_vars final : public wli::base_window::setup_vars {
		HACCEL accelTable = nullptr;
	};

private:
	HWND                         _hWnd = nullptr;
	wli::base_msg<LRESULT>       _baseMsg{_hWnd};
	wli::base_thread<LRESULT, 0> _baseThread{_baseMsg};
	wli::base_window             _baseWindow{_hWnd, _baseMsg};
	wli::base_loop               _baseLoop;

public:
	// Defines window creation parameters.
	setup_vars setup;

	// Wraps window style changes done by Get/SetWindowLongPtr.
	wli::styler<window_main> style{this};

protected:
	window_main() :
		wnd(_hWnd), base_msg_pubm(_baseMsg), base_thread_pubm(_baseThread), base_text_pubm(_hWnd)
	{
		this->_init_setup_styles();

		this->base_msg_pubm::on_message(WM_NCDESTROY, [](params) noexcept -> LRESULT {
			PostQuitMessage(0);
			return 0;
		});
	}

public:
	window_main(window_main&&) = default;
	window_main& operator=(window_main&&) = default; // movable only

	// Runs the window as the main program window; intended to be called in WinMain.
	int winmain_run(HINSTANCE hInst, int cmdShow) {
		InitCommonControls();
		this->_baseWindow.register_create(this->setup, nullptr, hInst);
		ShowWindow(this->hwnd(), cmdShow);
		if (!UpdateWindow(this->hwnd())) {
			throw std::system_error(GetLastError(), std::system_category(),
				"UpdateWindow failed");
		}
		wli::base_scroll::apply_behavior(this->hwnd());
		return this->_baseLoop.run_loop(this->hwnd(), this->setup.accelTable); // can be used as program return value
	}

private:
	void _init_setup_styles() noexcept {
		this->setup.wndClassEx.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
		this->setup.wndClassEx.style = wc::DBLCLKS;
		this->setup.position = {CW_USEDEFAULT, CW_USEDEFAULT};
		this->setup.size = {CW_USEDEFAULT, CW_USEDEFAULT};
		this->setup.style = ws::CAPTION | ws::SYSMENU | ws::CLIPCHILDREN | ws::BORDER;

		// Useful styles to be added by user:
		// WS_SIZEBOX resizable window
		// WS_MAXIMIZEBOX adds maximize button
		// WS_MINIMIZEBOX adds minimize button
		// WS_EX_ACCEPTFILES accepts dropped files (extended style, add on exStyle)
	}
};

}//namespace wl
