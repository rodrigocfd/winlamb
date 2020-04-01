/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "internals/base_focus_pubm.h"
#include "internals/base_native_ctrl_pubm.h"
#include "internals/member_image_list.h"
#include "internals/treeview_item_collection.h"
#include "internals/treeview_styler.h"
#include "wnd.h"

namespace wl {

// Wrapper to treeview control from Common Controls library.
class treeview final :
	public wnd,
	public _wli::base_native_ctrl_pubm<treeview>,
	public _wli::base_focus_pubm<treeview>
{
public:
	using item            = _wli::treeview_item;
	using item_collection = _wli::treeview_item_collection;

private:
	HWND                   _hWnd = nullptr;
	_wli::base_native_ctrl _baseNativeCtrl{_hWnd};

public:
	// Wraps window style changes done by Get/SetWindowLongPtr.
	_wli::treeview_styler<treeview>   style{this};

	item_collection                   items{this->_hWnd};
	_wli::member_image_list<treeview> imageList16{this, 16};

	treeview() :
		wnd(_hWnd), base_native_ctrl_pubm(_baseNativeCtrl), base_focus_pubm(_hWnd)
	{
		this->imageList16.on_create([this]() noexcept -> void {
			TreeView_SetImageList(this->_hWnd, this->imageList16.himagelist(), TVSIL_NORMAL);
		});
	}

	treeview(treeview&&) = default;
	treeview& operator=(treeview&&) = default; // movable only

	treeview& create(HWND hParent, int ctrlId, POINT pos, SIZE size) {
		this->_baseNativeCtrl.create(hParent, ctrlId, nullptr, pos, size, WC_TREEVIEW,
			WS_CHILD | WS_VISIBLE | WS_TABSTOP,
			WS_EX_CLIENTEDGE); // for children, WS_BORDER gives old, flat drawing; always use WS_EX_CLIENTEDGE
		return *this;
	}

	treeview& create(const wnd* parent, int ctrlId, POINT pos, SIZE size) {
		return this->create(parent->hwnd(), ctrlId, pos, size);
	}
};

}//namespace _wli
