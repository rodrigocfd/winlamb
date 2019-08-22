/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <string>
#include <vector>
#include <Windows.h>
#include <CommCtrl.h>

namespace wl {
namespace _wli {

class treeview_item final {
public:
private:
	std::reference_wrapper<HWND> _hTree; // the treeview must outlive us
	HTREEITEM                    _hTreeItem;

public:
	treeview_item(HTREEITEM hTreeItem, HWND& hTree) noexcept
		: _hTreeItem(hTreeItem), _hTree(hTree) { }

	HTREEITEM htreeitem() const noexcept {
		return this->_hTreeItem;
	}

	treeview_item get_parent() const noexcept {
		return {TreeView_GetParent(this->_hTree, this->_hTreeItem),
			this->_hTree};
	}

	bool is_root() const noexcept {
		return this->get_parent()._hTreeItem == nullptr;
	}

	treeview_item get_first_child() const noexcept {
		return {TreeView_GetChild(this->_hTree, this->_hTreeItem),
			this->_hTree};
	}

	std::vector<treeview_item> get_children() const {
		std::vector<treeview_item> children;
		treeview_item curIt = this->get_first_child();
		while (curIt.htreeitem()) {
			children.emplace_back(curIt);
			curIt = curIt.get_next_sibling();
		}
		return children;
	}

	treeview_item get_next_sibling() const noexcept {
		return {TreeView_GetNextSibling(this->_hTree, this->_hTreeItem),
			this->_hTree};
	}

	treeview_item add_child(const wchar_t* text, int imagelistIconIndex = -1) noexcept {
		TVINSERTSTRUCTW tvi{};
		tvi.hParent = this->_hTreeItem;
		tvi.hInsertAfter = TVI_LAST;
		tvi.itemex.mask = TVIF_TEXT | (imagelistIconIndex == -1 ? 0 : (TVIF_IMAGE | TVIF_SELECTEDIMAGE));
		tvi.itemex.pszText = const_cast<wchar_t*>(text);
		tvi.itemex.iImage = imagelistIconIndex;
		tvi.itemex.iSelectedImage = imagelistIconIndex;

		return {TreeView_InsertItem(this->_hTree, &tvi),
			this->_hTree}; // return newly added item
	}

	treeview_item add_child(const std::wstring& caption, int imagelistIconIndex = -1) noexcept {
		return this->add_child(caption.c_str(), imagelistIconIndex);
	}

	treeview_item& set_select() noexcept {
		TreeView_SelectItem(this->_hTree, this->_hTreeItem);
		return *this;
	}

	bool is_expanded() const noexcept {
		return (TreeView_GetItemState(this->_hTree, this->_hTreeItem, TVIS_EXPANDED) &
			TVIS_EXPANDED) != 0;
	}

	std::wstring get_text() const {
		wchar_t tmpBuf[MAX_PATH]{}; // arbitrary length
		TVITEMEX tvi{};
		tvi.hItem = this->_hTreeItem;
		tvi.mask = TVIF_TEXT;
		tvi.cchTextMax = ARRAYSIZE(tmpBuf);
		tvi.pszText = tmpBuf;

		TreeView_GetItem(this->_hTree, &tvi);
		return tvi.pszText;
	}

	treeview_item& set_text(const wchar_t* text) noexcept {
		TVITEMEX tvi{};
		tvi.hItem = this->_hTreeItem;
		tvi.mask = TVIF_TEXT;
		tvi.pszText = const_cast<wchar_t*>(text);

		TreeView_SetItem(this->_hTree, &tvi);
		return *this;
	}

	treeview_item& set_text(const std::wstring& text) noexcept {
		return this->set_text(text.c_str());
	}

	LPARAM get_param() const noexcept {
		TVITEMEX tvi{};
		tvi.hItem = this->_hTreeItem;
		tvi.mask = TVIF_PARAM;

		TreeView_GetItem(this->_hTree, &tvi);
		return tvi.lParam;
	}

	treeview_item& set_param(LPARAM lp) noexcept {
		TVITEMEX tvi{};
		tvi.hItem = this->_hTreeItem;
		tvi.mask = TVIF_PARAM;
		tvi.lParam = lp;

		TreeView_SetItem(this->_hTree, &tvi);
		return *this;
	}

	int get_icon_index() const noexcept {
		TVITEMEX tvi{};
		tvi.hItem = this->_hTreeItem;
		tvi.mask = TVIF_IMAGE;

		TreeView_GetItem(this->_hTree, &tvi);
		return tvi.iImage; // return index of icon within image_list
	}

	treeview_item& set_icon_index(int imagelistIconIndex) noexcept {
		TVITEMEX tvi{};
		tvi.hItem = this->_hTreeItem;
		tvi.mask = LVIF_IMAGE | TVIF_SELECTEDIMAGE;
		tvi.iImage = imagelistIconIndex;
		tvi.iSelectedImage = imagelistIconIndex;

		TreeView_SetItem(this->_hTree, &tvi);
		return *this;
	}
};

}//namepace _wli
}//namespace wl
