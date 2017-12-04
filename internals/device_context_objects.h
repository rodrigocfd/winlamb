/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <Windows.h>

namespace wl {
namespace wli {

class pen final {
public:
	enum class style : int {
		SOLID      = PS_SOLID,
		DASH       = PS_DASH,
		DOT        = PS_DOT,
		DASHDOT    = PS_DASHDOT,
		DASHDOTDOT = PS_DASHDOTDOT
	};

private:
	HPEN _hPen;

public:
	~pen() {
		this->release();
	}

	pen(style styleType, int width, COLORREF color) noexcept
		: _hPen(CreatePen(static_cast<int>(styleType), width, color)) { }

	HPEN hpen() const noexcept {
		return this->_hPen;
	}

	void release() noexcept {
		if (this->_hPen) {
			DeleteObject(this->_hPen);
			this->_hPen = nullptr;
		}
	}
};

class brush final {
public:
	enum class pattern : int {
		BDIAGONAL  = HS_BDIAGONAL,
		CROSS      = HS_CROSS,
		DIAGCROSS  = HS_DIAGCROSS,
		FDIAGONAL  = HS_FDIAGONAL,
		HORIZONTAL = HS_HORIZONTAL,
		VERTICAL   = HS_VERTICAL
	};

	enum class color : int {
		BTNFACE      = COLOR_BTNFACE,
		DESKTOP      = COLOR_DESKTOP,
		BTNTEXT      = COLOR_BTNTEXT,
		WINDOW       = COLOR_WINDOW,
		APPWORKSPACE = COLOR_APPWORKSPACE
	};

private:
	HBRUSH _hBrush;

public:
	~brush() {
		this->release();
	}

	brush(COLORREF color) noexcept
		: _hBrush(CreateSolidBrush(color)) { }

	brush(pattern hatch, COLORREF color) noexcept
		: _hBrush(CreateHatchBrush(static_cast<int>(hatch), color)) { }

	brush(color sysColor) noexcept
		: _hBrush(GetSysColorBrush(static_cast<int>(sysColor))) { }

	HBRUSH hbrush() const noexcept {
		return this->_hBrush;
	}

	void release() noexcept {
		if (this->_hBrush) {
			DeleteObject(this->_hBrush);
			this->_hBrush = nullptr;
		}
	}
};

}//namespace wli
}//namespace wl
