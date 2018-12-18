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
namespace wli {

template<typename listviewT>
class listview_item_collection final {
private:
	using _itemT = listview_item<listviewT>;

	listviewT& _list;

public:
	listview_item_collection(listview_item_collection&&) = default;
	listview_item_collection& operator=(listview_item_collection&&) = default; // movable only

	explicit listview_item_collection(listviewT* pList) noexcept
		: _list(*pList) { }

	_itemT operator[](size_t itemIndex) const noexcept {
		return {itemIndex, this->_list};
	}

	size_t count() const noexcept {
		return ListView_GetItemCount(this->_list.hwnd());
	}

	_itemT add(const wchar_t* caption, int imagelistIconIndex = -1, size_t positionIndex = _itemT::npos) noexcept {
		LVITEMW lvi{};
		lvi.iItem = static_cast<int>(positionIndex == -1 ? 0x0FFFFFFF : positionIndex);
		lvi.mask = LVIF_TEXT | (imagelistIconIndex == -1 ? 0 : LVIF_IMAGE);
		lvi.pszText = const_cast<wchar_t*>(caption);
		lvi.iImage = imagelistIconIndex;

		return {static_cast<size_t>(ListView_InsertItem(this->_list.hwnd(), &lvi)),
			this->_list}; // return newly inserted item
	}

	_itemT add(const std::wstring& caption, int imagelistIconIndex = -1, size_t positionIndex = _itemT::npos) noexcept {
		return this->add(caption.c_str(), imagelistIconIndex, positionIndex);
	}

	std::vector<_itemT> get_all() const {
		size_t totItems = this->count();
		std::vector<_itemT> items; // a big array with all items in list
		items.reserve(totItems);
		for (size_t i = 0; i < totItems; ++i) {
			items.emplace_back(i, this->_list);
		}
		return items;
	}

	listviewT& remove_all() noexcept {
		ListView_DeleteAllItems(this->_list.hwnd());
		return this->_list;
	}

	_itemT find(const wchar_t* caption) const noexcept {
		LVFINDINFOW lfi{};
		lfi.flags = LVFI_STRING; // search is case-insensitive
		lfi.psz = caption;
		return {static_cast<size_t>(ListView_FindItem(this->_list.hwnd(), -1, &lfi)),
			this->_list}; // returns -1 if not found
	}

	_itemT find(const std::wstring& caption) const noexcept {
		return this->find(caption.c_str());
	}

	bool exists(const wchar_t* caption) const noexcept {
		return this->find(caption).index != _itemT::npos;
	}

	bool exists(const std::wstring& caption) const noexcept {
		return this->exists(caption.c_str());
	}

	size_t count_selected() const noexcept {
		return ListView_GetSelectedCount(this->_list.hwnd());
	}

	listviewT& select(const std::vector<size_t>& indexes) const noexcept {
		// Select the items whose indexes have been passed in the array.
		for (const size_t index : indexes) {
			ListView_SetItemState(this->_list.hwnd(),
				static_cast<int>(index), LVIS_SELECTED, LVIS_SELECTED);
		}
		return this->_list;
	}

	listviewT& select_all() const noexcept {
		ListView_SetItemState(this->_list.hwnd(), -1, LVIS_SELECTED, LVIS_SELECTED);
		return this->_list;
	}

	listviewT& select_none() const noexcept {
		ListView_SetItemState(this->_list.hwnd(), -1, 0, LVIS_SELECTED);
		return this->_list;
	}

	listviewT& remove_selected() const noexcept {
		this->_list.set_redraw(false);
		int i = -1;
		while ((i = ListView_GetNextItem(this->_list.hwnd(), -1, LVNI_SELECTED)) != -1) {
			ListView_DeleteItem(this->_list.hwnd(), i);
		}
		return this->_list.set_redraw(true);
	}

	_itemT get_last() const noexcept {
		return this->operator[](this->count() - 1);
	}

	_itemT get_first_visible() const noexcept {
		return {ListView_GetTopIndex(this->_list.hwnd()), this->_list};
	}

	std::vector<_itemT> get_selected() const {
		std::vector<_itemT> items;
		items.reserve(this->count_selected());

		int iBase = -1;
		for (;;) {
			iBase = ListView_GetNextItem(this->_list.hwnd(), iBase, LVNI_SELECTED);
			if (iBase == -1) break;
			items.emplace_back(iBase, this->_list);
		}
		return items;
	}

	std::vector<size_t> get_selected_indexes() const {
		std::vector<size_t> indexes;
		indexes.reserve(this->count_selected());

		int iBase = -1;
		for (;;) {
			iBase = ListView_GetNextItem(this->_list.hwnd(), iBase, LVNI_SELECTED);
			if (iBase == -1) break;
			indexes.emplace_back(iBase);
		}
		return indexes;
	}

	_itemT get_focused() const noexcept {
		return {static_cast<size_t>(ListView_GetNextItem(this->_list.hwnd(), -1, LVNI_FOCUSED)),
			this->_list};
	}

	size_t get_focused_index() const noexcept {
		int iFoc = ListView_GetNextItem(this->_list.hwnd(), -1, LVNI_FOCUSED);
		return iFoc == -1 ? _itemT::npos : static_cast<size_t>(iFoc);
	}

	std::vector<std::wstring> get_texts(const std::vector<_itemT>& itemsToGet, size_t columnIndex = 0) const {
		std::vector<std::wstring> texts;
		texts.reserve(itemsToGet.size());
		for (const _itemT& oneItem : itemsToGet) {
			texts.emplace_back(oneItem.get_text(columnIndex));
		}
		return texts;
	}
};

}//namespace wli
}//namespace wl
