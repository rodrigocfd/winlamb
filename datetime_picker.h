/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "internals/base_focus_pubm.h"
#include "internals/base_native_ctrl_pubm.h"
#include "internals/styler.h"
#include "datetime.h"
#include "wnd.h"
#include <CommCtrl.h>

namespace wl {

// Wrapper to datetime picker control from Common Controls library.
class datetime_picker final :
	public wnd,
	public _wli::base_native_ctrl_pubm<datetime_picker>,
	public _wli::base_focus_pubm<datetime_picker>
{
private:
	class _styler final : public _wli::styler<datetime_picker> {
	public:
		explicit _styler(datetime_picker* pDtp) noexcept :
			styler(pDtp) { }

		datetime_picker& up_down_control(bool doSet) noexcept {
			return this->set_style(doSet, DTS_UPDOWN);
		}
	};

	HWND                   _hWnd = nullptr;
	_wli::base_native_ctrl _baseNativeCtrl{_hWnd};

public:
	// Wraps window style changes done by Get/SetWindowLongPtr.
	_styler style{this};

	datetime_picker() noexcept :
		wnd(_hWnd), base_native_ctrl_pubm(_baseNativeCtrl), base_focus_pubm(_hWnd) { }

	datetime_picker(datetime_picker&&) = default;
	datetime_picker& operator=(datetime_picker&&) = default; // movable only

	datetime_picker& create(HWND hParent, int ctrlId, POINT pos, LONG width) {
		this->_baseNativeCtrl.create(hParent, ctrlId,
			nullptr, pos, {width,21}, DATETIMEPICK_CLASS);
		return *this;
	}

	datetime_picker& create(const wnd* parent, int ctrlId, POINT pos, LONG width) {
		return this->create(parent->hwnd(), ctrlId, pos, width);
	}

	datetime get_time() const noexcept {
		SYSTEMTIME st{};
		DateTime_GetSystemtime(this->_hWnd, &st);
		return st;
	}

	datetime_picker& set_time(const datetime& dt) noexcept {
		DateTime_SetSystemtime(this->_hWnd, GDT_VALID, &dt.systemtime());
		return *this;
	}
};

}//namespace wl
