/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "internals/native_control.h"
#include "internals/has_enable.h"
#include "internals/has_text.h"
#include "internals/styler.h"

/**
 * hwnd_base
 *  native_control
 *   has_text
 *     has_enable
 *      label
 */

namespace wl {

// Wrapper to native static text control.
class label final :
	public wli::has_enable<
		label, wli::has_text<
			label, wli::native_control<label>>>
{
private:
	class _styler final : public wli::styler<label> {
	public:
		explicit _styler(label* pLabel) noexcept : styler(pLabel) { }
	};

public:
	_styler style{this};

	label& create(HWND hParent, int ctrlId,
		const wchar_t* caption, POINT pos, SIZE size)
	{
		return this->native_control::create(hParent, ctrlId, caption, pos, size, L"Static");
	}

	label& create(const wli::hwnd_base* parent, int ctrlId,
		const wchar_t* caption, POINT pos, SIZE size)
	{
		return this->create(parent->hwnd(), ctrlId, caption, pos, size);
	}
};

}//namespace wl
