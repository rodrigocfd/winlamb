/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <vector>
#include "internals/native_control.h"
#include "internals/params.h"
#include "internals/styler.h"
#include "icon.h"

/**
 * hwnd_base
 *  native_control
 *   statusbar
 */

namespace wl {

// Wrapper to status control from Common Controls library.
class statusbar final : public wli::native_control<statusbar> {
private:
	class _styler final : public wli::styler<statusbar> {
	public:
		explicit _styler(statusbar* pSb) noexcept : styler(pSb) { }

		statusbar& size_grip(bool doSet) noexcept {
			return this->set_style(doSet, SBARS_SIZEGRIP);
		}

		statusbar& tooltips(bool doSet) noexcept {
			return this->set_style(doSet, SBARS_TOOLTIPS);
		}
	};

	struct _part final {
		UINT sizePixels = 0;
		UINT resizeWeight = 0;
	};

	std::vector<_part> _parts;
	std::vector<int>   _rightEdges;

public:
	_styler style{this};

	statusbar& create(HWND hParent) {
		if (this->hwnd()) {
			throw std::logic_error("Trying to create a statusbar twice.");
		}

		DWORD parentStyle = static_cast<DWORD>(GetWindowLongPtrW(hParent, GWL_STYLE));
		bool canStretch = (parentStyle & WS_MAXIMIZEBOX) != 0 ||
			(parentStyle & WS_SIZEBOX) != 0;
		return this->native_control::create(hParent, 0, nullptr, {0,0}, {0,0}, STATUSCLASSNAME,
			(WS_CHILD | WS_VISIBLE) | (canStretch ? SBARS_SIZEGRIP : 0), 0);
	}

	statusbar& create(const hwnd_base* parent) {
		return this->create(parent->hwnd());
	}

	void adjust(const params& p) noexcept {
		// Intended to be called with parent's WM_SIZE processing.
		if (p.wParam != SIZE_MINIMIZED && this->hwnd()) {
			int cx = LOWORD(p.lParam); // available width
			SendMessageW(this->hwnd(), WM_SIZE, 0, 0); // tell statusbar to fit parent

			// Find the space to be divided among variable-width parts,
			// and total weight of variable-width parts.
			UINT totalWeight = 0;
			int  cxVariable = cx;
			for (const _part& onePart : this->_parts) {
				if (!onePart.resizeWeight) { // fixed-width?
					cxVariable -= onePart.sizePixels;
				} else {
					totalWeight += onePart.resizeWeight;
				}
			}

			// Fill right edges array with the right edge of each part.
			int cxTotal = cx;
			for (size_t i = this->_parts.size(); i-- > 0; ) {
				this->_rightEdges[i] = cxTotal;
				cxTotal -= (!this->_parts[i].resizeWeight) ? // fixed-width?
					this->_parts[i].sizePixels :
					static_cast<int>( (cxVariable / totalWeight) * this->_parts[i].resizeWeight );
			}
			SendMessageW(this->hwnd(), SB_SETPARTS, this->_rightEdges.size(),
				reinterpret_cast<LPARAM>(&this->_rightEdges[0]));
		}
	}

	statusbar& add_fixed_part(UINT sizePixels) noexcept {
		if (this->hwnd()) {
			this->_parts.push_back({sizePixels, 0});
			this->_rightEdges.emplace_back(0);
			this->adjust(params{WM_SIZE, SIZE_RESTORED, MAKELPARAM(this->_get_parent_cx(), 0)});
		}
		return *this;
	}

	statusbar& add_resizable_part(UINT resizeWeight) noexcept {
		// How resizeWeight works:
		// Suppose you have 3 parts, respectively with weights of 1, 1 and 2.
		// If available client area is 400px, respective part widths will be 100, 100 and 200px.
		// Zero weight means a fixed-width part, which internally should have sizePixels set.
		if (this->hwnd()) {
			this->_parts.push_back({0, resizeWeight});
			this->_rightEdges.emplace_back(0);
			this->adjust(params{WM_SIZE, SIZE_RESTORED, MAKELPARAM(this->_get_parent_cx(), 0)});
		}
		return *this;
	}

	statusbar& set_text(const wchar_t* text, size_t iPart) noexcept {
		SendMessageW(this->hwnd(), SB_SETTEXT, MAKEWPARAM(MAKEWORD(iPart, 0), 0),
			reinterpret_cast<LPARAM>(text));
		return *this;
	}

	statusbar& set_text(const std::wstring& text, size_t iPart) noexcept {
		return this->set_text(text.c_str(), iPart);
	}

	std::wstring get_text(size_t iPart) const noexcept {
		std::wstring buf;
		int len = LOWORD(SendMessageW(this->hwnd(), SB_GETTEXTLENGTH, iPart, 0));
		if (len) {
			buf.resize(len + 1, L'\0');
			SendMessageW(this->hwnd(), SB_GETTEXT, iPart, reinterpret_cast<LPARAM>(&buf[0]));
			buf.resize(len);
		}
		return buf;
	}

	statusbar& set_icon(HICON hIcon, size_t iPart) noexcept {
		// Pass nullptr to clear icon.
		SendMessageW(this->hwnd(), SB_SETICON, iPart, reinterpret_cast<LPARAM>(hIcon));
		return *this;
	}

	statusbar& set_icon(const icon& ico, size_t iPart) noexcept {
		return this->set_icon(ico.hicon(), iPart);
	}

private:
	int _get_parent_cx() noexcept {
		static int cx = 0; // cache, since parts are intended to be added during window creation only
		if (!cx && this->hwnd()) {
			RECT rc{};
			GetClientRect(GetParent(this->hwnd()), &rc);
			cx = rc.right;
		}
		return cx;
	}
};

}//namespace wl
