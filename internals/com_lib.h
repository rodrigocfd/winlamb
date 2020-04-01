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

// Wrappers to COM objects.
namespace com {

// Smart class to automate CoInitialize and CoUninitialize COM calls.
class lib {
private:
	HRESULT _hr = -1;

public:
	enum class init { NOW, LATER };

	~lib() {
		// https://stackoverflow.com/q/47123650/6923555
		this->un_initialize();
	}

	explicit lib(init when) noexcept {
		if (when == init::NOW) {
			this->initialize();
		}
	}

	lib(lib&& other) noexcept : _hr{other._hr} { other._hr = -1; }

	lib& operator=(lib&& other) noexcept {
		this->un_initialize();
		std::swap(this->_hr, other._hr);
		return *this;
	}

	void initialize() noexcept {
		// #define FAILED(hr) (((HRESULT)(hr)) < 0)
		if (FAILED(this->_hr)) {
			// So that initialize() can be carelessly called multiple
			// times, but CoInitialize() will be called only once.
			this->_hr = CoInitialize(nullptr);
		}
	}

	void un_initialize() noexcept {
		// #define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
		if (SUCCEEDED(this->_hr)) {
			CoUninitialize();
			this->_hr = -1;
		}
	}

	HRESULT hresult() const noexcept {
		return this->_hr;
	}
};


// COM utility which calls FAILED() macro upon HRESULT; if failed, throws a system_error.
inline void check_hr(HRESULT hr, const char* exceptionMsg) {
	if (FAILED(hr)) {
		throw std::system_error(hr, std::system_category(), exceptionMsg);
	}
}

}//namespace com
}//namespace wl
