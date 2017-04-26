/**
 * Part of WinLamb - Win32 API Lambda Library
 * @author Rodrigo Cesar de Freitas Dias
 * @see https://github.com/rodrigocfd/winlamb
 */

#pragma once
#include "base_wnd.h"

/**
 * base_wnd <-- native_control
 */

namespace wl {

// Generic class to any native Windows control.
class native_control : virtual public base::wnd {
public:
	bool assign(HWND hCtrl) {
		this->wnd::_hWnd = hCtrl;
		return this->hwnd() != nullptr;
	}

	bool assign(HWND hParent, int controlId) {
		return this->assign(GetDlgItem(hParent, controlId));
	}

	bool assign(const base::wnd* parent, int controlId) {
		return this->assign(parent->hwnd(), controlId);
	}

	bool create(HWND hParent, int controlId, const wchar_t* title,
		POINT pos, SIZE size, const wchar_t* className,
		DWORD styles = (WS_CHILD | WS_VISIBLE), DWORD exStyles = 0)
	{
		if (this->hwnd()) {
			OutputDebugStringW(L"ERROR: native control already created.\n");
			return false;
		}

		this->wnd::_hWnd = CreateWindowExW(exStyles, className, title, styles,
			pos.x, pos.y, size.cx, size.cy, hParent,
			reinterpret_cast<HMENU>(static_cast<UINT_PTR>(controlId)),
			reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(hParent, GWLP_HINSTANCE)),
			nullptr);

		return this->hwnd() != nullptr;
	}

	bool create(const wnd* parent, int controlId, const wchar_t* title,
		POINT pos, SIZE size, const wchar_t* className,
		DWORD styles = (WS_CHILD | WS_VISIBLE), DWORD exStyles = 0)
	{
		return this->create(parent->hwnd(), controlId, title, pos, size, className, styles);
	}

public:
	int control_id() const {
		return GetDlgCtrlID(this->hwnd());
	}
};

}//namespace wl