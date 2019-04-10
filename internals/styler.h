/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "styles_ws.h"
#include "styles_wsx.h"

namespace wl {
namespace _wli {

// Wraps window style changes done by Get/SetWindowLongPtr, and allows custom methods.
template<typename wndT>
class styler {
public:
	wndT& target;

	explicit styler(wndT* targetWnd) noexcept : target(*targetWnd) { }

	styler(styler&&) = default;
	styler& operator=(styler&&) = default; // movable only

	// Sets an ordinary window style, wraps SetWindowLongPtr.
	wndT& set_style(bool addStyle, DWORD styleFlags) noexcept {
		return this->_change_style_flags(false, addStyle, styleFlags);
	}

	// Sets an ordinary window style, wraps SetWindowLongPtr.
	wndT& set_style(bool addStyle, ws styleFlags) noexcept {
		return this->_change_style_flags(false, addStyle, static_cast<DWORD>(styleFlags));
	}

	// Sets an extended window style, wraps SetWindowLongPtr.
	wndT& set_style_ex(bool addStyle, DWORD styleFlags) noexcept {
		return this->_change_style_flags(true, addStyle, styleFlags);
	}

	// Sets an extended window style, wraps SetWindowLongPtr.
	wndT& set_style_ex(bool addStyle, wsx styleFlags) noexcept {
		return this->_change_style_flags(true, addStyle, static_cast<DWORD>(styleFlags));
	}

	// Wraps GetWindowLongPtr to retrieve an ordinary window style.
	bool has_style(DWORD styleFlags) const noexcept {
		return (GetWindowLongPtrW(this->target.hwnd(), GWL_STYLE) & styleFlags) != 0;
	}

	// Wraps GetWindowLongPtr to retrieve an ordinary window style.
	bool has_style(ws styleFlags) const noexcept {
		return (GetWindowLongPtrW(this->target.hwnd(), GWL_STYLE) & static_cast<DWORD>(styleFlags)) != 0;
	}

	// Wraps GetWindowLongPtr to retrieve an extended window style.
	bool has_style_ex(DWORD styleFlags) const noexcept {
		return (GetWindowLongPtrW(this->target.hwnd(), GWL_EXSTYLE) & styleFlags) != 0;
	}

	// Wraps GetWindowLongPtr to retrieve an extended window style.
	bool has_style_ex(wsx styleFlags) const noexcept {
		return (GetWindowLongPtrW(this->target.hwnd(), GWL_EXSTYLE) & static_cast<DWORD>(styleFlags)) != 0;
	}

private:
	wndT& _change_style_flags(bool isEx, bool addStyle, DWORD styleFlags) noexcept {
		LONG_PTR curFlags = GetWindowLongPtrW(this->target.hwnd(), isEx ? GWL_EXSTYLE : GWL_STYLE);
		if (addStyle) {
			curFlags |= static_cast<LONG_PTR>(styleFlags);
		} else {
			curFlags &= ~static_cast<LONG_PTR>(styleFlags);
		}
		SetWindowLongPtrW(this->target.hwnd(), isEx ? GWL_EXSTYLE : GWL_STYLE, curFlags);
		return this->target;
	}
};

}//namepace _wli
}//namespace wl
