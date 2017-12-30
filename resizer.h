/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <vector>
#include "hwnd_base.h"
#include "internals/params.h"

namespace wl {

// Allows the resizing of multiple controls when the parent window is resized.
class resizer final {
public:
	enum class go {
		REPOS,  // control size is fixed; control moves around anchored
		RESIZE, // control size stretches; control doesn't move
		NOTHING // control doesn't move or resize
	};

private:
	struct _ctrl final {
		HWND hChild;
		RECT rcOrig;   // original coordinates relative to parent
		go   modeHorz; // horizontal mode
		go   modeVert; // vertical mode
	};

	std::vector<_ctrl> _ctrls;
	SIZE               _szOrig;

public:
	resizer& add(HWND hCtrl, go modeHorz, go modeVert) noexcept {
		return this->_add_one(hCtrl, modeHorz, modeVert);
	}

	resizer& add(const hwnd_base& ctrl, go modeHorz, go modeVert) noexcept {
		return this->add(ctrl.hwnd(), modeHorz, modeVert);
	}

	resizer& add(std::initializer_list<HWND> hCtrls, go modeHorz, go modeVert) noexcept {
		this->_ctrls.reserve(this->_ctrls.size() + hCtrls.size());
		for (const HWND hCtrl : hCtrls) {
			this->_add_one(hCtrl, modeHorz, modeVert);
		}
		return *this;
	}

	resizer& add(std::initializer_list<hwnd_base*> ctrls, go modeHorz, go modeVert) noexcept {
		this->_ctrls.reserve(this->_ctrls.size() + ctrls.size());
		for (const hwnd_base* pCtrl : ctrls) {
			this->_add_one(pCtrl->hwnd(), modeHorz, modeVert);
		}
		return *this;
	}

	resizer& add(HWND hParent, int ctrlId, go modeHorz, go modeVert) noexcept {
		return this->add(GetDlgItem(hParent, ctrlId), modeHorz, modeVert);
	}

	resizer& add(const hwnd_base* parent, int ctrlId, go modeHorz, go modeVert) noexcept {
		return this->add(parent->hwnd(), ctrlId, modeHorz, modeVert);
	}

	resizer& add(HWND hParent, std::initializer_list<int> ctrlIds, go modeHorz, go modeVert) noexcept {
		this->_ctrls.reserve(this->_ctrls.size() + ctrlIds.size());
		for (int ctrlId : ctrlIds) {
			this->_add_one(GetDlgItem(hParent, ctrlId), modeHorz, modeVert);
		}
		return *this;
	}

	resizer& add(const hwnd_base* parent, std::initializer_list<int> ctrlIds, go modeHorz, go modeVert) noexcept {
		this->_ctrls.reserve(this->_ctrls.size() + ctrlIds.size());
		for (int ctrlId : ctrlIds) {
			this->_add_one(GetDlgItem(parent->hwnd(), ctrlId), modeHorz, modeVert);
		}
		return *this;
	}

	// Updates controls, intended to be called with parent's WM_SIZE processing.
	void arrange(const params& p) const noexcept {
		int state = static_cast<int>(p.wParam);
		int cx    = LOWORD(p.lParam);
		int cy    = HIWORD(p.lParam);

		if (this->_ctrls.empty() || state == SIZE_MINIMIZED) {
			return; // only if created() was called; if minimized, no need to resize
		}

		HDWP hdwp = BeginDeferWindowPos(static_cast<int>(this->_ctrls.size()));
		for (const _ctrl& control : this->_ctrls) {
			UINT uFlags = SWP_NOZORDER;
			if (control.modeHorz == go::REPOS && control.modeVert == go::REPOS) { // reposition both vert & horz
				uFlags |= SWP_NOSIZE;
			} else if (control.modeHorz == go::RESIZE && control.modeVert == go::RESIZE) { // resize both vert & horz
				uFlags |= SWP_NOMOVE;
			}

			DeferWindowPos(hdwp, control.hChild, nullptr,
				control.modeHorz == go::REPOS ?
				cx - this->_szOrig.cx + control.rcOrig.left :
				control.rcOrig.left, // keep original pos
				control.modeVert == go::REPOS ?
				cy - this->_szOrig.cy + control.rcOrig.top :
				control.rcOrig.top, // keep original pos
				control.modeHorz == go::RESIZE ?
				cx - this->_szOrig.cx + control.rcOrig.right - control.rcOrig.left :
				control.rcOrig.right - control.rcOrig.left, // keep original width
				control.modeVert == go::RESIZE ?
				cy - this->_szOrig.cy + control.rcOrig.bottom - control.rcOrig.top :
				control.rcOrig.bottom - control.rcOrig.top, // keep original height
				uFlags);
		}
		EndDeferWindowPos(hdwp);
	}

private:
	resizer& _add_one(HWND hChild, go modeHorz, go modeVert) noexcept {
		HWND hParent = GetParent(hChild);
		if (this->_ctrls.empty()) { // first call to _addOne()
			RECT rcP{};
			GetClientRect(hParent, &rcP);
			this->_szOrig.cx = rcP.right;
			this->_szOrig.cy = rcP.bottom; // save original size of parent
		}

		RECT rcCtrl{};
		GetWindowRect(hChild, &rcCtrl);
		this->_ctrls.push_back({hChild, rcCtrl, modeHorz, modeVert});
		ScreenToClient(hParent, reinterpret_cast<POINT*>(&this->_ctrls.back().rcOrig)); // client coordinates relative to parent
		ScreenToClient(hParent, reinterpret_cast<POINT*>(&this->_ctrls.back().rcOrig.right));
		return *this;
	}
};

}//namespace wl
