/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "internals/native_control.h"
#include "internals/has_enable.h"
#include "internals/has_focus.h"
#include "internals/styler.h"
#include "datetime.h"
#include <CommCtrl.h>

/**
 * hwnd_wrapper
 *  native_control
 *   has_focus
 *    has_enable
 *     textbox
 */

namespace wl {

// Wrapper to datetime picker control from Common Controls library.
class datetime_picker final :
	public wli::has_enable<
		datetime_picker, wli::has_focus<
			datetime_picker, wli::native_control<datetime_picker>>>
{
private:
	class _styler final : public wli::styler<datetime_picker> {
	public:
		explicit _styler(datetime_picker* pDtp) noexcept : styler(pDtp) { }

		datetime_picker& up_down_control(bool doSet) noexcept {
			return this->set_style(doSet, DTS_UPDOWN);
		}
	};

public:
	_styler style{this};

	datetime_picker& create(HWND hParent, int ctrlId, POINT pos, LONG width) {
		return this->native_control::create(hParent, ctrlId,
			nullptr, pos, {width,21}, DATETIMEPICK_CLASS);
	}

	datetime_picker& create(const wli::hwnd_wrapper* parent, int ctrlId, POINT pos, LONG width) {
		return this->create(parent->hwnd(), ctrlId, pos, width);
	}

	datetime get_time() const noexcept {
		SYSTEMTIME st{};
		DateTime_GetSystemtime(this->hwnd(), &st);
		return st;
	}

	datetime_picker& set_time(const datetime& dt) noexcept {
		DateTime_SetSystemtime(this->hwnd(), GDT_VALID, &dt.systemtime());
		return *this;
	}
};

}//namespace wl
