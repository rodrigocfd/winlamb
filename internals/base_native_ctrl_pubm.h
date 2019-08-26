/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "base_native_ctrl.h"
#include "../wnd.h"

namespace wl {
namespace _wli {

// Provides public methods for base_native_ctrl class.
template<typename derivedT>
class base_native_ctrl_pubm {
private:
	base_native_ctrl& _baseNativeCtrl;

public:
	base_native_ctrl_pubm(base_native_ctrl& baseNativeCtrl) noexcept :
		_baseNativeCtrl(baseNativeCtrl) { }

	// Returns the control ID.
	int ctrl_id() const noexcept {
		return GetDlgCtrlID(this->_baseNativeCtrl.hwnd());
	}

	// Ties this class instance to an existing native control.
	derivedT& assign(HWND hCtrl) {
		this->_baseNativeCtrl.assign(hCtrl);
		return *static_cast<derivedT*>(this);
	}

	// Ties this class instance to an existing native control.
	derivedT& assign(HWND hParent, int ctrlId) {
		return this->assign(GetDlgItem(hParent, ctrlId));
	}

	// Ties this class instance to an existing native control.
	derivedT& assign(const wnd* parent, int ctrlId) {
		return this->assign(parent->hwnd(), ctrlId);
	}

	// Simple wrapper to IsWindowEnabled.
	bool is_enabled() const noexcept {
		return IsWindowEnabled(this->_baseNativeCtrl.hwnd());
	}

	// Simple wrapper to EnableWindow.
	derivedT& set_enabled(bool enabled) noexcept {
		EnableWindow(this->_baseNativeCtrl.hwnd(), enabled);
		return *static_cast<derivedT*>(this);
	}
};

}//namespace _wli
}//namespace wl
