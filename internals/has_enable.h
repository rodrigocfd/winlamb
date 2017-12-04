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
class has_enable : public baseT {
protected:
	has_enable() = default;

public:
	bool is_enabled() const noexcept {
		return IsWindowEnabled(this->hwnd());
	}

	derivedT& set_enable(bool doEnable) noexcept {
		EnableWindow(this->hwnd(), doEnable);
		return *static_cast<derivedT*>(this);
	}
};

}//namespace wli
}//namespace wl
