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
 *      textbox
 */

namespace wl {

// Wrapper to native edit box control.
class textbox final :
	public wli::w_enable<
		textbox, wli::w_focus<
			textbox, wli::w_text<
				textbox, wli::native_control<textbox>>>>
{
private:
	class _styler final : public wli::styler<textbox> {
	public:
		explicit _styler(textbox* pTextbox) noexcept : styler(pTextbox) { }

		textbox& password(bool doSet) noexcept {
			return this->set_style(doSet, ES_PASSWORD);
		}
	};

public:
	enum class type { NORMAL, PASSWORD, MULTILINE };

	// Information about start and length of selected text.
	struct selection final {
		int start = 0;
		int len = 0;
	};

	_styler style{this};

	textbox& create(HWND hParent, int ctrlId,
		type t, POINT pos, LONG width, LONG height = 21)
	{
		DWORD styles = WS_CHILD | WS_VISIBLE;
		switch (t) {
		case type::NORMAL:    styles |= ES_AUTOHSCROLL; break;
		case type::PASSWORD:  styles |= ES_AUTOHSCROLL | ES_PASSWORD; break;
		case type::MULTILINE: styles |= ES_MULTILINE | ES_WANTRETURN;
		}
		return this->native_control::create(hParent, ctrlId, nullptr,
			pos, {width, height}, L"Edit",
			WS_CHILD | WS_VISIBLE | styles, WS_EX_CLIENTEDGE);
	}

	textbox& create(const hwnd_base* parent, int ctrlId,
		type t, POINT pos, LONG width, LONG height = 21)
	{
		return this->create(parent->hwnd(), ctrlId, t, pos, width, height);
	}

	textbox& textbox::selection_set(selection selec) noexcept {
		SendMessageW(this->hwnd(), EM_SETSEL, selec.start, selec.start + selec.len);
		return *this;
	}

	textbox& selection_set_all() noexcept {
		return this->selection_set({0, -1});
	}

	selection selection_get() const noexcept {
		int p0 = 0, p1 = 0;
		SendMessageW(this->hwnd(), EM_GETSEL,
			reinterpret_cast<WPARAM>(&p0), reinterpret_cast<LPARAM>(&p1));
		return {p0, p1 - p0}; // start, length
	}

	textbox& selection_replace(const wchar_t* t) noexcept {
		SendMessageW(this->hwnd(), EM_REPLACESEL,
			TRUE, reinterpret_cast<LPARAM>(t));
		return *this;
	}

	textbox& selection_replace(const std::wstring& t) noexcept {
		return this->selection_replace(t.c_str());
	}
};

}//namespace wl
