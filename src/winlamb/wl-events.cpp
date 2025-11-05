#include <stdexcept>
#include "events.h"
#include "window-user.h"
using namespace _wl_internal;
using namespace wl;
using namespace wl::events;

void EventsInternal::wm_create_or_init_dialog(std::function<void()> &&cb) {
	_inis.emplace_back(cb);
}

void EventsInternal::wm(UINT msg, std::function<void(wl::wm::Msg)> &&cb) {
	#ifdef _DEBUG
	if (msg == WM_CREATE || msg == WM_INITDIALOG || msg == WM_NOTIFY)
		throw std::logic_error("For WM_CREATE, WM_INITDIALOG, WM_NOTIFY, use the specific event methods.");
	#endif
	_msgs.emplace_back(msg, cb);
}

void EventsInternal::wm_notify(WORD idFrom, int code, std::function<void(wl::wm::Notify)> &&cb) {
	_nfys.emplace_back(idFrom, code, cb);
}

void EventsInternal::clear_inis() {
	std::vector<std::function<void()>>{}.swap(_inis); // https://stackoverflow.com/a/13944912/6923555
}

void EventsInternal::clear() {
	clear_inis();
	std::vector<Msg>{}.swap(_msgs);
	std::vector<Nfy>{}.swap(_nfys);
}

bool EventsInternal::process_all(wm::Msg procMsg) const {
	bool atLeastOne = false;

	switch (procMsg.wm) {
		case WM_CREATE:
		case WM_INITDIALOG:
			for (auto &&ini : _inis) {
				ini();
				atLeastOne = true;
			}
			break;

		case WM_NOTIFY: {
			NMHDR *pHdr = reinterpret_cast<NMHDR*>(procMsg.lp);
			for (auto &&nfy : _nfys) {
				if (nfy.idFrom == pHdr->idFrom && nfy.code == pHdr->code) {
					nfy.cb(wm::Notify{procMsg});
					atLeastOne = true;
				}
			}
			break;
		}

		default: { // finally, ordinary messages
			for (auto &&msg : _msgs) {
				if (msg.wm == procMsg.wm) {
					msg.cb(procMsg);
					atLeastOne = true;
				}
			}
		}
	}

	return atLeastOne;
}

////////////////////////////////////////////////////////////////////////////////

void WindowEvents::wm(UINT msg, std::function<LRESULT(wm::Msg)> &&cb) {
	#ifdef _DEBUG
	if (msg == WM_CREATE || msg == WM_INITDIALOG || msg == WM_COMMAND || msg == WM_NOTIFY)
		throw std::logic_error("For WM_CREATE, WM_INITDIALOG, WM_COMMAND or WM_NOTIFY, use the specific event methods.");
	#endif
	_msgs.emplace_back(msg, cb);
}

void WindowEvents::wm_create(std::function<int(wm::Create)> &&cb) {
	_inis.emplace_back(WM_CREATE, cb);
}

void WindowEvents::wm_init_dialog(std::function<bool(wm::InitDialog)> &&cb) {
	_inis.emplace_back(WM_INITDIALOG, cb);
}

void WindowEvents::wm_command(WORD cmdId, WORD notifCode, std::function<void()> &&cb) {
	_cmds.emplace_back(cmdId, notifCode, cb);
}

void WindowEvents::wm_command(WORD cmdId, std::function<void()> &&cb) {
	_cmds.emplace_back(cmdId, 0, cb); // menu
	_cmds.emplace_back(cmdId, 1, cb); // accelerator
}

void WindowEvents::wm_notify(WORD idFrom, int code, std::function<LRESULT(wm::Notify)> &&cb) {
	_nfys.emplace_back(idFrom, code, cb);
}

#define EVENT_NO_ARGS(name, msg) \
	void WindowEvents::name(std::function<void()> &&cb) { \
		wm(msg, [cb = std::move(cb), isDlg = _isDlg](wm::Msg) -> LRESULT { cb(); return isDlg ? TRUE : 0; }); \
	}
#define EVENT_ARGS(name, msg, ty) \
	void WindowEvents::name(std::function<void(ty)> &&cb) { \
		wm(msg, [cb = std::move(cb), isDlg = _isDlg](wm::Msg p) -> LRESULT { cb(p); return isDlg ? TRUE : 0; }); \
	}
#define EVENT_ARGS_RET(name, msg, ty, tyret) \
	void WindowEvents::name(std::function<tyret(ty)> &&cb) { \
		wm(msg, [cb = std::move(cb)](wm::Msg p) -> LRESULT { return cb(p); }); \
	}

