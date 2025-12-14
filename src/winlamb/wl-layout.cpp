#include <system_error>
#include "layout.h"
#include "events-wnd.h"
using namespace wl;
using namespace _wl_internal;

void Layout::add(HWND hCtrl, wl::Lay layout) {
	if (layout == Lay::hold_hold)
		return; // nothing to do, don't even bother adding the control

	HWND hParent = GetParent(hCtrl);

	if (_ctrls.empty()) { // first control being added?
		RECT rcParent{};
		BOOL ret = GetClientRect(hParent, &rcParent);
		#ifdef _DEBUG
		if (!ret)
			throw std::system_error(GetLastError(), std::system_category(), "GetClientRect failed");
		#endif
		_szOrig = {.cx = rcParent.right, .cy = rcParent.bottom}; // save original parent client area
	}

	RECT rcCtrl{};
	BOOL ret = GetWindowRect(hCtrl, &rcCtrl); // relative to screen
	#ifdef _DEBUG
	if (!ret)
		throw std::system_error(GetLastError(), std::system_category(), "GetWindowRect failed");
	#endif
	ScreenToClient(hParent, reinterpret_cast<POINT*>(&rcCtrl)); // now relative to parent
	ScreenToClient(hParent, reinterpret_cast<POINT*>(&rcCtrl.right)); // now relative to parent

	_ctrls.emplace_back(hCtrl, layout, rcCtrl);
}

void Layout::rearrange(WPARAM wp, LPARAM lp) {
	wm::Size p{wm::Msg{WM_SIZE, wp, lp}};
	if (_ctrls.empty() || p.is_minimized())
		return; // no need to resize if window is minimized

	HDWP hdwp = BeginDeferWindowPos(static_cast<int>(_ctrls.size()));
	for (auto &&ctrl : _ctrls) {
		WORD flags = SWP_NOZORDER;
		switch (ctrl.layout) {
		case Lay::move_move: // repos both horz and vert
			flags |= SWP_NOSIZE;
			break;
		case Lay::resize_resize: // resize both horz and vert
			flags |= SWP_NOMOVE;
		}

		DeferWindowPos(hdwp, ctrl.hCtrl, nullptr,
			(static_cast<BYTE>(ctrl.layout) & LAY_H_MOVE)
				? p.sz().cx - _szOrig.cx + ctrl.rcOrig.left
				: ctrl.rcOrig.left, // keep original horz pos
			(static_cast<BYTE>(ctrl.layout) & LAY_V_MOVE)
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
