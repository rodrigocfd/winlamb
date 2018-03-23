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

template<typename listviewT>
class listview_styler final : public wli::styler<listviewT> {
public:
	explicit listview_styler(listviewT* pList) noexcept : styler(pList) { }

	listviewT& always_show_sel(bool doSet) noexcept {
		return this->set_style(doSet, LVS_SHOWSELALWAYS);
	}

	listviewT& edit_labels(bool doSet) noexcept {
		return this->set_style(doSet, LVS_EDITLABELS);
	}

	listviewT& multiple_sel(bool doSet) noexcept {
		return this->set_style(!doSet, LVS_SINGLESEL);
	}

	listviewT& show_headers(bool doSet) noexcept {
		return this->set_style(doSet, LVS_NOCOLUMNHEADER);
	}

	listviewT& checkboxes(bool doSet) noexcept {
		ListView_SetExtendedListViewStyleEx(this->hwnd(), LVS_EX_CHECKBOXES,
			doSet ? LVS_EX_CHECKBOXES : 0);
		return this->target();
	}

	listviewT& double_buffer(bool doSet) noexcept {
		ListView_SetExtendedListViewStyleEx(this->hwnd(), LVS_EX_DOUBLEBUFFER,
			doSet ? LVS_EX_DOUBLEBUFFER : 0);
		return this->target();
	}

	listviewT& full_row_select(bool doSet) noexcept {
		ListView_SetExtendedListViewStyleEx(this->hwnd(), LVS_EX_FULLROWSELECT,
			doSet ? LVS_EX_FULLROWSELECT : 0);
		return this->target();
	}

	listviewT& grid_lines(bool doSet) noexcept {
		ListView_SetExtendedListViewStyleEx(this->hwnd(), LVS_EX_GRIDLINES,
			doSet ? LVS_EX_GRIDLINES : 0);
		return this->target();
	}

	listviewT& reorder_header(bool doSet) noexcept {
		ListView_SetExtendedListViewStyleEx(this->hwnd(), LVS_EX_HEADERDRAGDROP,
			doSet ? LVS_EX_HEADERDRAGDROP : 0);
		return this->target();
	}
};

}//namespace wli
}//namespace wl
