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
namespace _wli {

// Smart class to automate CoInitialize and CoUninitialize calls.
class com_lib {
private:
	HRESULT _hr = -1;

public:
	enum class init { NOW, LATER };

	~com_lib() {
		// https://stackoverflow.com/q/47123650/6923555
		this->un_initialize();
	}

	explicit com_lib(init when) noexcept {
		if (when == init::NOW) {
			this->initialize();
		}
	}

	com_lib(com_lib&& other) noexcept : _hr{other._hr} { other._hr = -1; }

	com_lib& operator=(com_lib&& other) noexcept {
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


inline void check_hr(HRESULT hr, const char* exceptionMsg) {
	if (FAILED(hr)) {
		throw std::system_error(hr, std::system_category(), exceptionMsg);
	}
}

}//namepace _wli
}//namespace wl
