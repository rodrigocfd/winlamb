/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <array>
#include <Windows.h>

namespace wl {

// Wrappers to GDI objects.
namespace gdi {

// Wrapper to device context HPEN.
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

	pen(style styleType, int width, std::array<BYTE, 3> rgbColor) noexcept
		: _hPen(CreatePen(static_cast<int>(styleType), width, RGB(rgbColor[0], rgbColor[1], rgbColor[2]))) { }

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

// Wrapper to device context HBRUSH.
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

	brush(std::array<BYTE, 3> rgbColor) noexcept
		: _hBrush(CreateSolidBrush(RGB(rgbColor[0], rgbColor[1], rgbColor[2]))) { }

	brush(pattern hatch, COLORREF color) noexcept
		: _hBrush(CreateHatchBrush(static_cast<int>(hatch), color)) { }

	brush(pattern hatch, std::array<BYTE, 3> rgbColor) noexcept
		: _hBrush(CreateHatchBrush(static_cast<int>(hatch), RGB(rgbColor[0], rgbColor[1], rgbColor[2]))) { }

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

}//namespace gdi
}//namespace wl
