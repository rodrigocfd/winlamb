/**
 * Part of WinLamb - Win32 API Lambda Library
 * @author Rodrigo Cesar de Freitas Dias
 * @see https://github.com/rodrigocfd/winlamb
 */

#pragma once
#include "base_inventory.h"

/**
 * base_wnd <-- base_inventory <-- base_window
 */

namespace wl {

class window_main;
class window_control;

namespace base {

	class window : virtual public inventory {
	public:
		friend window_main;
		friend window_control;

		struct setup_vars {
			WNDCLASSEX     wndClassEx;
			const wchar_t* title;
			DWORD          style, exStyle;
			POINT          position;
			SIZE           size;
			HMENU          menu;
			setup_vars() : wndClassEx({ 0 }), title(nullptr), style(0), exStyle(0),
				position({0,0}), size({0,0}), menu(nullptr) { }
		};

		~window() {
			if (this->hwnd()) {
				SetWindowLongPtrW(this->hwnd(), GWLP_USERDATA, 0);
			}
		}

	protected:
		explicit window(size_t msgsReserve) {
			this->inventory::_msgDepot.reserve(msgsReserve);

			this->inventory::_procHandled = [](params)->LRESULT {
				return 0;
			};
			this->inventory::_procUnhandled = [&](params p)->LRESULT {
				return DefWindowProcW(this->hwnd(), p.message, p.wParam, p.lParam);
			};
		}

	private:
		bool _register_create(setup_vars& setup, HWND hParent, HINSTANCE hInst = nullptr) {
			if (!this->_basic_initial_checks(setup)) return false;
			if (!hParent && !hInst) return false;
			if (!hInst) hInst = reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(hParent, GWLP_HINSTANCE));

			setup.wndClassEx.cbSize = sizeof(WNDCLASSEX); // make sure of these
			setup.wndClassEx.lpfnWndProc = _window_proc;
			setup.wndClassEx.hInstance = hInst;

			ATOM atom = this->_register_class(setup, hInst);
			if (!atom) return false;

			bool created = CreateWindowExW(setup.exStyle, MAKEINTATOM(atom),
				setup.title, setup.style,
				setup.position.x, setup.position.y,
				setup.size.cx, setup.size.cy,
				hParent, setup.menu, hInst,
				static_cast<LPVOID>(this)) != nullptr;

			if (!created) OutputDebugStringW(L"ERROR: CreateWindowEx failed.\n");
			return created;
		}

		bool _basic_initial_checks(const setup_vars& setup) const {
			if (this->hwnd()) {
				OutputDebugStringW(L"ERROR: tried to create window twice.\n");
				return false;
			}
			if (!setup.wndClassEx.lpszClassName) {
				OutputDebugStringW(L"ERROR: window not created, no class name given.\n");
				return false;
			}
			return true;
		}

		ATOM _register_class(setup_vars& setup, HINSTANCE hInst) {
			ATOM atom = RegisterClassExW(&setup.wndClassEx);
			if (!atom) {
				if (GetLastError() == ERROR_CLASS_ALREADY_EXISTS) {
					atom = static_cast<ATOM>(GetClassInfoExW(hInst,
						setup.wndClassEx.lpszClassName,
						&setup.wndClassEx)); // https://blogs.msdn.microsoft.com/oldnewthing/20041011-00/?p=37603
				} else {
					OutputDebugStringW(L"ERROR: window not created, failed to register class ATOM.\n");
					return 0;
				}
			}
			return atom;
		}

		static LRESULT CALLBACK _window_proc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
			window* pSelf = nullptr;

			if (msg == WM_NCCREATE) {
				pSelf = reinterpret_cast<window*>(reinterpret_cast<CREATESTRUCT*>(lp)->lpCreateParams);
				SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pSelf));
				pSelf->wnd::_hWnd = hWnd; // store HWND
			} else {
				pSelf = reinterpret_cast<window*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
			}

			auto cleanupIfDestroyed = [&]()->void {
				if (msg == WM_NCDESTROY) {
					SetWindowLongPtrW(hWnd, GWLP_USERDATA, 0);
					if (pSelf) {
						pSelf->wnd::_hWnd = nullptr; // clear HWND
					}
				}
			};

			if (pSelf) {
				inventory::msg_funcT* pFunc = pSelf->inventory::_msgDepot.find(msg);
				if (pFunc) {
					LRESULT ret = (*pFunc)(params{msg, wp, lp}); // call user lambda
					cleanupIfDestroyed();
					return ret;
				}
			}

			cleanupIfDestroyed();
			return DefWindowProcW(hWnd, msg, wp, lp);
		}
	};

}//namespace base
}//namespace wl