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

	// Returns the number of items in the listview.
	size_t count() const noexcept {
		return ListView_GetItemCount(this->_hList);
	}

	// Adds a new item to the listview, at a given position, with a given image list icon.
	listview_item add_at_pos_with_icon(const wchar_t* caption, size_t positionIndex,
		int imageListIconIndex) noexcept
	{
		LVITEMW lvi{};
		lvi.iItem = static_cast<int>(positionIndex == -1 ? 0x0FFFFFFF : positionIndex);
		lvi.mask = LVIF_TEXT | (imageListIconIndex == -1 ? 0 : LVIF_IMAGE);
		lvi.pszText = const_cast<wchar_t*>(caption);
		lvi.iImage = imageListIconIndex;

		return {static_cast<size_t>(ListView_InsertItem(this->_hList, &lvi)),
			this->_hList}; // return newly inserted item
	}

	// Adds a new item to the listview, at a given position, with a given image list icon.
	listview_item add_at_pos_with_icon(const std::wstring& caption, size_t positionIndex,
		int imageListIconIndex) noexcept
	{
		return this->add_at_pos_with_icon(caption.c_str(),
			positionIndex, imageListIconIndex);
	}

	// Adds a new item to the listview, with a given image list icon.
	listview_item add_with_icon(const wchar_t* caption, int imageListIconIndex) noexcept {
		return this->add_at_pos_with_icon(caption, -1, imageListIconIndex);
	}

	// Adds a new item to the listview, with a given image list icon.
	listview_item add_with_icon(const std::wstring& caption, int imageListIconIndex) noexcept {
		return this->add_at_pos_with_icon(caption, -1, imageListIconIndex);
	}

	// Adds a new item to the listview, at a given position.
	listview_item add_at_pos(const wchar_t* caption, size_t positionIndex) noexcept {
		return this->add_at_pos_with_icon(caption, positionIndex, -1);
	}

	// Adds a new item to the listview, at a given position.
	listview_item add_at_pos(const std::wstring& caption, size_t positionIndex) noexcept {
		return this->add_at_pos_with_icon(caption, positionIndex, -1);
	}

	// Adds a new item to the listview.
	listview_item add(const wchar_t* caption) noexcept {
		return this->add_at_pos_with_icon(caption, -1, -1);
	}

	// Adds a new item to the listview.
	listview_item add(const std::wstring& caption) noexcept {
		return this->add_at_pos_with_icon(caption, -1, -1);
	}

	// Returns a vector with all items in the listview.
	std::vector<listview_item> get_all() const {
		size_t totItems = this->count();
		std::vector<listview_item> items;
		items.reserve(totItems);
		for (size_t i = 0; i < totItems; ++i) {
			items.emplace_back(i, this->_hList); // construct a listview_item object for each item
		}
		return items;
	}

	// Removes all items from the listview.
	listview_item_collection& remove_all() noexcept {
		ListView_DeleteAllItems(this->_hList);
		return *this;
	}

	// Returns the first item which caption matches the given string, case-insensitive, or listview_item::npos if none is found.
	listview_item find(const wchar_t* caption) const noexcept {
		LVFINDINFOW lfi{};
		lfi.flags = LVFI_STRING; // search is case-insensitive
		lfi.psz = caption;
		return {static_cast<size_t>(ListView_FindItem(this->_hList, -1, &lfi)),
			this->_hList};
	}

	// Returns the first item which caption matches the given string, case-insensitive, or listview_item::npos if none is found.
	listview_item find(const std::wstring& caption) const noexcept {
		return this->find(caption.c_str());
	}

	// Returns true if there is an item which caption matches the given string, case-insensitive.
	bool exists(const wchar_t* caption) const noexcept {
		return this->find(caption).index != listview_item::npos;
	}

	// Returns true if there is an item which caption matches the given string, case-insensitive.
	bool exists(const std::wstring& caption) const noexcept {
		return this->exists(caption.c_str());
	}

	// Returns the number of selected items in the listview.
	size_t count_selected() const noexcept {
		return ListView_GetSelectedCount(this->_hList);
	}

	// Selects the items whose indexes have been passed in the array.
	listview_item_collection& select(const std::vector<size_t>& indexes) noexcept {
		for (const size_t index : indexes) {
			ListView_SetItemState(this->_hList,
				static_cast<int>(index), LVIS_SELECTED, LVIS_SELECTED);
		}
		return *this;
	}

	// Selects all items in the listview.
	listview_item_collection& select_all() noexcept {
		ListView_SetItemState(this->_hList, -1, LVIS_SELECTED, LVIS_SELECTED);
		return *this;
	}

	// Unselects all items in the listview.
	listview_item_collection& select_none() noexcept {
		ListView_SetItemState(this->_hList, -1, 0, LVIS_SELECTED);
		return *this;
	}

	// Removes all selected items in the listview.
	listview_item_collection& remove_selected() noexcept {
		SendMessageW(this->_hList, WM_SETREDRAW, static_cast<WPARAM>(FALSE), 0);
		int i = -1;
		while ((i = ListView_GetNextItem(this->_hList, -1, LVNI_SELECTED)) != -1) {
			ListView_DeleteItem(this->_hList, i);
		}
		SendMessageW(this->_hList, WM_SETREDRAW, static_cast<WPARAM>(TRUE), 0);
		return *this;
	}

	// Returns the last item in the listview.
	listview_item get_last() const noexcept {
		return this->operator[](this->count() - 1);
	}

	// Returns the topmost visible item when in list or report view.
	listview_item get_first_visible() const noexcept {
		return {static_cast<size_t>(ListView_GetTopIndex(this->_hList)),
			this->_hList};
	}

	// Returns all selected items in the listview.
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

	// Returns the indexes of all selected items in the listview.
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

	// Returns the focused item in the listview, or listview_item::npos if none.
	listview_item get_focused() const noexcept {
		return {static_cast<size_t>(ListView_GetNextItem(this->_hList, -1, LVNI_FOCUSED)),
			this->_hList};
	}

	// Returns the index of the focused item in the listview.
	size_t get_focused_index() const noexcept {
		int iFoc = ListView_GetNextItem(this->_hList, -1, LVNI_FOCUSED);
		return (iFoc == -1) ? listview_item::npos : static_cast<size_t>(iFoc);
	}

	// Return the texts, from the given column, from the given items.
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
