/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <vector>
#include "listview_item.h"

namespace wl {
namespace _wli {

class listview_item_collection final {
private:
	std::reference_wrapper<HWND> _hList; // the listview must outlive us

public:
	listview_item_collection(listview_item_collection&&) = default;
	listview_item_collection& operator=(listview_item_collection&&) = default; // movable only

	explicit listview_item_collection(HWND& hList) noexcept :
		_hList(hList) { }

	listview_item operator[](size_t itemIndex) const noexcept {
		return {itemIndex, this->_hList};
	}

	size_t count() const noexcept {
		return ListView_GetItemCount(this->_hList);
	}

	listview_item add(const wchar_t* caption, int imagelistIconIndex = -1,
		size_t positionIndex = listview_item::npos) noexcept
	{
		LVITEMW lvi{};
		lvi.iItem = static_cast<int>(positionIndex == -1 ? 0x0FFFFFFF : positionIndex);
		lvi.mask = LVIF_TEXT | (imagelistIconIndex == -1 ? 0 : LVIF_IMAGE);
		lvi.pszText = const_cast<wchar_t*>(caption);
		lvi.iImage = imagelistIconIndex;

		return {static_cast<size_t>(ListView_InsertItem(this->_hList, &lvi)),
			this->_hList}; // return newly inserted item
	}

	listview_item add(const std::wstring& caption, int imagelistIconIndex = -1,
		size_t positionIndex = listview_item::npos) noexcept
	{
		return this->add(caption.c_str(), imagelistIconIndex, positionIndex);
	}

	std::vector<listview_item> get_all() const {
		size_t totItems = this->count();
		std::vector<listview_item> items; // a big array with all items in list
		items.reserve(totItems);
		for (size_t i = 0; i < totItems; ++i) {
			items.emplace_back(i, this->_hList);
		}
		return items;
	}

	listview_item_collection& remove_all() noexcept {
		ListView_DeleteAllItems(this->_hList);
		return *this;
	}

	listview_item find(const wchar_t* caption) const noexcept {
		LVFINDINFOW lfi{};
		lfi.flags = LVFI_STRING; // search is case-insensitive
		lfi.psz = caption;
		return {static_cast<size_t>(ListView_FindItem(this->_hList, -1, &lfi)),
			this->_hList}; // returns listview_item::npos if not found
	}

	listview_item find(const std::wstring& caption) const noexcept {
		return this->find(caption.c_str());
	}

	bool exists(const wchar_t* caption) const noexcept {
		return this->find(caption).index != listview_item::npos;
	}

	bool exists(const std::wstring& caption) const noexcept {
		return this->exists(caption.c_str());
	}

	size_t count_selected() const noexcept {
		return ListView_GetSelectedCount(this->_hList);
	}

	listview_item_collection& select(const std::vector<size_t>& indexes) noexcept {
		// Select the items whose indexes have been passed in the array.
		for (const size_t index : indexes) {
			ListView_SetItemState(this->_hList,
				static_cast<int>(index), LVIS_SELECTED, LVIS_SELECTED);
		}
		return *this;
	}

	listview_item_collection& select_all() noexcept {
		ListView_SetItemState(this->_hList, -1, LVIS_SELECTED, LVIS_SELECTED);
		return *this;
	}

	listview_item_collection& select_none() noexcept {
		ListView_SetItemState(this->_hList, -1, 0, LVIS_SELECTED);
		return *this;
	}

	listview_item_collection& remove_selected() noexcept {
		SendMessageW(this->_hList, WM_SETREDRAW, static_cast<WPARAM>(FALSE), 0);
		int i = -1;
		while ((i = ListView_GetNextItem(this->_hList, -1, LVNI_SELECTED)) != -1) {
			ListView_DeleteItem(this->_hList, i);
		}
		SendMessageW(this->_hList, WM_SETREDRAW, static_cast<WPARAM>(TRUE), 0);
	}

	listview_item get_last() const noexcept {
		return this->operator[](this->count() - 1);
	}

	listview_item get_first_visible() const noexcept {
		return {static_cast<size_t>(ListView_GetTopIndex(this->_hList)),
			this->_hList};
	}

	std::vector<listview_item> get_selected() const {
		std::vector<listview_item> items;
		items.reserve(this->count_selected());

		int iBase = -1;
		for (;;) {
			iBase = ListView_GetNextItem(this->_hList, iBase, LVNI_SELECTED);
			if (iBase == -1) break;
			items.emplace_back(iBase, this->_hList);
		}
		return items;
	}

	std::vector<size_t> get_selected_indexes() const {
		std::vector<size_t> indexes;
		indexes.reserve(this->count_selected());

		int iBase = -1;
		for (;;) {
			iBase = ListView_GetNextItem(this->_hList, iBase, LVNI_SELECTED);
			if (iBase == -1) break;
			indexes.emplace_back(iBase);
		}
		return indexes;
	}

	listview_item get_focused() const noexcept {
		return {static_cast<size_t>(ListView_GetNextItem(this->_hList, -1, LVNI_FOCUSED)),
			this->_hList};
	}

	size_t get_focused_index() const noexcept {
		int iFoc = ListView_GetNextItem(this->_hList, -1, LVNI_FOCUSED);
		return (iFoc == -1) ? listview_item::npos : static_cast<size_t>(iFoc);
	}

	std::vector<std::wstring> get_texts(const std::vector<listview_item>& itemsToGet,
		size_t columnIndex = 0) const
	{
		std::vector<std::wstring> texts;
		texts.reserve(itemsToGet.size());
		for (const listview_item& oneItem : itemsToGet) {
			texts.emplace_back(oneItem.get_text(columnIndex));
		}
		return texts;
	}
};

}//namepace _wli
}//namespace wl
