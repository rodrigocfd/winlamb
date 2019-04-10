/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "treeview_item.h"

namespace wl {
namespace _wli {

template<typename treeviewT>
class treeview_item_collection final {
private:
	using _itemT = treeview_item<treeviewT>;

	treeviewT& _tree;

public:
	treeview_item_collection(treeview_item_collection&&) = default;
	treeview_item_collection& operator=(treeview_item_collection&&) = default; // movable only

	explicit treeview_item_collection(treeviewT* ptree) noexcept
		: _tree(*ptree) { }

	_itemT get_first_root() const noexcept {
		return {TreeView_GetRoot(this->_tree.hwnd()),
			&this->_tree};
	}

	_itemT get_selected() const noexcept {
		return {TreeView_GetSelection(this->_tree.hwnd()),
			&this->_tree};
	}

	std::vector<_itemT> get_roots() const {
		std::vector<_itemT> roots;
		_itemT curIt = this->get_first_root();
		while (curIt.htreeitem()) {
			roots.emplace_back(curIt);
			curIt = curIt.get_next_sibling();
		}
		return roots;
	}

	_itemT get_first_visible() const noexcept {
		return {TreeView_GetFirstVisible(this->_tree.hwnd()),
			&this->_tree};
	}

	_itemT get_last_visible() const noexcept {
		return {TreeView_GetLastVisible(this->_tree.hwnd()),
			&this->_tree};
	}

	size_t get_visible_count() const noexcept {
		return TreeView_GetVisibleCount(this->_tree.hwnd());
	}

	_itemT add_root(const wchar_t* text, int imagelistIconIndex = -1) noexcept {
		TVINSERTSTRUCTW tvi{};
		tvi.hParent = TVI_ROOT;
		tvi.hInsertAfter = TVI_LAST;
		tvi.itemex.mask = TVIF_TEXT | (imagelistIconIndex == -1 ? 0 : (TVIF_IMAGE | TVIF_SELECTEDIMAGE));
		tvi.itemex.pszText = const_cast<wchar_t*>(text);
		tvi.itemex.iImage = imagelistIconIndex;
		tvi.itemex.iSelectedImage = imagelistIconIndex;

		return {TreeView_InsertItem(this->_tree.hwnd(), &tvi),
			&this->_tree}; // return newly added item
	}

	_itemT add_root(const std::wstring& caption, int imagelistIconIndex = -1) noexcept {
		return this->add_root(caption.c_str(), imagelistIconIndex);
	}
};

}//namepace _wli
}//namespace wl
