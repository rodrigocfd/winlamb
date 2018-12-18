/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <Windows.h>
#include "internals/enable_bitmask_operators.h"
#include "wnd.h"

namespace wl {

// Automates SCROLLINFO operations.
class scrollinfo final {
public:
	// Can be combined with bitmask operators.
	enum class info : UINT {
		PAGE  = SIF_PAGE,
		POS   = SIF_POS,
		RANGE = SIF_RANGE,
		TRACK = SIF_TRACKPOS
	};

	enum class bar : int {
		HORZ = SB_HORZ,
		VERT = SB_VERT,
		CTRL = SB_CTL
	};

private:
	SCROLLINFO _si{sizeof(SCROLLINFO)};

public:
	int&  pos{_si.nPos};
	int&  minPos{_si.nMin};
	int&  maxPos{_si.nMax};
	int&  trackPos{_si.nTrackPos};
	UINT& pageSz{_si.nPage};

	scrollinfo() = default;

	explicit scrollinfo(info whatFlags) noexcept {
		this->set_flags(whatFlags);
	}

	scrollinfo& set_flags(info whatFlags) noexcept {
		this->_si.fMask = static_cast<UINT>(whatFlags);
		return *this;
	}
	
	info get_flags() const noexcept {
		return static_cast<info>(this->_si.fMask);
	}

	// Calls GetScrollInfo function.
	scrollinfo& get_scroll(HWND target, bar whatBar) noexcept {
		GetScrollInfo(target, static_cast<int>(whatBar), &this->_si);
		return *this;
	}

	// Calls GetScrollInfo function.
	scrollinfo& get_scroll(const wnd* target, bar whatBar) noexcept {
		return this->get_scroll(target->hwnd(), whatBar);
	}

	// Calls SetScrollInfo function, returns current position.
	int set_scroll(HWND target, bar whatBar) noexcept {
		return SetScrollInfo(target, static_cast<int>(whatBar), &this->_si, TRUE);
	}

	// Calls SetScrollInfo function, returns current position.
	int set_scroll(const wnd* target, bar whatBar) noexcept {
		return this->set_scroll(target->hwnd(), whatBar);
	}
};

ENABLE_BITMASK_OPERATORS(scrollinfo::info);

}//namespace wl
