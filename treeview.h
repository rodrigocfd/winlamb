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
#include "internals/treeview_item_collection.h"
#include "internals/member_image_list.h"

/**
 * hwnd_wrapper
 *  native_control
 *   has_focus
 *    has_enable
 *     textbox
 */

namespace wl {

// Wrapper to treeview control from Common Controls library.
class treeview final :
	public wli::has_enable<
		treeview, wli::has_focus<
			treeview, wli::native_control<treeview>>>
{
private:
	class _styler final : public wli::styler<treeview> {
	public:
		explicit _styler(treeview* pTree) : styler(pTree) { }

		treeview& always_show_sel(bool doSet) {
			return this->set_style(doSet, TVS_SHOWSELALWAYS);
		}

		treeview& lines_and_buttons(bool doSet) {
			return this->set_style(doSet,
				TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS);
		}

		treeview& single_expand(bool doSet) {
			return this->set_style(doSet, TVS_SINGLEEXPAND);
		}

		treeview& double_buffer(bool doSet) {
			TreeView_SetExtendedStyle(this->target().hwnd(),
				doSet ? TVS_EX_DOUBLEBUFFER : 0, TVS_EX_DOUBLEBUFFER);
			return this->target();
		}
	};

	using _item_collection = wli::treeview_item_collection<treeview>;

public:
	using item = wli::treeview_item<treeview>;

	_styler                          style{this};
	_item_collection                 items{this};
	wli::member_image_list<treeview> imageList16{this, 16};

	treeview() {
		this->imageList16.on_create([this]()->void {
			TreeView_SetImageList(this->hwnd(), this->imageList16.himagelist(), TVSIL_NORMAL);
		});
	}

	treeview& create(HWND hParent, int ctrlId, POINT pos, SIZE size) {
		return this->native_control::create(hParent, ctrlId, nullptr, pos, size, WC_TREEVIEW,
			WS_CHILD | WS_VISIBLE | WS_TABSTOP,
			WS_EX_CLIENTEDGE); // for children, WS_BORDER gives old, flat drawing; always use WS_EX_CLIENTEDGE
	}

	treeview& create(const wli::hwnd_wrapper* parent, int ctrlId, POINT pos, SIZE size) {
		return this->create(parent->hwnd(), ctrlId, pos, size);
	}
};

}//namespace wl
