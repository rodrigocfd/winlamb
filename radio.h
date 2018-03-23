/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "internals/native_control.h"
#include "internals/w_enable.h"
#include "internals/w_focus.h"
#include "internals/w_text.h"
#include "internals/styler.h"

/**
 * hwnd_base
 *  native_control
 *   w_text
 *    w_focus
 *     w_enable
 *      radio
 */

namespace wl {

// Wrapper to native radio button control.
class radio final :
	public wli::w_enable<
		radio, wli::w_focus<
			radio, wli::w_text<
				radio, wli::native_control<radio>>>>
{
private:
	class _styler final : public wli::styler<radio> {
	public:
		explicit _styler(radio* pRadio) noexcept : styler(pRadio) { }

		radio& first_in_group(bool doSet) noexcept {
			return this->set_style(doSet, WS_GROUP);
		}
	};

public:
	_styler style{this};

	radio& create(HWND hParent, int ctrlId,
		const wchar_t* caption, POINT pos, SIZE size)
	{
		return this->native_control::create(hParent, ctrlId, caption, pos, size,
			L"Button", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTORADIOBUTTON);
	}

	radio& create(const hwnd_base* parent, int ctrlId,
		const wchar_t* caption, POINT pos, SIZE size)
	{
		return this->create(parent->hwnd(), ctrlId, caption, pos, size);
	}

	bool is_checked() const noexcept {
		return SendMessageW(this->hwnd(), BM_GETCHECK, 0, 0) == BST_CHECKED;
	}

	radio& set_check(bool checked) noexcept {
		SendMessageW(this->hwnd(), BM_SETCHECK,
			checked ? BST_CHECKED : BST_UNCHECKED, 0);
		return *this;
	}

	radio& set_check_and_trigger(bool checked) noexcept {
		this->set_check(checked);
		SendMessageW(GetParent(this->hwnd()), WM_COMMAND,
			MAKEWPARAM(GetDlgCtrlID(this->hwnd()), 0),
			reinterpret_cast<LPARAM>(this->hwnd()) ); // emulate user click
		return *this;
	}
};

}//namespace wl
