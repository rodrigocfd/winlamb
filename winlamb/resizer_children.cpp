
#include "resizer_children.h"
using namespace _wli;
using namespace wl;

void resizer_children::add(HWND hparent, HWND hctrl, horz fhorz, vert fvert)
{
	// Must be called after both the parent and the children were created,
	// because both HWNDs are used.

	if (_ctrls.empty()) {
		RECT rc_parent = {0};
		GetClientRect(hparent, &rc_parent);
		_sz_orig.cx = rc_parent.right; // save parent client area
		_sz_orig.cy = rc_parent.bottom;
		_ctrls.reserve(16); // arbitrary
	}

	RECT rc_orig = {0};
	GetWindowRect(hctrl, &rc_orig); // relative to screen
	ScreenToClient(hparent, reinterpret_cast<POINT*>(&rc_orig.left)); // now relative to parent

	_ctrls.emplace_back(hctrl, rc_orig, fhorz, fvert);
}

void resizer_children::resize(WORD request, SIZE client_area)
{
	if (_ctrls.empty() && request == SIZE_MINIMIZED) {
		return; // no need to resize
	}

	HDWP hdwp = BeginDeferWindowPos(static_cast<int>(_ctrls.size()));
	for (const ctrl_data& ctrl : _ctrls) {
		UINT uflags = SWP_NOZORDER;
		if (ctrl.fhorz == horz::repos && ctrl.fvert == vert::repos) { // reposition both vert & horz
			uflags |= SWP_NOSIZE;
		} else if (ctrl.fhorz == horz::resize && ctrl.fvert == vert::resize) { // resize both vert & horz
			uflags |= SWP_NOMOVE;
		}

		DeferWindowPos(hdwp, ctrl.hctrl, nullptr,
			ctrl.fhorz == horz::repos ?
				client_area.cx - _sz_orig.cx + ctrl.rc_orig.left :
				ctrl.rc_orig.left, // keep original pos
			ctrl.fvert == vert::repos ?
				client_area.cy - _sz_orig.cy + ctrl.rc_orig.top :
				ctrl.rc_orig.top, // keep original pos
			ctrl.fhorz == horz::resize ?
				client_area.cx - _sz_orig.cx + ctrl.rc_orig.right - ctrl.rc_orig.left :
				ctrl.rc_orig.right - ctrl.rc_orig.left, // keep original width
			ctrl.fvert == vert::resize ?
				client_area.cy - this->_sz_orig.cy + ctrl.rc_orig.bottom - ctrl.rc_orig.top :
				ctrl.rc_orig.bottom - ctrl.rc_orig.top, // keep original height
			uflags);
	}
	EndDeferWindowPos(hdwp);
}
