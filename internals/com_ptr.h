/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "com_lib.h"

namespace wl {

// Wrappers to COM objects.
namespace com {

// Wrapper to a COM pointer.
template<typename pointerT>
class ptr final {
private:
	pointerT* _ptrObj = nullptr;

public:
	~ptr() {
		this->reset();
	}

	ptr() = default;
	ptr(ptr&& other) noexcept : _ptrObj{other._ptrObj} { other._ptrObj = nullptr; }
	explicit ptr(pointerT* livePtr) noexcept : _ptrObj{livePtr} { }

	ptr& operator=(ptr&& other) noexcept {
		this->reset();
		std::swap(this->_ptrObj, other._ptrObj);
		return *this;
	}
	
	explicit operator bool() const noexcept      { return this->_ptrObj != nullptr; }
	operator const pointerT*() const noexcept    { return this->_ptrObj; }
	operator pointerT*() noexcept                { return this->_ptrObj; }
	const pointerT*  operator->() const noexcept { return this->_ptrObj; }
	pointerT*        operator->() noexcept       { return this->_ptrObj; }
	const pointerT** operator&() const noexcept  { return &this->_ptrObj; }
	pointerT**       operator&() noexcept        { return &this->_ptrObj; }

	void reset(pointerT* livePtr = nullptr) noexcept {
		if (this->_ptrObj == livePtr) return;
		if (this->_ptrObj) this->_ptrObj->Release();
		this->_ptrObj = livePtr;
	}

	void co_create_instance(REFCLSID clsid_something) {
		this->_check_creating_twice();
		check_hr(
			CoCreateInstance(clsid_something, nullptr,
				CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&this->_ptrObj)),
			"CoCreateInstance failed");
	}

	void co_create_instance(REFCLSID clsid_something, REFIID iid_something) {
		this->_check_creating_twice();
		check_hr(
			CoCreateInstance(clsid_something, nullptr,
				CLSCTX_INPROC_SERVER, iid_something, reinterpret_cast<void**>(&this->_ptrObj)),
			"CoCreateInstance failed");
	}

	template<typename com_interfaceT>
	void query_interface(REFIID iid_something, com_interfaceT** targetComPtr) {
		this->_check_uncreated();
		check_hr(
			this->_ptrObj->QueryInterface(iid_something, reinterpret_cast<void**>(targetComPtr)),
			"QueryInterface failed");
	}

	template<typename com_interfaceT>
	void query_interface(com_interfaceT** targetComPtr) {
		this->_check_uncreated();
		check_hr(
			this->_ptrObj->QueryInterface(IID_PPV_ARGS(targetComPtr)),
			"QueryInterface failed");
	}

	ptr clone() noexcept {
		_ptrObj->AddRef();
		return ptr{_ptrObj};
	}

private:
	void _check_creating_twice() const {
		if (this->_ptrObj) {
			throw std::logic_error("Trying to create a COM object twice.");
		}
	}

	void _check_uncreated() const {
		if (!this->_ptrObj) {
			throw std::logic_error("Trying to query interface from a non-created COM object.");
		}
	}
};

}//namespace com
}//namespace wl
