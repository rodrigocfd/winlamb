/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "internals/base_native_ctrl_pubm.h"
#include "internals/member_image_list.h"
#include "internals/treeview_item_collection.h"
#include "internals/treeview_styler.h"
#include "wnd.h"

namespace wl {

// Wrapper to treeview control from Common Controls library.
class treeview final :
	public wnd,
	public wli::base_native_ctrl_pubm<treeview>
{
public:
	using item = wli::treeview_item<treeview>;

private:
	using _item_collection = wli::treeview_item_collection<treeview>;

	HWND                  _hWnd = nullptr;
	wli::base_native_ctrl _baseNativeCtrl{_hWnd};

public:
	// Wraps window style changes done by Get/SetWindowLongPtr.
	wli::treeview_styler<treeview>   style{this};

	_item_collection                 items{this};
	wli::member_image_list<treeview> imageList16{this, 16};

	treeview() :
		wnd(_hWnd), base_native_ctrl_pubm(_baseNativeCtrl)
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

}//namespace wli
