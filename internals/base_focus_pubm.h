/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <Windows.h>

namespace wl {
namespace _wli {

// Public methods that allow set/retrieve focus.
template<typename derivedT>
class base_focus_pubm {
private:
	const HWND& _hWnd;

public:
	base_focus_pubm(const HWND& hWnd) noexcept :
		_hWnd(hWnd) { }

	// Simple wrapper to SetFocus.
	derivedT& set_focus() noexcept {
		SetFocus(this->_hWnd);
		return *static_cast<derivedT*>(this);
	}

	// Simple wrapper to GetFocus, comparing to own handle.
	bool is_focus() const noexcept {
		return GetFocus() == this->_hWnd;
	}
};

}//namespace _wli
}//namespace wl
