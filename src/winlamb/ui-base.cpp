#include <memory>
#include <system_error>
#include "ui-base.hpp"
using namespace _wl_internal;
using namespace wl;
using namespace wl::events;

void InternalEvents::wm_create_or_init_dialog(std::function<void()> &&cb) {
	_inis.emplace_back(cb);
}

void InternalEvents::wm(UINT msg, std::function<void(wl::wm::Msg)> &&cb) {
	#ifdef _DEBUG
	if (msg == WM_CREATE || msg == WM_INITDIALOG || msg == WM_NOTIFY)
		throw std::logic_error{"For WM_CREATE, WM_INITDIALOG, WM_NOTIFY, use the specific event methods."};
	#endif
	_msgs.emplace_back(msg, cb);
}

void InternalEvents::wm_notify(WORD idFrom, int code, std::function<void(wl::wm::Notify)> &&cb) {
	_nfys.emplace_back(idFrom, code, cb);
}

void InternalEvents::clear_inis() {
	std::vector<std::function<void()>>{}.swap(_inis); // https://stackoverflow.com/a/13944912/6923555
}

void InternalEvents::clear() {
	clear_inis();
	std::vector<Msg>{}.swap(_msgs);
	std::vector<Nfy>{}.swap(_nfys);
}

bool InternalEvents::process_all(wm::Msg procMsg) const {
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
		throw std::logic_error{"For WM_CREATE, WM_INITDIALOG, WM_COMMAND or WM_NOTIFY, use the specific event methods."};
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
#define EVENT_NO_ARGS_RET(name, msg, tyret) \
	void WindowEvents::name(std::function<tyret()> &&cb) { \
		wm(msg, [cb = std::move(cb), isDlg = _isDlg](wm::Msg) -> LRESULT { return cb(); }); \
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
EVENT_ARGS(wm_display_change, WM_DISPLAYCHANGE, wm::DisplayChange)
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
EVENT_NO_ARGS_RET(wm_query_open, WM_QUERYOPEN, bool)
EVENT_ARGS(wm_r_button_dbl_clk, WM_RBUTTONDBLCLK, wm::RButtonDblClk)
EVENT_ARGS(wm_r_button_down, WM_RBUTTONDOWN, wm::RButtonDown)
EVENT_ARGS(wm_r_button_up, WM_RBUTTONUP, wm::RButtonUp)
EVENT_ARGS_RET(wm_set_cursor, WM_SETCURSOR, wm::SetCursor, bool)
EVENT_ARGS(wm_set_focus, WM_SETFOCUS, wm::SetFocus)
EVENT_ARGS(wm_show_window, WM_SHOWWINDOW, wm::ShowWindow)
EVENT_ARGS(wm_size, WM_SIZE, wm::Size)
EVENT_ARGS(wm_sizing, WM_SIZING, wm::Sizing)
EVENT_NO_ARGS(wm_theme_changed, WM_THEMECHANGED)
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

void Layout::add(HWND hCtrl, wl::Lay lay) {
	if (lay == Lay::hold_hold)
		return; // nothing to do, don't even bother adding the control

	HWND hParent = GetParent(hCtrl);

	if (_ctrls.empty()) { // first control being added?
		RECT rcParent{};
		BOOL ok = GetClientRect(hParent, &rcParent);
		#ifdef _DEBUG
		if (!ok)
			throw std::system_error(GetLastError(), std::system_category(), "GetClientRect failed");
		#endif
		_szOrig = {.cx = rcParent.right, .cy = rcParent.bottom}; // save original parent client area
	}

	RECT rcCtrl{};
	BOOL ok = GetWindowRect(hCtrl, &rcCtrl); // relative to screen
	#ifdef _DEBUG
	if (!ok)
		throw std::system_error(GetLastError(), std::system_category(), "GetWindowRect failed");
	#endif
	screen_to_client_rc(hParent, &rcCtrl); // now relative to parent

	_ctrls.emplace_back(hCtrl, lay, rcCtrl);
}

void Layout::rearrange(WPARAM wp, LPARAM lp) {
	wm::Size p{wm::Msg{WM_SIZE, wp, lp}};
	if (_ctrls.empty() || p.is_minimized())
		return; // no need to resize if window is minimized

	HDWP hdwp = BeginDeferWindowPos(static_cast<int>(_ctrls.size()));
	for (auto &&ctrl : _ctrls) {
		WORD flags = SWP_NOZORDER;
		switch (ctrl.lay) {
		case Lay::move_move: // repos both horz and vert
			flags |= SWP_NOSIZE;
			break;
		case Lay::resize_resize: // resize both horz and vert
			flags |= SWP_NOMOVE;
		}

		DeferWindowPos(hdwp, ctrl.hCtrl, nullptr,
			(ctrl.lay == Lay::move_hold || ctrl.lay == Lay::move_move || ctrl.lay == Lay::move_resize) // horz move
				? p.sz().cx - _szOrig.cx + ctrl.rcOrig.left
				: ctrl.rcOrig.left, // keep original horz pos
			(ctrl.lay == Lay::hold_move || ctrl.lay == Lay::move_move || ctrl.lay == Lay::resize_move) // vert move
				? p.sz().cy - _szOrig.cy + ctrl.rcOrig.top
				: ctrl.rcOrig.top, // keep original vert pos
			(ctrl.lay == Lay::resize_hold || ctrl.lay == Lay::resize_move || ctrl.lay == Lay::resize_resize) // horz resize
				? p.sz().cx - _szOrig.cx + ctrl.rcOrig.right - ctrl.rcOrig.left
				: ctrl.rcOrig.right - ctrl.rcOrig.left, // keep original width
			(ctrl.lay == Lay::hold_resize || ctrl.lay == Lay::move_resize || ctrl.lay == Lay::resize_resize) // vert resize
				? p.sz().cy - _szOrig.cy + ctrl.rcOrig.bottom - ctrl.rcOrig.top
				: ctrl.rcOrig.bottom - ctrl.rcOrig.top, // keep original height
			flags);
	}
	EndDeferWindowPos(hdwp);
}

////////////////////////////////////////////////////////////////////////////////

static constexpr UINT WM_THREAD = WM_APP + 0x3fff; // last WM_APP value

void WndBase::ui_thread(std::function<void()> &&cb) const {
	auto pPack = std::make_unique<ThreadPack>(std::move(cb));
	SendMessageW(_hWnd, WM_THREAD, WM_THREAD, reinterpret_cast<LPARAM>(pPack.release())); // leak ptr
}

WndBase::ProcResult WndBase::process_msgs(UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case WM_THREAD:
		if (wp == WM_THREAD) { // additional safety check
			std::unique_ptr<ThreadPack> pPack{reinterpret_cast<ThreadPack*>(lp)}; // rebuild from ptr
			pPack->cb();
			return {.hasPre = true, .hasPost = false, .userRet = std::nullopt};
		}
		break;
	case WM_SIZE:
		_layout.rearrange(wp, lp);
	}

	bool hasPre = _preEvents.process_all({msg, wp, lp});
	std::optional<LRESULT> userRet = _userEvents.process_last({msg, wp, lp});
	bool hasPost = _postEvents.process_all({msg, wp, lp});

	switch (msg) {
	case WM_CREATE:
	case WM_INITDIALOG:
		_preEvents.clear_inis(); // since initial messages will never be called again, release
		_userEvents.clear_inis();
		_postEvents.clear_inis();
		break;
	case WM_NCDESTROY:
		_preEvents.clear();
		_userEvents.clear();
		_postEvents.clear();
	}

	return {hasPre, hasPost, userRet};
}

int WndBase::main_loop(HACCEL hAccel, bool processDlgMsgs) {
	MSG msg{};
	for (;;) {
		if (BOOL ret = GetMessageW(&msg, nullptr, 0, 0); ret == -1) [[unlikely]] {
			throw std::system_error(GetLastError(), std::system_category(), "Main loop: GetMessage failed");
		} else if (!ret) {
			// WM_QUIT was sent, gracefully terminate the program, wParam is the program exit code.
			// https://learn.microsoft.com/en-us/windows/win32/winmsg/using-messages-and-message-queues
			break;
		}

		// If a child window, will retrieve its top-level parent.
		// If a top-level, use itself.
		HWND hWndTopLevel = GetAncestor(_hWnd, GA_ROOT);
		if (!hWndTopLevel) hWndTopLevel = msg.hwnd;

		// If we have an accelerator table, try to translate the message.
		if (hAccel && TranslateAcceleratorW(_hWnd, hAccel, &msg)) continue;

		// Try to process keyboard actions for child controls.
		if (processDlgMsgs && IsDialogMessageW(_hWnd, &msg)) continue;

		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return static_cast<int>(msg.wParam); // can be used as program return value
}

void WndBase::modal_loop(bool processDlgMsgs) {
	MSG msg{};
	for (;;) {
		if (BOOL ret = GetMessageW(&msg, nullptr, 0, 0); ret == -1) [[unlikely]] {
			throw std::system_error(GetLastError(), std::system_category(), "Modal loop: GetMessage failed");
		} else if (!ret) {
			break; // our modal was destroyed
		}

		if (!_hWnd || !IsWindow(_hWnd)) break; // our modal was destroyed

		// If a child window, will retrieve its top-level parent.
		// If a top-level, use itself.
		HWND hWndTopLevel = GetAncestor(_hWnd, GA_ROOT);
		if (!hWndTopLevel) hWndTopLevel = _hWnd;

		// Try to process keyboard actions for child controls.
		if (processDlgMsgs && IsDialogMessageW(_hWnd, &msg)) {
			// Processed all keyboard actions for child controls.
			if (!_hWnd) break; // our modal was destroyed
			else continue;
		}

		TranslateMessage(&msg);
		DispatchMessageW(&msg);

		if (!_hWnd || !IsWindow(_hWnd)) break; // our modal was destroyed
	}
}

////////////////////////////////////////////////////////////////////////////////

void NativeCtrlBase::create_wnd(WORD ctrlId, DWORD exStyle, const wchar_t *className,
	std::wstring &&title, DWORD style, POINT pos, SIZE size)
{
	#ifdef _DEBUG
	if (_hWnd)
		throw std::logic_error{"Cannot create control twice."};
	if (!_parent._hWnd)
		throw std::logic_error{"Cannot create control before parent."};
	#endif

	_hWnd = CreateWindowExW(exStyle, className, title.c_str(), style,
		pos.x, pos.y, size.cx, size.cy, _parent._hWnd,
		reinterpret_cast<HMENU>(ctrlId), wnd_hinst(_parent._hWnd), nullptr);
	#ifdef _DEBUG
	if (!_hWnd)
		throw std::system_error(GetLastError(), std::system_category(), "NativeCtrlBase: CreateWindowEx failed");
	#endif

	install_subclass();
}

void NativeCtrlBase::assign_dlg(WORD ctrlId) {
	#ifdef _DEBUG
	if (_hWnd)
		throw std::logic_error{"Cannot assign control twice."};
	if (!_parent._hWnd)
		throw std::logic_error{"Cannot assign control before parent."};
	#endif

	_hWnd = GetDlgItem(_parent._hWnd, ctrlId);
	#ifdef _DEBUG
	if (!_hWnd)
		throw std::system_error(GetLastError(), std::system_category(), "NativeCtrlBase: GetDlgItem failed");
	#endif

	install_subclass();
}

void NativeCtrlBase::install_subclass() {
	static UINT_PTR subclassId = 0;
	if (_subclassEvents.has_message()) {
		BOOL ok = SetWindowSubclass(_hWnd, subclass_proc, ++subclassId, reinterpret_cast<DWORD_PTR>(this));
		#ifdef _DEBUG
		if (!ok)
			throw std::runtime_error{"SetWindowSubclass failed."};
		#endif
	}
}

LRESULT CALLBACK NativeCtrlBase::subclass_proc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp,
	UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	NativeCtrlBase *pSelf = reinterpret_cast<NativeCtrlBase*>(dwRefData);

	std::optional<LRESULT> ret{};
	if (pSelf)
		ret = pSelf->_subclassEvents.process_last({msg, wp, lp});

	if (msg == WM_NCDESTROY) { // always check
		// https://devblogs.microsoft.com/oldnewthing/20031111-00/?p=41883
		BOOL ok = RemoveWindowSubclass(hWnd, subclass_proc, uIdSubclass);
		#ifdef _DEBUG
		if (!ok)
			throw std::runtime_error{"RemoveWindowSubclass failed."};
		#endif
		if (pSelf)
			pSelf->_subclassEvents.clear();
	}

	return ret.has_value() ? ret.value() : DefSubclassProc(hWnd, msg, wp, lp);
}
