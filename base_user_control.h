/**
 * Part of WinLamb - Win32 API Lambda Library
 * @author Rodrigo Cesar de Freitas Dias
 * @see https://github.com/rodrigocfd/winlamb
 */

#pragma once
#include "base_inventory.h"
#include <VsStyle.h>
#include <Uxtheme.h>
#pragma comment(lib, "UxTheme.lib")

/**
 * base_wnd <-- base_inventory <-- base_user_control
 */

namespace wl {
namespace base {

	class user_control : virtual public inventory {
	protected:
		user_control() {
			this->on_message(WM_NCPAINT, [&](params p)->LONG_PTR {
				this->_paint_themed_borders(p);
				return this->inventory::_procHandled(p);
			});
		}

	private:
		LONG_PTR _paint_themed_borders(const params& p) const {
			LONG_PTR defRet = DefWindowProcW(this->hwnd(), WM_NCPAINT, p.wParam, p.lParam); // will make system draw the scrollbar for us

			if ((GetWindowLongPtrW(this->hwnd(), GWL_EXSTYLE) & WS_EX_CLIENTEDGE) && IsThemeActive() && IsAppThemed()) {
				RECT rc = { 0 };
				GetWindowRect(this->hwnd(), &rc); // window outmost coordinates, including margins
				ScreenToClient(this->hwnd(), reinterpret_cast<POINT*>(&rc));
				ScreenToClient(this->hwnd(), reinterpret_cast<POINT*>(&rc.right));
				rc.left += 2; rc.top += 2; rc.right += 2; rc.bottom += 2; // because it comes up anchored at -2,-2

				RECT rc2 = { 0 }; // clipping region; will draw only within this rectangle
				HDC hdc = GetWindowDC(this->hwnd());
				HTHEME hTheme = OpenThemeData(this->hwnd(), L"LISTVIEW"); // borrow style from listview

				SetRect(&rc2, rc.left, rc.top, rc.left + 2, rc.bottom); // draw only the borders to avoid flickering
				DrawThemeBackground(hTheme, hdc, LVP_LISTGROUP, 0, &rc, &rc2); // draw themed left border
				SetRect(&rc2, rc.left, rc.top, rc.right, rc.top + 2);
				DrawThemeBackground(hTheme, hdc, LVP_LISTGROUP, 0, &rc, &rc2); // draw themed top border
				SetRect(&rc2, rc.right - 2, rc.top, rc.right, rc.bottom);
				DrawThemeBackground(hTheme, hdc, LVP_LISTGROUP, 0, &rc, &rc2); // draw themed right border
				SetRect(&rc2, rc.left, rc.bottom - 2, rc.right, rc.bottom);
				DrawThemeBackground(hTheme, hdc, LVP_LISTGROUP, 0, &rc, &rc2); // draw themed bottom border

				CloseThemeData(hTheme);
				ReleaseDC(this->hwnd(), hdc);
				return 0;
			}

			return defRet;
		}
	};

}//namespace base
}//namespace wl