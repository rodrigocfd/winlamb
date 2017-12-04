/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "hwnd_wrapper.h"

/**
 * hwnd_wrapper
 *  native_control
 */

namespace wl {
namespace wli {

template<typename derivedT>
class native_control : public hwnd_wrapper {
protected:
	native_control() = default;

public:
	native_control(const native_control&) = delete;
	native_control& operator=(const native_control&) = delete; // non-copyable, non-movable

	int control_id() const noexcept {
		return GetDlgCtrlID(this->hwnd());
	}

	derivedT& assign(HWND hCtrl) {
		if (this->hwnd()) {
			throw std::logic_error("Trying to assign or create a native control twice.");
		}
		this->_hWnd = hCtrl;
		return *static_cast<derivedT*>(this);
	}

	derivedT& assign(HWND hParent, int ctrlId) {
		return this->assign(GetDlgItem(hParent, ctrlId));
	}

	derivedT& assign(const hwnd_wrapper* parent, int ctrlId) {
		return this->assign(parent->hwnd(), ctrlId);
	}

	derivedT& create(HWND hParent, int ctrlId, const wchar_t* title,
		POINT pos, SIZE size, const wchar_t* className,
		DWORD styles = (WS_CHILD | WS_VISIBLE), DWORD exStyles = 0)
	{
		return this->assign(CreateWindowExW(exStyles, className, title, styles,
			pos.x, pos.y, size.cx, size.cy, hParent,
			reinterpret_cast<HMENU>(static_cast<UINT_PTR>(ctrlId)),
			reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(hParent, GWLP_HINSTANCE)),
			nullptr));
	}

	derivedT& create(const hwnd_wrapper* parent, int ctrlId, const wchar_t* title,
		POINT pos, SIZE size, const wchar_t* className,
		DWORD styles = (WS_CHILD | WS_VISIBLE), DWORD exStyles = 0)
	{
		return this->create(parent->hwnd(), ctrlId, title, pos, size, className, styles);
	}
};

}//namespace wli
}//namespace wl
