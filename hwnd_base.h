/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <Windows.h>

namespace wl {
class subclass; // friend forward declaration

namespace wli {
	template<typename baseT> class dialog; // friend forward declarations
	template<typename baseT> class window;
	template<typename baseT> class native_control;
}

// Simple HWND wrapper, base to all dialog and window classes.
class hwnd_base {
	friend class subclass;
	template<typename baseT> friend class wli::dialog;
	template<typename baseT> friend class wli::native_control;
	template<typename baseT> friend class wli::window;

private:
	HWND _hWnd = nullptr; // can be set only by friends

protected:
	hwnd_base() = default;

public:
	HWND hwnd() const noexcept {
		return this->_hWnd;
	}
};

}//namespace wl
