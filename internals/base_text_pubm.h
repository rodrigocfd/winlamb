/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <string>
#include <Windows.h>

namespace wl {
namespace wli {

// Public methods that allow get/set window text with std::wstring.
template<typename derivedT>
class base_text_pubm {
private:
	const HWND& _hWnd;

public:
	base_text_pubm(const HWND& hWnd) noexcept :
		_hWnd(hWnd) { }

	// Simple wrapper to SetWindowText.
	derivedT& set_text(const wchar_t* s) noexcept {
		SetWindowTextW(this->_hWnd, s);
		return *static_cast<derivedT*>(this);
	}

	// Simple wrapper to SetWindowText.
	derivedT& set_text(const std::wstring& s) noexcept {
		return this->set_text(s.c_str());
	}

	// Simple wrapper to GetWindowText.
	std::wstring get_text() const {
		std::wstring buf;
		int len = GetWindowTextLengthW(this->_hWnd);
		if (len) {
			buf.resize(len + 1, L'\0');
			GetWindowTextW(this->_hWnd, &buf[0], len + 1);
			buf.resize(len);
		}
		return buf;
	}
};

}//namespace wli
}//namespace wl
