/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <string>
#include <Windows.h>
#include <CommCtrl.h>

namespace wl {
namespace wli {

template<typename listviewT>
class listview_item final {
public:
	static const size_t npos = -1;

private:
	listviewT& _list;
	size_t     _index;

public:
	const size_t& index;

	listview_item(size_t itemIndex, listviewT& list)
		: _index(itemIndex), _list(list), index(_index) { }

	void remove() const {
		ListView_DeleteItem(this->_list.hwnd(), this->_index);
	}

	void swap_with(size_t itemIndex) {
		this->_list.set_redraw(false);

		item newItem = this->_list.items[itemIndex]; // make a copy
		size_t numCols = this->_list.columns.count();
		std::wstring tmpStr;
		for (size_t c = 0; c < numCols; ++c) { // swap texts of all columns
			tmpStr = this->get_text(c);
			this->set_text(newItem.get_text(c), c);
			newItem.set_text(tmpStr, c);
		}

		LPARAM oldp = this->get_param(); // swap LPARAMs
		this->set_param(newItem.get_param());
		newItem.set_param(oldp);

		int oldi = this->get_icon_index(); // swap icons
		this->set_icon_index(newItem.get_icon_index());
		newItem.set_icon_index(oldi);

		this->_list.set_redraw(true);
	}

	listview_item& ensure_visible() {
		if (this->_list.get_view() == view::DETAILS) {
			// In details view, ListView_EnsureVisible() won't center the item vertically.
			// This new implementation has this behavior.
			RECT rc{};
			GetClientRect(this->_list.hwnd(), &rc);
			int cyList = rc.bottom; // total height of list

			SecureZeroMemory(&rc, sizeof(rc));
			LVITEMINDEX lvii{};
			lvii.iItem = ListView_GetTopIndex(this->_list.hwnd()); // 1st visible item
			ListView_GetItemIndexRect(this->_list.hwnd(), &lvii, 0, LVIR_BOUNDS, &rc);
			int cyItem = rc.bottom - rc.top; // height of a single item
			int xTop = rc.top; // topmost X of 1st visible item

			SecureZeroMemory(&rc, sizeof(rc));
			SecureZeroMemory(&lvii, sizeof(lvii));
			lvii.iItem = static_cast<int>(this->_index);
			ListView_GetItemIndexRect(this->_list.hwnd(), &lvii, 0, LVIR_BOUNDS, &rc);
			int xUs = rc.top; // our current X

			if (xUs < xTop || xUs > xTop + cyList) { // if we're not visible
				ListView_Scroll(this->_list.hwnd(), 0, xUs - xTop - cyList / 2 + cyItem * 2);
			}
		} else {
			ListView_EnsureVisible(this->_list.hwnd(), this->_index, FALSE);
		}
		return *this;
	}

	bool is_visible() const {
		return ListView_IsItemVisible(this->_list.hwnd(), this->_index) == TRUE;
	}

	listview_item& set_select(bool select) {
		ListView_SetItemState(this->_list.hwnd(), this->_index,
			select ? LVIS_SELECTED : 0, LVIS_SELECTED);
		return *this;
	}

	bool is_selected() const {
		return (ListView_GetItemState(this->_list.hwnd(),
			this->_index, LVIS_SELECTED) & LVIS_SELECTED) != 0;
	}

	listview_item& set_focus() {
		ListView_SetItemState(this->_list.hwnd(),
			this->_index, LVIS_FOCUSED, LVIS_FOCUSED);
		return *this;
	}

	bool is_focused() const {
		return (ListView_GetItemState(this->_list.hwnd(),
			this->_index, LVIS_FOCUSED) & LVIS_FOCUSED) != 0;
	}

	RECT get_rect() const {
		RECT rc{};
		ListView_GetItemRect(this->_list.hwnd(), this->_index, &rc, LVIR_BOUNDS);
		return rc;
	}

	std::wstring get_text(size_t columnIndex = 0) const {
		// http://forums.codeguru.com/showthread.php?351972-Getting-listView-item-text-length
		LVITEMW lvi{};
		lvi.iItem = static_cast<int>(this->_index);
		lvi.iSubItem = static_cast<int>(columnIndex);

		// Notice that, since strings' size always increase, if the buffer
		// was previously allocated with a value bigger than our 1st step,
		// this will speed up the size checks.

		std::wstring buf(64, L'\0'); // speed-up 1st allocation
		int baseBufLen = 0;
		int charsWrittenWithoutNull = 0;
		do {
			baseBufLen += 64; // buffer increasing step, arbitrary!
			buf.resize(baseBufLen);
			lvi.cchTextMax = baseBufLen;
			lvi.pszText = &buf[0];
			charsWrittenWithoutNull = static_cast<int>(
				SendMessageW(this->_list.hwnd(), LVM_GETITEMTEXT,
					this->_index, reinterpret_cast<LPARAM>(&lvi)) );
		} while (charsWrittenWithoutNull == baseBufLen - 1); // to break, must have at least 1 char gap

		buf.resize(lstrlenW(buf.c_str())); // str::trim_nulls()
		return buf;
	}

	listview_item& set_text(const wchar_t* text, size_t columnIndex = 0) {
		ListView_SetItemText(this->_list.hwnd(), this->_index,
			static_cast<int>(columnIndex), const_cast<wchar_t*>(text));
		return *this;
	}

	listview_item& set_text(const std::wstring& text, size_t columnIndex = 0) {
		return this->set_text(text.c_str(), columnIndex);
	}

	LPARAM get_param() const {
		LVITEMW lvi{};
		lvi.iItem = static_cast<int>(this->_index);
		lvi.mask = LVIF_PARAM;

		ListView_GetItem(this->_list.hwnd(), &lvi);
		return lvi.lParam;
	}

	listview_item& set_param(LPARAM lp) {
		LVITEMW lvi{};
		lvi.iItem = static_cast<int>(this->_index);
		lvi.mask = LVIF_PARAM;
		lvi.lParam = lp;

		ListView_SetItem(this->_list.hwnd(), &lvi);
		return *this;
	}

	int get_icon_index() const {
		LVITEMW lvi{};
		lvi.iItem = static_cast<int>(this->_index);
		lvi.mask = LVIF_IMAGE;

		ListView_GetItem(this->_list.hwnd(), &lvi);
		return lvi.iImage; // return index of icon within image_list
	}

	listview_item& set_icon_index(int imagelistIconIndex) {
		LVITEMW lvi{};
		lvi.iItem = static_cast<int>(this->_index);
		lvi.mask = LVIF_IMAGE;
		lvi.iImage = imagelistIconIndex;

		ListView_SetItem(this->_list.hwnd(), &lvi);
		return *this;
	}
};

}//namespace wli
}//namespace wl
