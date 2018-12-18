/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "styler.h"

namespace wl {
namespace wli {

template<typename treeviewT>
class treeview_styler final : public wli::styler<treeviewT> {
public:
	explicit treeview_styler(treeviewT* pTree) noexcept : styler<treeviewT>(pTree) { }

	treeviewT& always_show_sel(bool doSet) noexcept {
		return this->set_style(doSet, TVS_SHOWSELALWAYS);
	}

	treeviewT& lines_and_buttons(bool doSet) noexcept {
		return this->set_style(doSet,
			TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS);
	}

	treeviewT& single_expand(bool doSet) noexcept {
		return this->set_style(doSet, TVS_SINGLEEXPAND);
	}

	treeviewT& double_buffer(bool doSet) noexcept {
		TreeView_SetExtendedStyle(this->styler<treeviewT>::target.hwnd(),
			doSet ? TVS_EX_DOUBLEBUFFER : 0, TVS_EX_DOUBLEBUFFER);
		return this->styler<treeviewT>::target;
	}
};

}//namespace wli
}//namespace wl
