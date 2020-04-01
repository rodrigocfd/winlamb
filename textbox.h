/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "internals/base_focus_pubm.h"
#include "internals/base_text_pubm.h"
#include "internals/base_native_ctrl_pubm.h"
#include "internals/styler.h"
#include "wnd.h"

namespace wl {

// Wrapper to native edit box control.
class textbox final :
	public wnd,
	public _wli::base_native_ctrl_pubm<textbox>,
	public _wli::base_focus_pubm<textbox>,
	public _wli::base_text_pubm<textbox>
{
private:
	class _styler final : public _wli::styler<textbox> {
	public:
		explicit _styler(textbox* pTextbox) noexcept :
			styler(pTextbox) { }

		textbox& password(bool doSet) noexcept {
			return this->set_style(doSet, ES_PASSWORD);
		}
	};

	HWND                   _hWnd = nullptr;
	_wli::base_native_ctrl _baseNativeCtrl{_hWnd};

public:
	enum class type { NORMAL, PASSWORD, MULTILINE };

	// Information about start and length of selected text.
	struct selection final {
		int start = 0;
		int len = 0;
	};

	// Wraps window style changes done by Get/SetWindowLongPtr.
	_styler style{this};

	textbox() noexcept :
		wnd(_hWnd), base_native_ctrl_pubm(_baseNativeCtrl),
		base_focus_pubm(_hWnd), base_text_pubm(_hWnd) { }

	textbox(textbox&&) = default;
	textbox& operator=(textbox&&) = default; // movable only

	textbox& create(HWND hParent, int ctrlId,
		type t, POINT pos, LONG width, LONG height = 21)
	{
		DWORD styles = WS_CHILD | WS_VISIBLE;
		switch (t) {
		case type::NORMAL:    styles |= ES_AUTOHSCROLL; break;
		case type::PASSWORD:  styles |= ES_AUTOHSCROLL | ES_PASSWORD; break;
		case type::MULTILINE: styles |= ES_MULTILINE | ES_WANTRETURN;
		}

		this->_baseNativeCtrl.create(hParent, ctrlId, nullptr,
			pos, {width, height}, L"Edit",
			WS_CHILD | WS_VISIBLE | styles, WS_EX_CLIENTEDGE);
		return *this;
	}

	textbox& create(const wnd* parent, int ctrlId,
		type t, POINT pos, LONG width, LONG height = 21)
	{
		return this->create(parent->hwnd(), ctrlId, t, pos, width, height);
	}

	textbox& select(selection selec) noexcept {
		SendMessageW(this->_hWnd, EM_SETSEL, selec.start, selec.start + selec.len);
		return *this;
	}

	textbox& select_all() noexcept {
		return this->select({0, -1});
	}

	selection get_selected() const noexcept {
		int p0 = 0, p1 = 0;
		SendMessageW(this->_hWnd, EM_GETSEL,
			reinterpret_cast<WPARAM>(&p0), reinterpret_cast<LPARAM>(&p1));
		return {p0, p1 - p0}; // start, length
	}

	textbox& replace_selected(const wchar_t* t) noexcept {
		SendMessageW(this->_hWnd, EM_REPLACESEL,
			TRUE, reinterpret_cast<LPARAM>(t));
		return *this;
	}

	textbox& replace_selected(const std::wstring& t) noexcept {
		return this->replace_selected(t.c_str());
	}
};

}//namespace wl
