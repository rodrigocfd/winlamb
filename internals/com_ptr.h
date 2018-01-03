/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "com_lib.h"

namespace wl {
namespace wli {

// Wrapper to a COM pointer.
template<typename ptrT>
class com_ptr final {
private:
	ptrT* _ptr = nullptr;

public:
	~com_ptr() {
		this->reset();
	}

	com_ptr() = default;
	com_ptr(ptrT* livePtr) noexcept   : _ptr{livePtr} { }
	com_ptr(com_ptr&& other) noexcept : _ptr{other._ptr} { other._ptr = nullptr; }

	explicit operator bool() const noexcept  { return this->_ptr != nullptr; }
	operator const ptrT*() const noexcept    { return this->_ptr; }
	operator ptrT*() noexcept                { return this->_ptr; }
	const ptrT*  operator->() const noexcept { return this->_ptr; }
	ptrT*        operator->() noexcept       { return this->_ptr; }
	const ptrT** operator&() const noexcept  { return &this->_ptr; }
	ptrT**       operator&() noexcept        { return &this->_ptr; }

	com_ptr& operator=(com_ptr&& other) noexcept {
		this->reset();
		std::swap(this->_ptr, other._ptr);
		return *this;
	}

	void reset(ptrT* livePtr = nullptr) noexcept {
		if (this->_ptr == livePtr) return;
		if (this->_ptr) this->_ptr->Release();
		this->_ptr = livePtr;
	}

	void co_create_instance(REFCLSID clsid_something) {
		this->_check_creating_twice();
		check_hr(
			CoCreateInstance(clsid_something, nullptr,
				CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&this->_ptr)),
			"CoCreateInstance failed");
	}

	void co_create_instance(REFCLSID clsid_something, REFIID iid_something) {
		this->_check_creating_twice();
		check_hr(
			CoCreateInstance(clsid_something, nullptr,
				CLSCTX_INPROC_SERVER, iid_something, reinterpret_cast<void**>(&this->_ptr)),
			"CoCreateInstance failed");
	}

	template<typename com_interfaceT>
	void query_interface(REFIID iid_something, com_interfaceT** targetComPtr) {
		this->_check_uncreated();
		check_hr(
			this->_ptr->QueryInterface(iid_something, reinterpret_cast<void**>(targetComPtr)),
			"QueryInterface failed");
	}

	template<typename com_interfaceT>
	void query_interface(com_interfaceT** targetComPtr) {
		this->_check_uncreated();
		check_hr(
			this->_ptr->QueryInterface(IID_PPV_ARGS(targetComPtr)),
			"QueryInterface failed");
	}

private:
	void _check_creating_twice() const {
		if (this->_ptr) {
			throw std::logic_error("Trying to create a COM object twice.");
		}
	}

	void _check_uncreated() const {
		if (!this->_ptr) {
			throw std::logic_error("Trying to query interface from a non-created COM object.");
		}
	}
};

}//namespace wli
}//namespace wl
