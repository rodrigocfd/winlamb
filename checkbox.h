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
#include "internals/has_text.h"
#include "internals/styler.h"

/**
 * hwnd_wrapper
 *  native_control
 *   has_text
 *    has_focus
 *     has_enable
 *      checkbox
 */

namespace wl {

// Wrapper to native checkbox control.
class checkbox final :
	public wli::has_enable<
		checkbox, wli::has_focus<
			checkbox, wli::has_text<
				checkbox, wli::native_control<checkbox>>>>
{
private:
	class _styler final : public wli::styler<checkbox> {
	public:
		explicit _styler(checkbox* pCheckbox) : styler(pCheckbox) { }
	};

public:
	_styler style{this};

	checkbox& create(HWND hParent, int ctrlId,
		const wchar_t* caption, POINT pos, SIZE size)
	{
		return this->native_control::create(hParent, ctrlId, caption, pos, size,
			L"Button", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX);
	}

	checkbox& create(const wli::hwnd_wrapper* parent, int ctrlId,
		const wchar_t* caption, POINT pos, SIZE size)
	{
		return this->create(parent->hwnd(), ctrlId, caption, pos, size);
	}

	bool is_checked() const {
		return SendMessageW(this->hwnd(), BM_GETCHECK, 0, 0) == BST_CHECKED;
	}

	checkbox& set_check(bool checked) {
		SendMessageW(this->hwnd(), BM_SETCHECK,
			checked ? BST_CHECKED : BST_UNCHECKED, 0);
		return *this;
	}

	checkbox& set_check_and_trigger(bool checked) {
		this->set_check(checked);
		SendMessageW(GetParent(this->hwnd()), WM_COMMAND,
			MAKEWPARAM(GetDlgCtrlID(this->hwnd()), 0),
			reinterpret_cast<LPARAM>(this->hwnd()) ); // emulate user click
		return *this;
	}
};

}//namespace wl
