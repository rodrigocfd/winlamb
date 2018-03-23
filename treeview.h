/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "internals/native_control.h"
#include "internals/w_enable.h"
#include "internals/w_focus.h"
#include "internals/treeview_item_collection.h"
#include "internals/treeview_styler.h"
#include "internals/member_image_list.h"

/**
 * hwnd_base
 *  native_control
 *   w_focus
 *    w_enable
 *     textbox
 */

namespace wl {

// Wrapper to treeview control from Common Controls library.
class treeview final :
	public wli::w_enable<
		treeview, wli::w_focus<
			treeview, wli::native_control<treeview>>>
{
private:
	using _item_collection = wli::treeview_item_collection<treeview>;

public:
	using item = wli::treeview_item<treeview>;

	wli::treeview_styler<treeview>   style{this};
	_item_collection                 items{this};
	wli::member_image_list<treeview> imageList16{this, 16};

	treeview() {
		this->imageList16.on_create([this]() noexcept->void {
			TreeView_SetImageList(this->hwnd(), this->imageList16.himagelist(), TVSIL_NORMAL);
		});
	}

	treeview& create(HWND hParent, int ctrlId, POINT pos, SIZE size) {
		return this->native_control::create(hParent, ctrlId, nullptr, pos, size, WC_TREEVIEW,
			WS_CHILD | WS_VISIBLE | WS_TABSTOP,
			WS_EX_CLIENTEDGE); // for children, WS_BORDER gives old, flat drawing; always use WS_EX_CLIENTEDGE
	}

	treeview& create(const hwnd_base* parent, int ctrlId, POINT pos, SIZE size) {
		return this->create(parent->hwnd(), ctrlId, pos, size);
	}
};

}//namespace wl
