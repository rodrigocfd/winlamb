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
namespace com {

// Wrapper to BSTR string, used with COM.
class bstr final {
private:
	BSTR _bstrObj = nullptr;

public:
	~bstr() {
		this->free();
	}

	bstr() = default;
	bstr(bstr&& other) noexcept          : _bstrObj{other._bstrObj} { other._bstrObj = nullptr; }
	bstr(const wchar_t* s) noexcept      : _bstrObj{SysAllocString(s)} { }
	bstr(const std::wstring& s) noexcept : bstr(s.c_str()) { }

	operator const BSTR&() const noexcept  { return this->_bstrObj; }
	const BSTR* operator&() const noexcept { return &this->_bstrObj; }
	BSTR* operator&() noexcept             { return &this->_bstrObj; }

	bstr& operator=(bstr&& other) noexcept {
		this->free();
		std::swap(this->_bstrObj, other._bstrObj);
		return *this;
	}
	
	bstr& operator=(const wchar_t* s) noexcept {
		this->free();
		this->_bstrObj = SysAllocString(s);
		return *this;
	}

	bstr& operator=(const std::wstring& s) noexcept {
		return this->operator=(s.c_str());
	}

	bstr& free() noexcept {
		if (this->_bstrObj) {
			SysFreeString(this->_bstrObj);
			this->_bstrObj = nullptr;
		}
		return *this;
	}

	const wchar_t* c_str() const noexcept {
		return static_cast<wchar_t*>(this->_bstrObj);
	}
};

}//namespace com
}//namespace wl
