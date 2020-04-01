/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <string>
#include <Windows.h>
#include <CommCtrl.h>

namespace wl {
namespace _wli {

class listview_column_collection final {
private:
	std::reference_wrapper<HWND> _hList; // the listview must outlive us

public:
	listview_column_collection(listview_column_collection&&) = default;
	listview_column_collection& operator=(listview_column_collection&&) = default; // movable only

	explicit listview_column_collection(HWND& hList) noexcept :
		_hList(hList) { }

	size_t count() const noexcept {
		return Header_GetItemCount(ListView_GetHeader(this->_hList));
	}

	listview_column_collection& add(const wchar_t* text, size_t width) noexcept {
		LVCOLUMNW lvc{};
		lvc.mask = LVCF_TEXT | LVCF_WIDTH;
		lvc.pszText = const_cast<wchar_t*>(text);
		lvc.cx = static_cast<int>(width);
		ListView_InsertColumn(this->_hList, 0xFFFF, &lvc);
		return *this;
	}

	listview_column_collection& add(const std::wstring& text, size_t width) noexcept {
		return this->add(text.c_str(), width);
	}

	listview_column_collection& set_width(size_t columnIndex, size_t width) noexcept {
		ListView_SetColumnWidth(this->_hList, columnIndex, width);
		return *this;
	}

	listview_column_collection& set_width_to_fill(size_t columnIndex) noexcept {
		size_t numCols = this->count();
		int cxUsed = 0;
		for (size_t i = 0; i < numCols; ++i) {
			if (i != columnIndex) {
				LVCOLUMNW lvc{};
				lvc.mask = LVCF_WIDTH;
				ListView_GetColumn(this->_hList, i, &lvc); // retrieve cx of each column, except stretchee
				cxUsed += lvc.cx; // sum up
			}
		}

		RECT rc{};
		GetClientRect(this->_hList, &rc); // listview client area
		ListView_SetColumnWidth(this->_hList, columnIndex,
			rc.right /*- GetSystemMetrics(SM_CXVSCROLL)*/ - cxUsed); // fit the rest of available space
		return *this;
	}

	listview_column_collection& set_text(size_t columnIndex, const wchar_t* text) noexcept {
		LVCOLUMNW lvc{};
		lvc.mask = LVCF_TEXT;
		lvc.pszText = const_cast<wchar_t*>(text);
		ListView_SetColumn(this->_hList, columnIndex, &lvc);
		return *this;
	}

	listview_column_collection& set_text(size_t columnIndex, const std::wstring& text) noexcept {
		return this->set_text(columnIndex, text.c_str());
	}

	std::wstring get_text(size_t columnIndex) const {
		wchar_t buf[64]{}; // arbitrary length
		LVCOLUMNW lvc{};
		lvc.mask = LVCF_TEXT;
		lvc.pszText = buf;
		lvc.cchTextMax = ARRAYSIZE(buf);
		ListView_GetColumn(this->_hList, columnIndex, &lvc);
		return buf;
	}
};

}//namespace _wli
}//namespace wl
