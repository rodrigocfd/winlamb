/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "internals/base_native_ctrl_impl.h"
#include "internals/styler.h"
#include "str.h"
#include "wnd.h"

namespace wl {

// Wrapper to native combobox control.
class combobox final :
	public wnd,
	public wli::base_native_ctrl_impl<combobox>
{
public:
	enum class sort { SORTED, UNSORTED };

private:
	HWND                  _hWnd = nullptr;
	wli::base_native_ctrl _baseNativeCtrl{_hWnd};

public:
	// Wraps window style changes done by Get/SetWindowLongPtr.
	wli::styler<combobox> style{this};

	combobox() noexcept :
		wnd(_hWnd), base_native_ctrl_impl(_baseNativeCtrl) { }

	combobox(combobox&&) = default;
	combobox& operator=(combobox&&) = default; // movable only

	combobox& create(HWND hParent, int ctrlId, POINT pos,
		LONG width, sort sortType)
	{
		this->_baseNativeCtrl.create(hParent, ctrlId, nullptr,
			pos, {width, 0}, L"combobox",
			WS_CHILD | WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST |
			(sortType == sort::SORTED ? CBS_SORT : 0), 0);
		return *this;
	}

	combobox& create(const wnd* parent, int ctrlId, POINT pos,
		LONG width, sort sortType)
	{
		return this->create(parent->hwnd(), ctrlId, pos, width, sortType);
	}

	size_t count() const noexcept {
		return SendMessageW(this->_hWnd, CB_GETCOUNT, 0, 0);
	}

	size_t get_selected_index() const noexcept {
		return static_cast<size_t>(SendMessageW(this->_hWnd, CB_GETCURSEL, 0, 0));
	}

	combobox& remove_all() noexcept {
		SendMessageW(this->_hWnd, CB_RESETCONTENT, 0, 0);
		return *this;
	}

	combobox& add(const wchar_t* entries, wchar_t delimiter = L'|') {
		wchar_t delim[2]{delimiter, L'\0'};
		std::vector<std::wstring> vals = str::split(entries, delim);
		for (const std::wstring& s : vals) {
			SendMessageW(this->_hWnd, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(s.c_str()));
		}
		return *this;
	}

	combobox& add(std::initializer_list<const wchar_t*> entries) noexcept {
		for (const wchar_t* s : entries) {
			SendMessageW(this->_hWnd, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(s));
		}
		return *this;
	}

	std::wstring get_text(size_t index) const {
		std::wstring buf;
		size_t len = SendMessageW(this->_hWnd, CB_GETLBTEXTLEN, index, 0);
		if (len) {
			buf.resize(len, L'\0');
			SendMessageW(this->_hWnd, CB_GETLBTEXT, index, reinterpret_cast<LPARAM>(&buf[0]));
			buf.resize(len);
		}
		return buf;
	}

	std::wstring get_selected_text() const {
		return this->get_text(this->get_selected_index());
	}

	combobox& select(size_t index) noexcept {
		SendMessageW(this->_hWnd, CB_SETCURSEL, index, 0);
		return *this;
	}
};

}//namespace wl
