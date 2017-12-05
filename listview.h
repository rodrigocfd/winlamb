/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "internals/native_control.h"
#include "internals/has_enable.h"
#include "internals/has_focus.h"
#include "internals/styler.h"
#include "internals/listview_column_collection.h"
#include "internals/listview_item_collection.h"
#include "internals/member_image_list.h"
#include "subclass.h"
#include "menu.h"

/**
 * hwnd_wrapper
 *  native_control
 *   has_focus
 *    has_enable
 *     listview
 */

namespace wl {

// Wrapper to listview control from Common Controls library.
class listview final :
	public wli::has_enable<
		listview, wli::has_focus<
			listview, wli::native_control<listview>>>
{
private:
	class _styler final : public wli::styler<listview> {
	public:
		explicit _styler(listview* pList) noexcept : styler(pList) { }

		listview& always_show_sel(bool doSet) noexcept {
			return this->set_style(doSet, LVS_SHOWSELALWAYS);
		}

		listview& edit_labels(bool doSet) noexcept {
			return this->set_style(doSet, LVS_EDITLABELS);
		}

		listview& multiple_sel(bool doSet) noexcept {
			return this->set_style(!doSet, LVS_SINGLESEL);
		}

		listview& show_headers(bool doSet) noexcept {
			return this->set_style(doSet, LVS_NOCOLUMNHEADER);
		}

		listview& checkboxes(bool doSet) noexcept {
			ListView_SetExtendedListViewStyleEx(this->target().hwnd(), LVS_EX_CHECKBOXES,
				doSet ? LVS_EX_CHECKBOXES : 0);
			return this->target();
		}

		listview& double_buffer(bool doSet) noexcept {
			ListView_SetExtendedListViewStyleEx(this->target().hwnd(), LVS_EX_DOUBLEBUFFER,
				doSet ? LVS_EX_DOUBLEBUFFER : 0);
			return this->target();
		}

		listview& full_row_select(bool doSet) noexcept {
			ListView_SetExtendedListViewStyleEx(this->target().hwnd(), LVS_EX_FULLROWSELECT,
				doSet ? LVS_EX_FULLROWSELECT : 0);
			return this->target();
		}

		listview& grid_lines(bool doSet) noexcept {
			ListView_SetExtendedListViewStyleEx(this->target().hwnd(), LVS_EX_GRIDLINES,
				doSet ? LVS_EX_GRIDLINES : 0);
			return this->target();
		}

		listview& reorder_header(bool doSet) noexcept {
			ListView_SetExtendedListViewStyleEx(this->target().hwnd(), LVS_EX_HEADERDRAGDROP,
				doSet ? LVS_EX_HEADERDRAGDROP : 0);
			return this->target();
		}
	};

	using _column_collection = wli::listview_column_collection<listview>;
	using _item_collection = wli::listview_item_collection<listview>;

public:
	using item = wli::listview_item<listview>;

	enum class view : WORD {
		DETAILS   = LV_VIEW_DETAILS,
		ICON      = LV_VIEW_ICON,
		LIST      = LV_VIEW_LIST,
		SMALLICON = LV_VIEW_SMALLICON,
		TILE      = LV_VIEW_TILE
	};

private:
	subclass _subclass;
	menu     _contextMenu;

public:
	_styler                          style{this};
	_item_collection                 items{this};
	_column_collection               columns{this};
	wli::member_image_list<listview> imageList16{this, 16}, imageList32{this, 32};

	~listview() {
		this->_contextMenu.destroy();
	}

	listview() noexcept {
		this->imageList16.on_create([this]() noexcept->void {
			ListView_SetImageList(this->hwnd(), this->imageList16.himagelist(), LVSIL_SMALL);
		});

		this->imageList32.on_create([this]() noexcept->void {
			ListView_SetImageList(this->hwnd(), this->imageList32.himagelist(), LVSIL_NORMAL);
		});

		this->_subclass.on_message(WM_GETDLGCODE, [this](wm::getdlgcode p) noexcept->LRESULT {
			if (!p.is_query() && p.vkey_code() == 'A' && p.has_ctrl()) { // Ctrl+A to select all items
				ListView_SetItemState(this->hwnd(), -1, LVIS_SELECTED, LVIS_SELECTED);
				return DLGC_WANTCHARS;
			} else if (!p.is_query() && p.vkey_code() == VK_RETURN) { // send Enter key to parent
				NMLVKEYDOWN nmlvkd = {
					{this->hwnd(), static_cast<WORD>(this->control_id()), LVN_KEYDOWN},
					VK_RETURN, 0
				};
				SendMessageW(GetAncestor(this->hwnd(), GA_PARENT),
					WM_NOTIFY, reinterpret_cast<WPARAM>(this->hwnd()),
					reinterpret_cast<LPARAM>(&nmlvkd));
				return DLGC_WANTALLKEYS;
			} else if (!p.is_query() && p.vkey_code() == VK_APPS) { // context menu keyboard key
				this->_show_context_menu(false, p.has_ctrl(), p.has_shift());
			}
			return DefSubclassProc(this->hwnd(), p.message, p.wParam, p.lParam);
		});

		this->_subclass.on_message(WM_RBUTTONDOWN, [this](wm::rbuttondown p) noexcept->LRESULT {
			this->_show_context_menu(true, p.has_ctrl(), p.has_shift());
			return 0;
		});
	}

	listview& assign(HWND hCtrl) {
		this->native_control::assign(hCtrl);
		return this->_install_subclass();
	}

	listview& assign(HWND hParent, int ctrlId) {
		this->native_control::assign(hParent, ctrlId);
		return this->_install_subclass();
	}

	listview& assign(const hwnd_wrapper* parent, int ctrlId) {
		this->native_control::assign(parent, ctrlId);
		return this->_install_subclass();
	}

	listview& create(HWND hParent, int ctrlId, POINT pos, SIZE size, view viewType = view::DETAILS) {
		this->native_control::create(hParent, ctrlId, nullptr, pos, size, WC_LISTVIEW,
			WS_CHILD | WS_VISIBLE | WS_TABSTOP | static_cast<DWORD>(viewType),
			WS_EX_CLIENTEDGE); // for children, WS_BORDER gives old, flat drawing; always use WS_EX_CLIENTEDGE
		return this->_install_subclass();
	}

	listview& create(const wli::hwnd_wrapper* parent, int ctrlId, POINT pos, SIZE size, view viewType = view::DETAILS) {
		return this->create(parent->hwnd(), ctrlId, pos, size, viewType);
	}

	listview& set_context_menu(int contextMenuId) {
		if (this->_contextMenu.hmenu()) {
			throw std::logic_error("Trying to set listview context menu twice.");
		}
		this->_contextMenu.load_resource_submenu(contextMenuId, 0,
			GetParent(this->hwnd()));
		return *this;
	}

	listview& set_redraw(bool doRedraw) noexcept {
		SendMessageW(this->hwnd(), WM_SETREDRAW,
			static_cast<WPARAM>(static_cast<BOOL>(doRedraw)), 0);
		return *this;
	}

	listview& set_view(view viewType) noexcept {
		ListView_SetView(this->hwnd(), static_cast<DWORD>(viewType));
		return *this;
	}

	view get_view() const noexcept {
		return static_cast<view>(ListView_GetView(this->hwnd()));
	}

private:
	listview& _install_subclass() {
		this->_subclass.install_subclass(*this);
		return *this;
	}

	int _show_context_menu(bool followCursor, bool hasCtrl, bool hasShift) noexcept {
		if (!this->_contextMenu.hmenu()) return -1; // no context menu assigned

		POINT coords{};
		int itemBelowCursor = -1;
		if (followCursor) { // usually fired with a right-click
			LVHITTESTINFO lvhti{};
			GetCursorPos(&lvhti.pt); // relative to screen
			ScreenToClient(this->hwnd(), &lvhti.pt); // now relative to listview
			ListView_HitTest(this->hwnd(), &lvhti); // item below cursor, if any
			coords = lvhti.pt;
			itemBelowCursor = lvhti.iItem; // -1 if none
			if (itemBelowCursor != -1) { // an item was right-clicked
				if (!hasCtrl && !hasShift) {
					if ((ListView_GetItemState(this->hwnd(), itemBelowCursor, LVIS_SELECTED) & LVIS_SELECTED) == 0) {
						// If right-clicked item isn't currently selected, unselect all and select just it.
						ListView_SetItemState(this->hwnd(), -1, 0, LVIS_SELECTED);
						ListView_SetItemState(this->hwnd(), itemBelowCursor, LVIS_SELECTED, LVIS_SELECTED);
					}
					ListView_SetItemState(this->hwnd(), itemBelowCursor, LVIS_FOCUSED, LVIS_FOCUSED); // focus clicked
				}
			} else if (!hasCtrl && !hasShift) {
				ListView_SetItemState(this->hwnd(), -1, 0, LVIS_SELECTED); // unselect all
			}
			this->set_focus(); // because a right-click won't set the focus by default
		} else { // usually fired with the context menu keyboard key
			int itemFocused = ListView_GetNextItem(this->hwnd(), -1, LVNI_FOCUSED);
			if (itemFocused != -1 && ListView_IsItemVisible(this->hwnd(), itemFocused)) { // item focused and visible
				RECT rcItem{};
				ListView_GetItemRect(this->hwnd(), itemFocused, &rcItem, LVIR_BOUNDS); // relative to listview
				coords = {rcItem.left + 16, rcItem.top + (rcItem.bottom - rcItem.top) / 2};
			} else { // no focused and visible item
				coords = {6, 10};
			}
		}

		// The popup menu is created with hDlg as parent, so the menu messages go to it.
		// The lvhti coordinates are relative to listview, and will be mapped into screen-relative.
		this->_contextMenu.show_at_point(GetParent(this->hwnd()), coords, this->hwnd());
		return itemBelowCursor; // -1 if none
	}
};

}//namespace wl
