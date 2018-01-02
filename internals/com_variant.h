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
namespace wli {

// Wrapper to VARIANT object, used with COM.
class com_variant final {
private:
	VARIANT _variant{};

public:
	~com_variant() {
		this->clear();
	}

	com_variant() = default;
	com_variant(com_variant&& other) noexcept { this->operator=(std::move(other)); }

	operator const VARIANT&() const noexcept  { return this->_variant; }
	const VARIANT* operator&() const noexcept { return &this->_variant; }
	VARIANT* operator&() noexcept             { return &this->_variant; }

	com_variant& operator=(com_variant&& other) noexcept {
		this->clear();
		std::swap(this->_variant, other._variant);
		return *this;
	}

	com_variant& clear() noexcept {
		if (this->_variant.vt != VT_EMPTY) {
			VariantClear(&this->_variant); // will set VT_EMPTY
		}
		return *this;
	}

	com_variant& set_str(const wchar_t* s) noexcept {
		this->clear();
		this->_variant.vt = VT_BSTR;
		this->_variant.bstrVal = SysAllocString(s);
		return *this;
	}

	com_variant& set_str(const std::wstring& s) noexcept {
		return this->set_str(s.c_str());
	}

	const wchar_t* get_str() const noexcept {
		return static_cast<wchar_t*>(this->_variant.bstrVal);
	}

	com_variant& set_int4(long n) noexcept {
		this->clear();
		this->_variant.vt = VT_I4;
		this->_variant.lVal = n;
		return *this;
	}

	long get_int4() const noexcept {
		return this->_variant.lVal;
	}

	template<typename idispatch_derivedT>
	com_variant& set_idispatch(com_ptr<idispatch_derivedT>& objToQueryFrom) {
		this->clear();
		this->_variant.vt = VT_DISPATCH;
		objToQueryFrom.query_interface(IID_IDispatch, &this->_variant.pdispVal);
		return *this;
	}

	IDispatch* get_idispatch() const noexcept {
		return this->_variant.pdispVal;
	}
};

}//namespace wli
}//namespace wl
