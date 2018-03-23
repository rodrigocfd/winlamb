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
#include "str.h"

/**
 * hwnd_base
 *  native_control
 *   w_text
 *    w_focus
 *     w_enable
 *      combobox
 */

namespace wl {

// Wrapper to native combobox control.
class combobox final :
	public wli::w_enable<
		combobox, wli::w_focus<
			combobox, wli::w_text<
				combobox, wli::native_control<combobox>>>>
{
private:
	class _styler final : public wli::styler<combobox> {
	public:
		explicit _styler(combobox* pCombobox) noexcept : styler(pCombobox) { }
	};

public:
	enum class sort { SORTED, UNSORTED };

	_styler style{this};

	combobox& create(HWND hParent, int ctrlId, POINT pos, LONG width, sort sortType) {
		return this->native_control::create(hParent, ctrlId, nullptr,
			pos, {width,0}, L"combobox",
			WS_CHILD | WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST |
			(sortType == sort::SORTED ? CBS_SORT : 0), 0);
	}

	combobox& create(const hwnd_base* parent, int ctrlId, POINT pos,
		LONG width, sort sortType)
	{
		return this->create(parent->hwnd(), ctrlId, pos, width, sortType);
	}

	size_t item_count() const noexcept {
		return SendMessageW(this->hwnd(), CB_GETCOUNT, 0, 0);
	}

	size_t item_get_selected() const noexcept {
		return static_cast<size_t>(SendMessageW(this->hwnd(), CB_GETCURSEL, 0, 0));
	}

	combobox& item_remove_all() noexcept {
		SendMessageW(this->hwnd(), CB_RESETCONTENT, 0, 0);
		return *this;
	}

	combobox& item_add(const wchar_t* entries, wchar_t delimiter = L'|') {
		wchar_t delim[2]{delimiter, L'\0'};
		std::vector<std::wstring> vals = str::explode(entries, delim);
		for (const std::wstring& s : vals) {
			SendMessageW(this->hwnd(), CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(s.c_str()));
		}
		return *this;
	}

	combobox& item_add(std::initializer_list<const wchar_t*> entries) noexcept {
		for (const wchar_t* s : entries) {
			SendMessageW(this->hwnd(), CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(s));
		}
		return *this;
	}

	std::wstring item_get_text(size_t index) const {
		std::wstring buf;
		size_t len = SendMessageW(this->hwnd(), CB_GETLBTEXTLEN, index, 0);
		if (len) {
			buf.resize(len, L'\0');
			SendMessageW(this->hwnd(), CB_GETLBTEXT, index, reinterpret_cast<LPARAM>(&buf[0]));
			buf.resize(len);
		}
		return buf;
	}

	std::wstring item_get_selected_text() const {
		return this->item_get_text(this->item_get_selected());
	}

	combobox& item_set_selected(size_t index) noexcept {
		SendMessageW(this->hwnd(), CB_SETCURSEL, index, 0);
		return *this;
	}
};

}//namespace wl
