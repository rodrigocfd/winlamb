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

template<typename derivedT, typename baseT>
class w_text : public baseT {
protected:
	w_text() = default;

public:
	derivedT& set_text(const wchar_t* s) noexcept {
		SetWindowTextW(this->hwnd(), s);
		return *static_cast<derivedT*>(this);
	}

	derivedT& set_text(const std::wstring& s) noexcept {
		return this->set_text(s.c_str());
	}

	std::wstring get_text() const {
		std::wstring buf;
		int len = GetWindowTextLengthW(this->hwnd());
		if (len) {
			buf.resize(len + 1, L'\0');
			GetWindowTextW(this->hwnd(), &buf[0], len + 1);
			buf.resize(len);
		}
		return buf;
	}
};

}//namespace wli
}//namespace wl
