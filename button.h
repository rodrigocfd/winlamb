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
 *      button
 */

namespace wl {

// Wrapper to native button control.
class button final :
	public wli::w_enable<
		button, wli::w_focus<
			button, wli::w_text<
				button, wli::native_control<button>>>>
{
private:
	class _styler final : public wli::styler<button> {
	public:
		explicit _styler(button* pButton) noexcept : styler(pButton) { }
	};

public:
	_styler style{this};

	button& create(HWND hParent, int ctrlId,
		const wchar_t* caption, POINT pos, SIZE size = {75,23})
	{
		return this->native_control::create(hParent, ctrlId, caption, pos, size, L"Button");
	}

	button& create(const hwnd_base* parent, int ctrlId,
		const wchar_t* caption, POINT pos, SIZE size = {75,23})
	{
		return this->create(parent->hwnd(), ctrlId, caption, pos, size);
	}
};

}//namespace wl
