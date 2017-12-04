/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "internals/ui_thread.h"
#include "internals/user_control.h"
#include "styler.h"

/**
 * hwnd_wrapper
 *  inventory
 *   ui_thread
 *    [user_control]
 *     window
 */

namespace wl {
class window_control; // friend forward declarations
class window_main;

namespace wli {

template<typename baseT>
class window : public baseT {
	friend class window_control;
	friend class window_main;

private:
	class _styler final : public wli::styler<window> {
	public:
		explicit _styler(window* pWindow) noexcept : styler(pWindow) { }
	};

protected:
	struct setup_vars {
		WNDCLASSEX     wndClassEx{};
		const wchar_t* title = nullptr;
		DWORD          style = 0, exStyle = 0;
		POINT          position{};
		SIZE           size{};
		HMENU          menu = nullptr;
	};

public:
	~window() {
		if (this->hwnd()) {
			SetWindowLongPtrW(this->hwnd(), GWLP_USERDATA, 0);
		}
	}

	window(const window&) = delete;
	window& operator=(const window&) = delete; // non-copyable, non-movable

protected:
	_styler style{this};

	window() = default;

private:
	void _basic_initial_checks(const setup_vars& setup) const {
		if (this->hwnd()) {
			throw std::logic_error("Tried to create window twice.");
		}
		if (!setup.wndClassEx.lpszClassName) {
			throw std::logic_error("No window class name given on this->setup.wndClassEx.lpszClassName.");
		}
	}

	void _register_create(setup_vars& setup, HWND hParent, HINSTANCE hInst = nullptr) {
		this->_basic_initial_checks(setup);
		if (!hParent && !hInst) {
			throw std::invalid_argument("To create a window, HINSTANCE or parent HWND must be provided.");
		}
		if (!hInst) {
			hInst = reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(hParent, GWLP_HINSTANCE));
		}

		setup.wndClassEx.cbSize = sizeof(WNDCLASSEX); // make sure of these
		setup.wndClassEx.lpfnWndProc = _window_proc;
		setup.wndClassEx.hInstance = hInst;

		ATOM atom = this->_register_class(setup, hInst);

		if (!CreateWindowExW(setup.exStyle,
			MAKEINTATOM(atom), setup.title, setup.style,
			setup.position.x, setup.position.y, setup.size.cx, setup.size.cy,
			hParent, setup.menu, hInst, static_cast<LPVOID>(this)) )
		{
			throw std::system_error(GetLastError(), std::system_category(),
				"CreateWindowEx failed");
		}
	}

	ATOM _register_class(setup_vars& setup, HINSTANCE hInst) {
		ATOM atom = RegisterClassExW(&setup.wndClassEx);
		if (!atom) {
			DWORD errCode = GetLastError();
			if (errCode == ERROR_CLASS_ALREADY_EXISTS) {
				atom = static_cast<ATOM>(GetClassInfoExW(hInst,
					setup.wndClassEx.lpszClassName,
					&setup.wndClassEx)); // https://blogs.msdn.microsoft.com/oldnewthing/20041011-00/?p=37603
			} else {
				throw std::system_error(errCode, std::system_category(),
					"RegisterClassEx failed");
			}
		}
		return atom;
	}

	static LRESULT CALLBACK _window_proc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) noexcept {
		window* pSelf = nullptr;

		if (msg == WM_NCCREATE) {
			pSelf = reinterpret_cast<window*>(reinterpret_cast<CREATESTRUCT*>(lp)->lpCreateParams);
			SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pSelf));
			pSelf->_hWnd = hWnd; // store HWND
		} else {
			pSelf = reinterpret_cast<window*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
		}

		auto cleanupIfDestroyed = [&]() noexcept->void {
			if (msg == WM_NCDESTROY) {
				SetWindowLongPtrW(hWnd, GWLP_USERDATA, 0);
				if (pSelf) {
					pSelf->_hWnd = nullptr; // clear HWND
				}
			}
		};

		if (pSelf) {
			std::pair<bool, LRESULT> procRet = pSelf->_process_msg(params{msg, wp, lp}); // catches all message exceptions internally
			if (procRet.first) {
				cleanupIfDestroyed();
				return procRet.second; // message was processed
			}
		}

		cleanupIfDestroyed();
		return DefWindowProcW(hWnd, msg, wp, lp); // message was not processed
	}
};

using window_ui_thread = window<ui_thread<LRESULT, 0>>;
using window_user_control = window<user_control<LRESULT, 0>>;

}//namespace wli
}//namespace wl
