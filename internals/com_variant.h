/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <string>
#include <OleAuto.h>
#include "com_ptr.h"

namespace wl {
namespace com {

// Wrapper to VARIANT object, used with COM.
class variant final {
private:
	VARIANT _variantObj{};

public:
	~variant() {
		this->clear();
	}

	variant() = default;
	variant(variant&& other) noexcept { this->operator=(std::move(other)); }

	operator const VARIANT&() const noexcept  { return this->_variantObj; }
	const VARIANT* operator&() const noexcept { return &this->_variantObj; }
	VARIANT* operator&() noexcept             { return &this->_variantObj; }

	variant& operator=(variant&& other) noexcept {
		this->clear();
		std::swap(this->_variantObj, other._variantObj);
		return *this;
	}

	variant& clear() noexcept {
		if (this->_variantObj.vt != VT_EMPTY) {
			VariantClear(&this->_variantObj); // will set VT_EMPTY
		}
		return *this;
	}

	variant& set_str(const wchar_t* s) noexcept {
		this->clear();
		this->_variantObj.vt = VT_BSTR;
		this->_variantObj.bstrVal = SysAllocString(s);
		return *this;
	}

	variant& set_str(const std::wstring& s) noexcept {
		return this->set_str(s.c_str());
	}

	const wchar_t* get_str() const noexcept {
		return static_cast<wchar_t*>(this->_variantObj.bstrVal);
	}

	variant& set_int4(long n) noexcept {
		this->clear();
		this->_variantObj.vt = VT_I4;
		this->_variantObj.lVal = n;
		return *this;
	}

	long get_int4() const noexcept {
		return this->_variantObj.lVal;
	}

	template<typename idispatch_derivedT>
	variant& set_idispatch(ptr<idispatch_derivedT>& objToQueryFrom) {
		this->clear();
		this->_variantObj.vt = VT_DISPATCH;
		objToQueryFrom.query_interface(IID_IDispatch, &this->_variantObj.pdispVal);
		return *this;
	}

	IDispatch* get_idispatch() const noexcept {
		return this->_variantObj.pdispVal;
	}
};

}//namepace com
}//namespace wl
