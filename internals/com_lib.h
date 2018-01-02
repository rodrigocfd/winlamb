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

// Smart class to automate CoInitialize calls and cleanup.
class com_lib {
private:
	HRESULT _hr = -1;

public:
	enum class init { NOW, LATER };

	~com_lib() {
		// https://stackoverflow.com/q/47123650/6923555
		if (SUCCEEDED(this->_hr)) {
			CoUninitialize();
		}
	}

	explicit com_lib(init when) noexcept {
		if (when == init::NOW) {
			this->initialize();
		}
	}

	void initialize() noexcept {
		if (FAILED(this->_hr)) { // so that initialize() can be called multiple times
			this->_hr = CoInitialize(nullptr);
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

}//namespace wli
}//namespace wl
