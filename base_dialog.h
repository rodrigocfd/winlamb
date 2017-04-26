/**
 * Part of WinLamb - Win32 API Lambda Library
 * @author Rodrigo Cesar de Freitas Dias
 * @see https://github.com/rodrigocfd/winlamb
 */

#pragma once
#include "base_inventory.h"
#include "base_wheel.h"
#include "font.h"

/**
 *             +-- base_inventory <--+
 * base_wnd <--+                     +-- base_dialog
 *             +---- base_wheel <----+
 */

namespace wl {

class dialog_main;
class dialog_modal;
class dialog_modeless;
class dialog_control;

namespace base {

	class dialog :
		virtual public inventory,
		protected wheel
	{
	public:
		friend dialog_main;
		friend dialog_modal;
		friend dialog_modeless;
		friend dialog_control;

		struct setup_vars {
			int dialogId;
			setup_vars() : dialogId(0) { }
		};

		~dialog() {
			if (this->hwnd()) {
				SetWindowLongPtrW(this->hwnd(), GWLP_USERDATA, 0);
			}
		}

	protected:
		explicit dialog(size_t msgsReserve) {
			this->inventory::_msgDepot.reserve(msgsReserve);

			this->inventory::_procHandled = [](const params&)->INT_PTR {
				return TRUE;
			};
			this->inventory::_procUnhandled = [](const params&)->INT_PTR {
				return FALSE;
			};
		}

	private:
		bool _basic_initial_checks(const setup_vars& setup) const {
			if (this->hwnd()) {
				OutputDebugStringW(L"ERROR: tried to create dialog twice.\n");
				return false;
			}
			if (!setup.dialogId) {
				OutputDebugStringW(L"ERROR: dialog not created, no resource ID given.\n");
				return false;
			}
			return true;
		}

		static INT_PTR CALLBACK _dialog_proc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp) {
			dialog* pSelf = nullptr;
			INT_PTR ret = FALSE;

			if (msg == WM_INITDIALOG) {
				pSelf = reinterpret_cast<dialog*>(lp);
				SetWindowLongPtrW(hDlg, DWLP_USER, reinterpret_cast<LONG_PTR>(pSelf));
				font::set_ui_on_children(hDlg); // if user creates controls manually, font must be set manually on them
				pSelf->wnd::_hWnd = hDlg; // store HWND
			} else {
				pSelf = reinterpret_cast<dialog*>(GetWindowLongPtrW(hDlg, DWLP_USER));
			}

			if (pSelf) {
				inventory::msg_funcT* pFunc = pSelf->inventory::_msgDepot.find(msg);
				if (pFunc) ret = (*pFunc)(params{msg, wp, lp}); // call user lambda
			}

			if (msg == WM_INITDIALOG) {
				pSelf->wheel::_apply_wheel_hover_behavior();
			} else if (msg == WM_NCDESTROY) { // cleanup
				SetWindowLongPtrW(hDlg, DWLP_USER, 0);
				if (pSelf) {
					pSelf->wnd::_hWnd = nullptr; // clear HWND
				}
			}

			return ret;
		}
	};

}//namespace base
}//namespace wl