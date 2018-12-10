/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <Windows.h>

namespace wl {
namespace wli {

// Wraps window style changes with Get/SetWindowLongPtr, and allows custom methods.
template<typename wndT>
class styler {
private:
	wndT& _wnd;

protected:
	explicit styler(wndT* target) noexcept : _wnd(*target) { }

public:
	styler(const styler&) = delete;
	styler& operator=(const styler&) = delete; // non-copyable, non-movable

protected:
	HWND  hwnd() const noexcept   { return this->_wnd.hwnd(); }
	wndT& target() const noexcept { return this->_wnd; }

public:
	// Sets an ordinary window style, wraps SetWindowLongPtr.
	wndT& set_style(bool addStyle, DWORD styleFlags) noexcept {
		return this->_change_style_flags(false, addStyle, styleFlags);
	}

	// Sets an extended window style, wraps SetWindowLongPtr.
	wndT& set_style_ex(bool addStyle, DWORD styleFlags) noexcept {
		return this->_change_style_flags(true, addStyle, styleFlags);
	}

	// Wraps GetWindowLongPtr to retrieve an ordinary window style.
	bool has_style(DWORD styleFlags) const noexcept {
		return (GetWindowLongPtrW(this->_wnd.hwnd(), GWL_STYLE) & styleFlags) != 0;
	}

	// Wraps GetWindowLongPtr to retrieve an extended window style.
	bool has_style_ex(DWORD styleFlags) const noexcept {
		return (GetWindowLongPtrW(this->_wnd.hwnd(), GWL_EXSTYLE) & styleFlags) != 0;
	}

private:
	wndT& _change_style_flags(bool isEx, bool addStyle, DWORD styleFlags) noexcept {
		LONG_PTR curFlags = GetWindowLongPtrW(this->_wnd.hwnd(), isEx ? GWL_EXSTYLE : GWL_STYLE);
		if (addStyle) {
			curFlags |= static_cast<LONG_PTR>(styleFlags);
		} else {
			curFlags &= ~static_cast<LONG_PTR>(styleFlags);
		}
		SetWindowLongPtrW(this->_wnd.hwnd(), isEx ? GWL_EXSTYLE : GWL_STYLE, curFlags);
		return this->_wnd;
	}
};

}//namespace wli
}//namespace wl