EVENT_ARGS(wm_activate, WM_ACTIVATE, wm::Activate)
EVENT_ARGS(wm_activate_app, WM_ACTIVATEAPP, wm::ActivateApp)
EVENT_NO_ARGS(wm_child_activate, WM_CHILDACTIVATE)
EVENT_NO_ARGS(wm_close, WM_CLOSE)
EVENT_NO_ARGS(wm_destroy, WM_DESTROY)
EVENT_ARGS(wm_enable, WM_ENABLE, wm::Enable)
EVENT_ARGS(wm_end_session, WM_ENDSESSION, wm::EndSession)
EVENT_NO_ARGS(wm_enter_size_move, WM_ENTERSIZEMOVE)
EVENT_ARGS_RET(wm_erase_bkgnd, WM_ERASEBKGND, wm::EraseBkgnd, int)
EVENT_NO_ARGS(wm_exit_size_move, WM_EXITSIZEMOVE)
EVENT_ARGS_RET(wm_get_dlg_code, WM_GETDLGCODE, wm::GetDlgCode, WORD)
EVENT_ARGS(wm_h_scroll, WM_HSCROLL, wm::HScroll)
EVENT_ARGS(wm_init_menu_popup, WM_INITMENUPOPUP, wm::InitMenuPopup)
EVENT_ARGS(wm_kill_focus, WM_KILLFOCUS, wm::KillFocus)
EVENT_ARGS(wm_l_button_dbl_clk, WM_LBUTTONDBLCLK, wm::LButtonDblClk)
EVENT_ARGS(wm_l_button_down, WM_LBUTTONDOWN, wm::LButtonDown)
EVENT_ARGS(wm_l_button_up, WM_LBUTTONUP, wm::LButtonUp)
EVENT_ARGS(wm_m_button_dbl_clk, WM_MBUTTONDBLCLK, wm::MButtonDblClk)
EVENT_ARGS(wm_m_button_down, WM_MBUTTONDOWN, wm::MButtonDown)
EVENT_ARGS(wm_m_button_up, WM_MBUTTONUP, wm::MButtonUp)
EVENT_ARGS(wm_mouse_hover, WM_MOUSEHOVER, wm::MouseHover)
EVENT_ARGS(wm_mouse_move, WM_MOUSEMOVE, wm::MouseMove)
EVENT_ARGS(wm_move, WM_MOVE, wm::Move)
EVENT_ARGS(wm_moving, WM_MOVING, wm::Moving)
EVENT_ARGS_RET(wm_nc_calc_size, WM_NCCALCSIZE, wm::NcCalcSize, WORD)
EVENT_NO_ARGS(wm_nc_destroy, WM_NCDESTROY)
EVENT_ARGS(wm_nc_paint, WM_NCPAINT, wm::NcPaint)
EVENT_NO_ARGS(wm_paint, WM_PAINT)
EVENT_ARGS(wm_power_broadcast, WM_POWERBROADCAST, wm::PowerBroadcast)
EVENT_ARGS(wm_r_button_dbl_clk, WM_RBUTTONDBLCLK, wm::RButtonDblClk)
EVENT_ARGS(wm_r_button_down, WM_RBUTTONDOWN, wm::RButtonDown)
EVENT_ARGS(wm_r_button_up, WM_RBUTTONUP, wm::RButtonUp)
EVENT_ARGS_RET(wm_set_cursor, WM_SETCURSOR, wm::SetCursor, bool)
EVENT_ARGS(wm_set_focus, WM_SETFOCUS, wm::SetFocus)
EVENT_ARGS(wm_show_window, WM_SHOWWINDOW, wm::ShowWindow)
EVENT_ARGS(wm_size, WM_SIZE, wm::Size)
EVENT_ARGS(wm_sizing, WM_SIZING, wm::Sizing)
EVENT_NO_ARGS(wm_time_change, WM_TIMECHANGE)
EVENT_ARGS(wm_v_scroll, WM_VSCROLL, wm::VScroll)
EVENT_ARGS(wm_window_pos_changed, WM_WINDOWPOSCHANGED, wm::WindowPosChanged)
EVENT_ARGS(wm_window_pos_changing, WM_WINDOWPOSCHANGING, wm::WindowPosChanging)

bool WindowEvents::has_message() const {
	return !_inis.empty() || !_msgs.empty() || !_cmds.empty() || !_nfys.empty();
}

void WindowEvents::clear_inis() {
	std::vector<Msg>{}.swap(_inis); // https://stackoverflow.com/a/13944912/6923555
}

void WindowEvents::clear() {
	clear_inis();
	std::vector<Msg>{}.swap(_msgs);
	std::vector<Cmd>{}.swap(_cmds);
	std::vector<Nfy>{}.swap(_nfys);
}

std::optional<LRESULT> WindowEvents::process_last(wm::Msg procMsg) const {
	// We process the last added message because the library adds some events
	// which can be overwritten by the user.

	switch (procMsg.wm) {
		case WM_CREATE:
		case WM_INITDIALOG:
			for (auto it = _inis.rbegin(); it != _inis.rend(); ++it) {
				if (it->wm == procMsg.wm)
					return std::make_optional(it->cb(procMsg));
			}
			break;

		case WM_COMMAND: {
			wm::Command msgCmd{procMsg};
			for (auto it = _cmds.rbegin(); it != _cmds.rend(); ++it) {
				if (it->cmdId == msgCmd.control_id() && it->notifCode == msgCmd.control_notif_code()) {
					it->cb();
					return std::make_optional(_isDlg ? TRUE : 0);
				}
			}
			break;
		}

		case WM_NOTIFY: {
			NMHDR *pHdr = reinterpret_cast<NMHDR*>(procMsg.lp);
			for (auto it = _nfys.rbegin(); it != _nfys.rend(); ++it) {
				if (it->idFrom == pHdr->idFrom && it->code == pHdr->code)
					return std::make_optional(it->cb(wm::Notify{procMsg}));
			}
			break;
		}

		default: { // finally, ordinary messages
			for (auto it = _msgs.rbegin(); it != _msgs.rend(); ++it) {
				if (it->wm == procMsg.wm)
					return std::make_optional(it->cb(procMsg));
			}
		}
	}

	return std::nullopt;
}

////////////////////////////////////////////////////////////////////////////////

EventsNativeCtrl::EventsNativeCtrl(WindowParent &owner, WORD ctrlId)
	: _owner{owner.wnd_msg()}, _ctrlId{ctrlId}
{
}
