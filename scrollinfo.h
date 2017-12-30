/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "hwnd_base.h"
#include "internals/combinable_flags.h"

namespace wl {

// Automates SCROLLINFO operations.
class scrollinfo final {
public:
	enum class flags : UINT {
		PAGE  = SIF_PAGE,
		POS   = SIF_POS,
		RANGE = SIF_RANGE,
		TRACK = SIF_TRACKPOS,
		WINLAMB_COMBINED_FLAGS4(PAGE, POS, RANGE, TRACK)
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
	explicit scrollinfo(flags whatFlags) noexcept { this->set_flags(whatFlags); }

	scrollinfo& set_flags(flags whatFlags) noexcept {
		this->_si.fMask = static_cast<UINT>(whatFlags);
		return *this;
	}
	
	flags get_flags() const noexcept {
		return static_cast<flags>(this->_si.fMask);
	}

	// Calls GetScrollInfo function.
	scrollinfo& get(HWND target, bar whatBar) noexcept {
		GetScrollInfo(target, static_cast<int>(whatBar), &this->_si);
		return *this;
	}

	// Calls GetScrollInfo function.
	scrollinfo& get(const hwnd_base* target, bar whatBar) noexcept {
		return this->get(target->hwnd(), whatBar);
	}

	// Calls SetScrollInfo function.
	int set(HWND target, bar whatBar) noexcept {
		return SetScrollInfo(target, static_cast<int>(whatBar), &this->_si, TRUE); // returns current position
	}

	// Calls SetScrollInfo function.
	int set(const hwnd_base* target, bar whatBar) noexcept {
		return this->set(target->hwnd(), whatBar);
	}
};

WINLAMB_COMBINABLE_FLAGS(scrollinfo::flags);

}//namespace wl
