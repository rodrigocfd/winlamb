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
 *      button
 */

namespace wl {

// Wrapper to native button control.
class button final :
	public wli::has_enable<
		button, wli::has_focus<
			button, wli::has_text<
				button, wli::native_control<button>>>>
{
private:
	class _styler final : public wli::styler<button> {
	public:
		explicit _styler(button* pButton) : styler(pButton) { }
	};

public:
	_styler style{this};

	button& create(HWND hParent, int ctrlId,
		const wchar_t* caption, POINT pos, SIZE size = {75,23})
	{
		return this->native_control::create(hParent, ctrlId, caption, pos, size, L"Button");
	}

	button& create(const wli::hwnd_wrapper* parent, int ctrlId,
		const wchar_t* caption, POINT pos, SIZE size = {75,23})
	{
		return this->create(parent->hwnd(), ctrlId, caption, pos, size);
	}
};

}//namespace wl
