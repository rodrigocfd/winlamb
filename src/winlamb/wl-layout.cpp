#include "layout.h"
using namespace _wl_internal;
using namespace wl;

void Layout::rearrange(HWND hParent, wm::Size p) {
	if (_ctrls.empty() || p.is_minimized())
		return; // no need to resize if window is minimized

	if (_firstPass) {
		calc_origins(hParent); // only once
		_firstPass = false;
	}

	HDWP hdwp = BeginDeferWindowPos(static_cast<int>(_ctrls.size()));
	for (auto& ctrl : _ctrls) {
		WORD flags = SWP_NOZORDER;
		switch (ctrl.layout) {
		case Lay::repos_repos: // repos both horz and vert
			flags |= SWP_NOSIZE;
			break;
		case Lay::resize_resize: // resize both horz and vert
			flags |= SWP_NOMOVE;
		}

		DeferWindowPos(hdwp, ctrl.hCtrl, nullptr,
			(static_cast<BYTE>(ctrl.layout) & LAY_H_REPOS)
				? p.sz().cx - _szOrig.cx + ctrl.rcOrig.left
				: ctrl.rcOrig.left, // keep original horz pos
			(static_cast<BYTE>(ctrl.layout) & LAY_V_REPOS)
				? p.sz().cy - _szOrig.cy + ctrl.rcOrig.top
				: ctrl.rcOrig.top, // keep original vert pos
			(static_cast<BYTE>(ctrl.layout) & LAY_H_RESIZE)
				? p.sz().cx - _szOrig.cx + ctrl.rcOrig.right - ctrl.rcOrig.left
				: ctrl.rcOrig.right - ctrl.rcOrig.left, // keep original width
			(static_cast<BYTE>(ctrl.layout) & LAY_V_RESIZE)
				? p.sz().cy - _szOrig.cy + ctrl.rcOrig.bottom - ctrl.rcOrig.top
				: ctrl.rcOrig.bottom - ctrl.rcOrig.top, // keep original height
			flags);
	}
	EndDeferWindowPos(hdwp);
}

void Layout::calc_origins(HWND hParent) {
	RECT rcParent{};
	GetClientRect(hParent, &rcParent);
	_szOrig = {rcParent.right, rcParent.bottom}; // save original parent client area

	for (auto& ctrl : _ctrls) { // save original client area of each control, relative to parent
		RECT rcCtrlOrig{};
		GetWindowRect(ctrl.hCtrl, &ctrl.rcOrig); // relative to screen
		ScreenToClient(hParent, reinterpret_cast<POINT*>(&ctrl.rcOrig)); // now relative to parent
		ScreenToClient(hParent, reinterpret_cast<POINT*>(&ctrl.rcOrig.right)); // now relative to parent
	}
}
