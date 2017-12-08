/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "internals/dialog.h"
#include "internals/loop.h"
#include "internals/has_text.h"
#include "internals/run.h"

/**
 * hwnd_base
 *  inventory
 *   ui_thread
 *    dialog
 *     has_text
 *      dialog_main
 */

namespace wl {
namespace wli { class dialog_modeless; } // friend forward declaration

// Inherit from this class to have a dialog as the main window for your application.
class dialog_main :
	public wli::has_text<
		dialog_main, wli::dialog_ui_thread>
{
	friend class dialog_modeless;

protected:
	struct setup_vars final : public wli::dialog_ui_thread::setup_vars {
		int iconId = 0;
		int accelTableId = 0;
	};

private:
	wli::loop _loop;

protected:
	setup_vars setup;

	dialog_main() noexcept {
		this->on_message(WM_CLOSE, [this](params) noexcept->INT_PTR {
			DestroyWindow(this->hwnd());
			return TRUE;
		});
		this->on_message(WM_NCDESTROY, [](params) noexcept->INT_PTR {
			PostQuitMessage(0);
			return TRUE;
		});
	}

public:
	int winmain_run(HINSTANCE hInst, int cmdShow) {
		InitCommonControls();
		this->_basic_initial_checks(this->setup);

		if (!CreateDialogParamW(hInst, MAKEINTRESOURCEW(this->setup.dialogId),
			nullptr, _dialog_proc, reinterpret_cast<LPARAM>(this)) )
		{
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
		ShowWindow(this->hwnd(), cmdShow);
		return this->_loop.run_loop(this->hwnd(), hAccel); // can be used as program return value
	}

private:
	void _set_icon(HINSTANCE hInst) const noexcept {
		if (this->setup.iconId) {
			SendMessageW(this->hwnd(), WM_SETICON, ICON_SMALL,
				reinterpret_cast<LPARAM>(reinterpret_cast<HICON>(LoadImageW(hInst,
					MAKEINTRESOURCEW(this->setup.iconId),
					IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR))));
			SendMessageW(this->hwnd(), WM_SETICON, ICON_BIG,
				reinterpret_cast<LPARAM>(reinterpret_cast<HICON>(LoadImageW(hInst,
					MAKEINTRESOURCEW(this->setup.iconId),
					IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR))));
		}
	}
};

}//namespace wl
