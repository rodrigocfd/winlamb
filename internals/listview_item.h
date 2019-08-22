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
namespace _wli {

class listview_item final {
public:
	static const size_t npos = -1;

private:
	std::reference_wrapper<HWND> _hList; // the listview must outlive us
	size_t                       _index;

public:
	const size_t& index;

	listview_item(size_t itemIndex, HWND& hList) noexcept
		: _index(itemIndex), _hList(hList), index(_index) { }

	void remove() const noexcept {
		ListView_DeleteItem(this->_hList, this->_index);
	}

	void swap_with(size_t itemIndex) {
		SendMessageW(this->_hList, WM_SETREDRAW, static_cast<WPARAM>(FALSE), 0);

		listview_item newItem{itemIndex, this->_hList}; // make a copy
		size_t numCols = Header_GetItemCount(ListView_GetHeader(this->_hList));
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

		SendMessageW(this->_hList, WM_SETREDRAW, static_cast<WPARAM>(TRUE), 0);
	}

	listview_item& ensure_visible() noexcept {
		if (ListView_GetView(this->_hList) == LV_VIEW_DETAILS) {
			// In details view, ListView_EnsureVisible() won't center the item vertically.
			// This new implementation has this behavior.
			RECT rc{};
			GetClientRect(this->_hList, &rc);
			int cyList = rc.bottom; // total height of list

			SecureZeroMemory(&rc, sizeof(rc));
			LVITEMINDEX lvii{};
			lvii.iItem = ListView_GetTopIndex(this->_hList); // 1st visible item
			ListView_GetItemIndexRect(this->_hList, &lvii, 0, LVIR_BOUNDS, &rc);
			int cyItem = rc.bottom - rc.top; // height of a single item
			int xTop = rc.top; // topmost X of 1st visible item

			SecureZeroMemory(&rc, sizeof(rc));
			SecureZeroMemory(&lvii, sizeof(lvii));
			lvii.iItem = static_cast<int>(this->_index);
			ListView_GetItemIndexRect(this->_hList, &lvii, 0, LVIR_BOUNDS, &rc);
			int xUs = rc.top; // our current X

			if (xUs < xTop || xUs > xTop + cyList) { // if we're not visible
				ListView_Scroll(this->_hList, 0, xUs - xTop - cyList / 2 + cyItem * 2);
			}
		} else {
			ListView_EnsureVisible(this->_hList, this->_index, FALSE);
		}
		return *this;
	}

	bool is_visible() const noexcept {
		return ListView_IsItemVisible(this->_hList, this->_index) == TRUE;
	}

	listview_item& set_select(bool select) noexcept {
		ListView_SetItemState(this->_hList, this->_index,
			select ? LVIS_SELECTED : 0, LVIS_SELECTED);
		return *this;
	}

	bool is_selected() const noexcept {
		return (ListView_GetItemState(this->_hList,
			this->_index, LVIS_SELECTED) & LVIS_SELECTED) != 0;
	}

	listview_item& set_focus() noexcept {
		ListView_SetItemState(this->_hList,
			this->_index, LVIS_FOCUSED, LVIS_FOCUSED);
		return *this;
	}

	bool is_focused() const noexcept {
		return (ListView_GetItemState(this->_hList,
			this->_index, LVIS_FOCUSED) & LVIS_FOCUSED) != 0;
	}

	RECT get_rect() const noexcept {
		RECT rc{};
		ListView_GetItemRect(this->_hList, this->_index, &rc, LVIR_BOUNDS);
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
				SendMessageW(this->_hList, LVM_GETITEMTEXT,
					this->_index, reinterpret_cast<LPARAM>(&lvi)) );
		} while (charsWrittenWithoutNull == baseBufLen - 1); // to break, must have at least 1 char gap

		buf.resize(lstrlenW(buf.c_str())); // str::trim_nulls()
		return buf;
	}

	listview_item& set_text(const wchar_t* text, size_t columnIndex = 0) noexcept {
		ListView_SetItemText(this->_hList, this->_index,
			static_cast<int>(columnIndex), const_cast<wchar_t*>(text));
		return *this;
	}

	listview_item& set_text(const std::wstring& text, size_t columnIndex = 0) noexcept {
		return this->set_text(text.c_str(), columnIndex);
	}

	LPARAM get_param() const noexcept {
		LVITEMW lvi{};
		lvi.iItem = static_cast<int>(this->_index);
		lvi.mask = LVIF_PARAM;

		ListView_GetItem(this->_hList, &lvi);
		return lvi.lParam;
	}

	listview_item& set_param(LPARAM lp) noexcept {
		LVITEMW lvi{};
		lvi.iItem = static_cast<int>(this->_index);
		lvi.mask = LVIF_PARAM;
		lvi.lParam = lp;

		ListView_SetItem(this->_hList, &lvi);
		return *this;
	}

	int get_icon_index() const noexcept {
		LVITEMW lvi{};
		lvi.iItem = static_cast<int>(this->_index);
		lvi.mask = LVIF_IMAGE;

		ListView_GetItem(this->_hList, &lvi);
		return lvi.iImage; // return index of icon within image_list
	}

	listview_item& set_icon_index(int imagelistIconIndex) noexcept {
		LVITEMW lvi{};
		lvi.iItem = static_cast<int>(this->_index);
		lvi.mask = LVIF_IMAGE;
		lvi.iImage = imagelistIconIndex;

		ListView_SetItem(this->_hList, &lvi);
		return *this;
	}
};

}//namepace _wli
}//namespace wl
