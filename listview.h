/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "internals/base_native_ctrl_pubm.h"
#include "internals/listview_column_collection.h"
#include "internals/listview_item_collection.h"
#include "internals/listview_styler.h"
#include "internals/member_image_list.h"
#include "menu.h"
#include "subclass.h"
#include "wnd.h"

namespace wl {

// Wrapper to listview control from Common Controls library.
class listview final :
	public wnd,
	public _wli::base_native_ctrl_pubm<listview>
{
public:
	using item              = _wli::listview_item;
	using item_collection   = _wli::listview_item_collection;
	using column_collection = _wli::listview_column_collection;

	enum class view : WORD {
		DETAILS   = LV_VIEW_DETAILS,
		ICON      = LV_VIEW_ICON,
		LIST      = LV_VIEW_LIST,
		SMALLICON = LV_VIEW_SMALLICON,
		TILE      = LV_VIEW_TILE
	};

private:
	HWND                   _hWnd = nullptr;
	_wli::base_native_ctrl _baseNativeCtrl{_hWnd};
	subclass               _subclass;
	menu                   _contextMenu;

public:
	// Wraps window style changes done by Get/SetWindowLongPtr.
	_wli::listview_styler<listview>   style{this};

	item_collection                   items{this->_hWnd};
	column_collection                 columns{this->_hWnd};
	_wli::member_image_list<listview> imageList16{this, 16}, imageList32{this, 32};

	~listview() {
		this->_contextMenu.destroy();
	}

	listview() :
		wnd(_hWnd), base_native_ctrl_pubm(_baseNativeCtrl)
	{
		this->imageList16.on_create([this]() noexcept -> void {
			ListView_SetImageList(this->_hWnd, this->imageList16.himagelist(), LVSIL_SMALL);
		});

		this->imageList32.on_create([this]() noexcept -> void {
			ListView_SetImageList(this->_hWnd, this->imageList32.himagelist(), LVSIL_NORMAL);
		});

		this->_subclass.on_message(WM_GETDLGCODE, [this](wm::getdlgcode p) noexcept -> LRESULT {
			if (!p.is_query() && p.vkey_code() == 'A' && p.has_ctrl()) { // Ctrl+A to select all items
				ListView_SetItemState(this->_hWnd, -1, LVIS_SELECTED, LVIS_SELECTED);
				return DLGC_WANTCHARS;
			} else if (!p.is_query() && p.vkey_code() == VK_RETURN) { // send Enter key to parent
				NMLVKEYDOWN nmlvkd = {
					{this->_hWnd, static_cast<WORD>(this->ctrl_id()), LVN_KEYDOWN},
					VK_RETURN, 0
				};
				SendMessageW(GetAncestor(this->_hWnd, GA_PARENT),
					WM_NOTIFY, reinterpret_cast<WPARAM>(this->_hWnd),
					reinterpret_cast<LPARAM>(&nmlvkd));
				return DLGC_WANTALLKEYS;
			} else if (!p.is_query() && p.vkey_code() == VK_APPS) { // context menu keyboard key
				this->_show_context_menu(false, p.has_ctrl(), p.has_shift());
			}
			return DefSubclassProc(this->_hWnd, p.message, p.wParam, p.lParam);
		});

		this->_subclass.on_message(WM_RBUTTONDOWN, [this](wm::rbuttondown p) noexcept -> LRESULT {
			this->_show_context_menu(true, p.has_ctrl(), p.has_shift());
			return 0;
		});
	}

	listview(listview&&) = default;
	listview& operator=(listview&&) = default; // movable only

	// Ties this class instance to an existing native control.
	listview& assign(HWND hCtrl) {
		this->base_native_ctrl_pubm::assign(hCtrl); // hides base method
		return this->_install_subclass();
	}

	// Ties this class instance to an existing native control.
	listview& assign(HWND hParent, int ctrlId) {
		this->base_native_ctrl_pubm::assign(hParent, ctrlId); // hides base method
		return this->_install_subclass();
	}

	// Ties this class instance to an existing native control.
	listview& assign(const wnd* parent, int ctrlId) {
		this->base_native_ctrl_pubm::assign(parent, ctrlId); // hides base method
		return this->_install_subclass();
	}

	listview& set_context_menu(int contextMenuId) {
		if (this->_contextMenu.hmenu()) {
			throw std::logic_error("Trying to set listview context menu twice.");
		}
		this->_contextMenu.load_from_resource_submenu(contextMenuId, 0,
			GetParent(this->_hWnd));
		return *this;
	}

	listview& set_redraw(bool doRedraw) noexcept {
		SendMessageW(this->_hWnd, WM_SETREDRAW,
			static_cast<WPARAM>(static_cast<BOOL>(doRedraw)), 0);
		return *this;
	}

	listview& set_view(view viewType) noexcept {
		ListView_SetView(this->_hWnd, static_cast<DWORD>(viewType));
		return *this;
	}

	view get_view() const noexcept {
		return static_cast<view>(ListView_GetView(this->_hWnd));
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
			ScreenToClient(this->_hWnd, &lvhti.pt); // now relative to listview
			ListView_HitTest(this->_hWnd, &lvhti); // item below cursor, if any
			coords = lvhti.pt;
			itemBelowCursor = lvhti.iItem; // -1 if none
			if (itemBelowCursor != -1) { // an item was right-clicked
				if (!hasCtrl && !hasShift) {
					if ((ListView_GetItemState(this->_hWnd, itemBelowCursor, LVIS_SELECTED) & LVIS_SELECTED) == 0) {
						// If right-clicked item isn't currently selected, unselect all and select just it.
						ListView_SetItemState(this->_hWnd, -1, 0, LVIS_SELECTED);
						ListView_SetItemState(this->_hWnd, itemBelowCursor, LVIS_SELECTED, LVIS_SELECTED);
					}
					ListView_SetItemState(this->_hWnd, itemBelowCursor, LVIS_FOCUSED, LVIS_FOCUSED); // focus clicked
				}
			} else if (!hasCtrl && !hasShift) {
				ListView_SetItemState(this->_hWnd, -1, 0, LVIS_SELECTED); // unselect all
			}
			SetFocus(this->_hWnd); // because a right-click won't set the focus by default
		} else { // usually fired with the context menu keyboard key
			int itemFocused = ListView_GetNextItem(this->_hWnd, -1, LVNI_FOCUSED);
			if (itemFocused != -1 && ListView_IsItemVisible(this->_hWnd, itemFocused)) { // item focused and visible
				RECT rcItem{};
				ListView_GetItemRect(this->_hWnd, itemFocused, &rcItem, LVIR_BOUNDS); // relative to listview
				coords = {rcItem.left + 16, rcItem.top + (rcItem.bottom - rcItem.top) / 2};
			} else { // no focused and visible item
				coords = {6, 10};
			}
		}

		// The popup menu is created with hDlg as parent, so the menu messages go to it.
		// The lvhti coordinates are relative to listview, and will be mapped into screen-relative.
		this->_contextMenu.show_at_point(GetParent(this->_hWnd), coords, this->_hWnd);
		return itemBelowCursor; // -1 if none
	}
};

}//namespace wl
