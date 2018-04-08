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

template<typename derivedT, typename baseT>
class w_enable : public baseT {
protected:
	w_enable() = default;

public:
	// Simple wrapper to IsWindowEnabled.
	bool is_enabled() const noexcept {
		return IsWindowEnabled(this->hwnd());
	}

	// Simple wrapper to EnableWindow.
	derivedT& set_enable(bool doEnable) noexcept {
		EnableWindow(this->hwnd(), doEnable);
		return *static_cast<derivedT*>(this);
	}
};

}//namespace wli
}//namespace wl
