/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <system_error>
#include <Windows.h>
#include <objbase.h>

namespace wl {
namespace wli {

// Wrapper to a COM pointer.
template<typename ptrT>
class com_ptr final {
private:
	ptrT* _ptr = nullptr;

public:
	~com_ptr() {
		this->release();
	}

	com_ptr() = default;
	com_ptr(com_ptr&& other) noexcept { this->operator=(std::move(other)); }

	bool   empty() const noexcept { return this->_ptr == nullptr; }
	ptrT*  operator->() noexcept  { return this->_ptr; }
	ptrT*  ptr() const noexcept   { return this->_ptr; }
	ptrT** pptr() noexcept        { return &this->_ptr; }

	com_ptr& operator=(com_ptr&& other) noexcept {
		this->release();
		std::swap(this->_ptr, other._ptr);
		return *this;
	}

	void release() noexcept {
		if (this->_ptr) {
			this->_ptr->Release();
			this->_ptr = nullptr;
		}
	}

	void co_create_instance(REFCLSID clsid_something) {
		this->_check_creating_twice();
		HRESULT hr = CoCreateInstance(clsid_something, nullptr,
			CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&this->_ptr));
		if (FAILED(hr)) {
			throw std::system_error(hr, std::system_category(),
				"CoCreateInstance failed");
		}
	}

	void co_create_instance(REFCLSID clsid_something, REFIID iid_something) {
		this->_check_creating_twice();
		HRESULT hr = CoCreateInstance(clsid_something, nullptr,
			CLSCTX_INPROC_SERVER, iid_something, reinterpret_cast<void**>(&this->_ptr));
		if (FAILED(hr)) {
			throw std::system_error(hr, std::system_category(),
				"CoCreateInstance failed");
		}
	}

	template<typename com_interfaceT>
	void query_interface(REFIID iid_something, com_interfaceT** targetComPtr) {
		this->_check_uncreated();
		HRESULT hr = this->_ptr->QueryInterface(iid_something,
			reinterpret_cast<void**>(targetComPtr));
		if (FAILED(hr)) {
			throw std::system_error(hr, std::system_category(),
				"QueryInterface failed");
		}
	}

	template<typename com_interfaceT>
	void query_interface(REFIID iid_something, com_ptr<com_interfaceT>& targetComPtr) {
		this->query_interface(iid_something, targetComPtr.pptr());
	}

	template<typename com_interfaceT>
	void query_interface(com_interfaceT** targetComPtr) {
		this->_check_uncreated();
		HRESULT hr = this->_ptr->QueryInterface(IID_PPV_ARGS(targetComPtr));
		if (FAILED(hr)) {
			throw std::system_error(hr, std::system_category(),
				"QueryInterface failed");
		}
	}

	template<typename com_interfaceT>
	void query_interface(com_ptr<com_interfaceT>& targetComPtr) {
		this->query_interface(targetComPtr.pptr());
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
