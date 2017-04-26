/**
 * Part of WinLamb - Win32 API Lambda Library
 * @author Rodrigo Cesar de Freitas Dias
 * @see https://github.com/rodrigocfd/winlamb
 */

#pragma once
#include <vector>
#include "base_wnd.h"

/**
 * base_wnd <-- base_loop
 */

namespace wl {

class dialog_modeless;
class dialog_main;
class window_main;

namespace base {

	class loop : virtual public wnd {
	private:
		friend dialog_modeless;
		friend dialog_main;
		friend window_main;

		std::vector<HWND> _modelessChildren;
	protected:
		loop() = default;

	private:
		int _msg_loop(HACCEL hAccel = nullptr) const {
			MSG  msg = { 0 };
			BOOL ret = 0;
			while ((ret = GetMessageW(&msg, nullptr, 0, 0)) != 0) {
				if (ret == -1) return -1;
				if (this->_is_modeless_msg(&msg) ||
					(hAccel && TranslateAcceleratorW(this->hwnd(), hAccel, &msg)) ||
					IsDialogMessageW(this->hwnd(), &msg) ) continue;
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
			return static_cast<int>(msg.wParam); // this can be used as program return value
		}

		void _add_modeless(HWND hWnd) {
			this->_modelessChildren.emplace_back(hWnd);
		}

		void _remove_modeless(HWND hWnd) {
			for (auto it = this->_modelessChildren.begin();
				it != this->_modelessChildren.end(); ++it)
			{
				if (*it == this->hwnd()) {
					this->_modelessChildren.erase(it);
					break;
				}
			}
		}

		bool _is_modeless_msg(MSG* pMsg) const {
			for (const HWND hModl : this->_modelessChildren) {
				if (!hModl || !IsWindow(hModl)) continue; // skip invalid HWND
				if (IsDialogMessageW(hModl, pMsg)) return true;
			}
			return false;
		}
	};

}//namespace base
}//namespace wl