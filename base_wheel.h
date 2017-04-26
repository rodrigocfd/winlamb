/**
 * Part of WinLamb - Win32 API Lambda Library
 * @author Rodrigo Cesar de Freitas Dias
 * @see https://github.com/rodrigocfd/winlamb
 */

#pragma once
#include "base_wnd.h"
#include <CommCtrl.h>

/**
 * base_wnd <-- base_wheel
 */

namespace wl {

class window_main;

namespace base {

	class dialog;

	class wheel : virtual public wnd {
	protected:
		friend dialog;
		friend window_main;

		wheel() = default;
	private:
		void _apply_wheel_hover_behavior() const {
			EnumChildWindows(this->hwnd(), [](HWND hChild, LPARAM lp)->BOOL {
				static UINT_PTR uniqueSubclassId = 1;
				if (GetWindowLongPtrW(hChild, GWL_STYLE) & WS_TABSTOP) {
					SetWindowSubclass(hChild, _wheel_proc, uniqueSubclassId++,
						static_cast<DWORD_PTR>(lp)); // subclass every focusable control
				}
				return TRUE;
			}, reinterpret_cast<LPARAM>(this->hwnd()));
		}

		static LRESULT CALLBACK _wheel_proc(HWND hChild, UINT msg, WPARAM wp, LPARAM lp,
			UINT_PTR idSubclass, DWORD_PTR refData)
		{
			switch (msg) {
			case WM_MOUSEWHEEL:
				if (!(LOWORD(wp) & 0x0800)) { // bitflag not set, this is the first and unprocessed WM_MOUSEWHEEL passage
					HWND hTopLevelParent = reinterpret_cast<HWND>(refData);
					POINT pt = { LOWORD(lp), HIWORD(lp) };
					ScreenToClient(hTopLevelParent, &pt); // to client coordinates relative to parent
					SendMessageW(ChildWindowFromPoint(hTopLevelParent, pt), // window below cursor
						WM_MOUSEWHEEL,
						MAKEWPARAM(LOWORD(wp) | 0x0800, HIWORD(wp)), // set 0x0800 bitflag and kick to window below cursor
						lp);
					return 0; // halt processing
				} else { // bitflag is set, WM_MOUSEWHEEL has been kicked here and can be safely processed
					wp &= ~0x0800; // unset bitflag
					break; // finally dispatch to default processing
				}
			case WM_NCDESTROY:
				RemoveWindowSubclass(hChild, _wheel_proc, idSubclass); // http://blogs.msdn.com/b/oldnewthing/archive/2003/11/11/55653.aspx
			}
			return DefSubclassProc(hChild, msg, wp, lp);
		}
	};

}//namespace base
}//namespace wl