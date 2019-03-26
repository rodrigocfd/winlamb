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
#include "internals/run.h"
#include "internals/styler.h"
#include "wnd.h"

namespace wl {
namespace wli { class dialog_modeless; } // friend forward declaration

// Inherit from this class to have a dialog as the main window for your application.
class dialog_main :
	public wnd,
	public wli::base_msg_pubm<INT_PTR>,
	public wli::base_thread_pubm<INT_PTR, TRUE>,
	public wli::base_text_pubm<dialog_main>
{
	friend wli::dialog_modeless; // needs to access _baseLoop

protected:
	// Variables to be set by user, used only during window creation.
	struct setup_vars final : public wli::base_dialog::setup_vars {
		int iconId = 0;
		int accelTableId = 0;
	};

private:
	HWND                            _hWnd = nullptr;
	wli::base_msg<INT_PTR>          _baseMsg{_hWnd};
	wli::base_thread<INT_PTR, TRUE> _baseThread{_baseMsg};
	wli::base_dialog                _baseDialog{_hWnd, _baseMsg};
	wli::base_loop                  _baseLoop;

public:
	// Defines window creation parameters.
	setup_vars setup;

	// Wraps window style changes done by Get/SetWindowLongPtr.
	wli::styler<dialog_main> style{this};

protected:
	dialog_main() :
		wnd(_hWnd), base_msg_pubm(_baseMsg), base_thread_pubm(_baseThread), base_text_pubm(_hWnd)
	{
		this->base_msg_pubm::on_message(WM_CLOSE, [this](params) noexcept -> INT_PTR {
			DestroyWindow(this->_hWnd);
			return TRUE;
		});
		this->base_msg_pubm::on_message(WM_NCDESTROY, [](params) noexcept -> INT_PTR {
			PostQuitMessage(0);
			return TRUE;
		});
	}

public:
	dialog_main(dialog_main&&) = default;
	dialog_main& operator=(dialog_main&&) = default; // movable only

	// Runs the dialog as the main program window; intended to be called in WinMain.
	int winmain_run(HINSTANCE hInst, int cmdShow) {
		InitCommonControls();

		if (!this->_baseDialog.create_dialog_param(this->setup, nullptr)) {
			throw std::system_error(GetLastError(), std::system_category(),
				"CreateDialogParam failed for main dialog");
		}

		HACCEL hAccel = nullptr;
		if (this->setup.accelTableId) {
			hAccel = LoadAcceleratorsW(hInst, MAKEINTRESOURCEW(this->setup.accelTableId));
			if (!hAccel) {
				throw std::system_error(GetLastError(), std::system_category(),
					"LoadAccelerators failed for main dialog");
			}
		}

		this->_set_icon(hInst);
		ShowWindow(this->_hWnd, cmdShow);
		return this->_baseLoop.run_loop(this->_hWnd, hAccel); // can be used as program return value
	}

private:
	void _set_icon(HINSTANCE hInst) const noexcept {
		if (this->setup.iconId) {
			SendMessageW(this->_hWnd, WM_SETICON, ICON_SMALL,
				reinterpret_cast<LPARAM>(reinterpret_cast<HICON>(LoadImageW(hInst,
					MAKEINTRESOURCEW(this->setup.iconId),
					IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR))));
			SendMessageW(this->_hWnd, WM_SETICON, ICON_BIG,
				reinterpret_cast<LPARAM>(reinterpret_cast<HICON>(LoadImageW(hInst,
					MAKEINTRESOURCEW(this->setup.iconId),
					IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR))));
		}
	}
};

}//namespace wl
