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
namespace _wli {

// Wrapper to BSTR string, used with COM.
class com_bstr final {
private:
	BSTR _bstr = nullptr;

public:
	~com_bstr() {
		this->free();
	}

	com_bstr() = default;
	com_bstr(com_bstr&& other) noexcept      : _bstr{other._bstr} { other._bstr = nullptr; }
	com_bstr(const wchar_t* s) noexcept      : _bstr{SysAllocString(s)} { }
	com_bstr(const std::wstring& s) noexcept : com_bstr(s.c_str()) { }

	operator const BSTR&() const noexcept  { return this->_bstr; }
	const BSTR* operator&() const noexcept { return &this->_bstr; }
	BSTR* operator&() noexcept             { return &this->_bstr; }

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

	const wchar_t* c_str() const noexcept {
		return static_cast<wchar_t*>(this->_bstr);
	}
};

}//namepace _wli
}//namespace wl
