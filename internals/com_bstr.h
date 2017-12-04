/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <string>
#include <Windows.h>
#include <OleAuto.h>

namespace wl {
namespace wli {

// Wrapper to BSTR string, used with COM.
class com_bstr final {
private:
	BSTR _bstr = nullptr;

public:
	~com_bstr() {
		this->free();
	}

	com_bstr() = default;
	com_bstr(com_bstr&& other) noexcept      { this->operator=(std::move(other)); }
	com_bstr(const wchar_t* s) noexcept      { this->operator=(s); }
	com_bstr(const std::wstring& s) noexcept { this->operator=(s); }

	com_bstr& operator=(com_bstr&& other) noexcept {
		this->free();
		std::swap(this->_bstr, other._bstr);
		return *this;
	}
	
	com_bstr& operator=(const wchar_t* s) noexcept {
		this->free();
		this->_bstr = SysAllocString(s);
		return *this;
	}

	com_bstr& operator=(const std::wstring& s) noexcept {
		return this->operator=(s.c_str());
	}

	com_bstr& free() noexcept {
		if (this->_bstr) {
			SysFreeString(this->_bstr);
			this->_bstr = nullptr;
		}
		return *this;
	}

	BSTR           bstr() const noexcept  { return this->_bstr; }
	BSTR*          ptr() noexcept         { return &this->_bstr; }
	const wchar_t* c_str() const noexcept { return static_cast<wchar_t*>(this->_bstr); }
};

}//namespace wli
}//namespace wl
