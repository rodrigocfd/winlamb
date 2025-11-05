#include "ctl-button.h"
#include "events-ctl-macros.h"
using namespace _wl_internal;
using namespace wl;
using namespace wl::events;

EVENT_NFY_ARG(ButtonEvents, bcn_drop_down, BCN_DROPDOWN, NMBCDROPDOWN)
EVENT_NFY_ARG(ButtonEvents, bcn_hot_item_change, BCN_HOTITEMCHANGE, NMBCHOTITEM)
EVENT_CMD(ButtonEvents, bn_clicked, BN_CLICKED)
EVENT_CMD(ButtonEvents, bn_dbl_clk, BN_DBLCLK)
EVENT_CMD(ButtonEvents, bn_kill_focus, BN_KILLFOCUS)
EVENT_CMD(ButtonEvents, bn_set_focus, BN_SETFOCUS)
void ButtonEvents::nm_custom_draw(std::function<DWORD(NMCUSTOMDRAW&)> &&cb) {
	_events._owner._userEvents.wm_notify(_events._ctrlId, NM_CUSTOMDRAW, [cb = std::move(cb)](wm::Notify p) -> LRESULT {
		return cb(p.hdr<NMCUSTOMDRAW>());
	});
}

////////////////////////////////////////////////////////////////////////////////

Button::Button(WindowParent &owner, WORD ctrlId)
	: _ctrl{owner}, _events{owner, NativeCtrl::valid_ctrl_id(ctrlId)}
{
	_ctrl._owner._preEvents.wm_create_or_init_dialog([this, pOwner = &owner]() -> void {
		_ctrl.create_wnd(ctrl_id(), _opts.windowExStyle, L"BUTTON", _opts.text,
			_opts.windowStyle | _opts.ctrlStyle, _opts.pos, _opts.size);
		_ctrl._owner._layout.add(hwnd(), _opts.layout);
	});
}

Button::Button(WindowParent &owner, WORD ctrlId, Lay layout)
	: _ctrl{owner}, _events{owner, NativeCtrl::valid_ctrl_id(ctrlId)}
{
	_ctrl._owner._preEvents.wm_create_or_init_dialog([this, layout]() -> void {
		_ctrl.assign_dlg(ctrl_id());
		_ctrl._owner._layout.add(hwnd(), layout);
	});
}

const Button& Button::set_text(WStrPtr text) const {
	_ctrl._wnd.set_text(text);
	return *this;
}

const Button& Button::trigger_click() const {
	SendMessageW(hwnd(), BM_CLICK, 0, 0);
	return *this;
}
