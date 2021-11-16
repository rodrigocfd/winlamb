
#include <memory>
#include "events.h"
#include "run_funcs.h"
using namespace _wli;
using std::function;
using std::optional;
using std::pair;
using std::shared_ptr;
using std::variant;
using std::vector;

void events::_add_msg(UINT msg, function<optional<LRESULT>(WPARAM, LPARAM)> fun)
{
	if (_msgs.empty()) _msgs.reserve(10); // arbitrary
	_msgs.emplace_back(msg, std::move(fun));
}

void events::_add_nfy(int id_from, WORD notif_code, function<optional<LRESULT>(NMHDR*)> fun)
{
	if (_nfys.empty()) _nfys.reserve(10); // arbitrary
	_nfys.emplace_back(id_from, notif_code, std::move(fun));
}

void events::_process_all(UINT msg, WPARAM wp, LPARAM lp) const
{
	// Exceptions are not caught because this method is supposed to be called
	// for internal callbacks, which are added only by the library itself.
	if (msg == WM_NOTIFY) {
		NMHDR* hdr = reinterpret_cast<NMHDR*>(lp);
		for (const nfy_elem& elem : _nfys) {
			if (elem.id_from == hdr->idFrom && elem.notif_code == hdr->code) {
				elem.fun(hdr);
			}
		}
	} else if (msg == WM_COMMAND) {
		for (const cmd_elem& elem : _cmds) {
			if (elem.cmd_id == LOWORD(wp) && elem.notif_code == HIWORD(wp)) {
				elem.fun();
			}
		}
	} else if (msg == WM_TIMER) {
		UINT_PTR timer_id = wp;
		for (const timer_elem& elem : _timers) {
			if (elem.id == timer_id) {
				elem.fun();
			}
		}
	} else { // any other message
		for (const msg_elem& elem : _msgs) {
			if (elem.msg == msg) {
				elem.fun(wp, lp);
			}
		}
	}
}

pair<bool, optional<LRESULT>> events::_process_last(UINT msg, WPARAM wp, LPARAM lp) const
{
	// Exceptions are caught because this method is supposed to be called
	// for user callbacks.
	try {
		if (msg == WM_NOTIFY) {
			NMHDR* hdr = reinterpret_cast<NMHDR*>(lp);
			for (auto it = _nfys.crbegin(); it != _nfys.crend(); ++it) {
				if (it->id_from == hdr->idFrom && it->notif_code == hdr->code) {
					optional<LRESULT> res = it->fun(hdr);
					return {true, res}; // processed
				}
			}
		} else if (msg == WM_COMMAND) {
			for (auto it = _cmds.crbegin(); it != _cmds.crend(); ++it) {
				if (it->cmd_id == LOWORD(wp) && it->notif_code == HIWORD(wp)) {
					it->fun();
					return {true, std::nullopt}; // processed; return value is not meaningful
				}
			}
		} else if (msg == WM_TIMER) {
			for (auto it = _timers.crbegin(); it != _timers.crend(); ++it) {
				UINT_PTR timer_id = wp;
				if (it->id == timer_id) {
					it->fun();
					return {true, std::nullopt}; // processed; return value is not meaningful
				}
			}
		} else { // any other message
			for (auto it = _msgs.crbegin(); it != _msgs.crend(); ++it) {
				if (it->msg == msg) {
					optional<LRESULT> res = it->fun(wp, lp);
					return {true, res}; // processed
				}
			}
		}
	} catch (...) {
		lippincott();
		PostQuitMessage(-1);
	}
	return {false, std::nullopt}; // not processed
}

void events::wm(UINT msg, function<LRESULT(WPARAM wp, LPARAM lp)> fun)
{
	_add_msg(msg, [fun = std::move(fun)](WPARAM wp, LPARAM lp) -> optional<LRESULT> {
		return {fun(wp, lp)};
	});
}

void events::wm_timer(UINT_PTR id, function<void()> fun)
{
	if (_timers.empty()) _timers.reserve(4); // arbitrary
	_timers.emplace_back(id, std::move(fun));
}

void events::wm_command(WORD cmd_id, WORD notif_code, function<void()> fun)
{
	if (_cmds.empty()) _cmds.reserve(10); // arbitrary
	_cmds.emplace_back(cmd_id, notif_code, std::move(fun));
}

void events::wm_command_menu_accel(WORD cmd_id, function<void()> fun)
{
	struct fun_pack { function<void()> fun; };
	shared_ptr<fun_pack> pack = std::make_shared<fun_pack>(std::move(fun));

	wm_command(cmd_id, 0, [shared_pack = pack]() {
		shared_pack.get()->fun();
	});
	wm_command(cmd_id, 1, [shared_pack = pack]() {
		shared_pack.get()->fun();
	});
}

void events::wm_notify(int id_from, WORD notif_code, function<LRESULT(NMHDR* hdr)> fun)
{
	_add_nfy(id_from, notif_code, [fun = std::move(fun)](NMHDR* hdr) -> optional<LRESULT> {
		return {fun(hdr)};
	});
}

void events::wm_activate(function<void(WORD state, bool is_minimized, HWND htarget)> fun)
{
	_add_msg(WM_ACTIVATE, [fun = std::move(fun)](WPARAM wp, LPARAM lp) -> optional<LRESULT> {
		fun(LOWORD(wp), HIWORD(wp) != 0, reinterpret_cast<HWND>(lp));
		return std::nullopt;
	});
}

void events::wm_activate_app(function<void(bool being_activated, DWORD thread_id)> fun)
{
	_add_msg(WM_ACTIVATEAPP, [fun = std::move(fun)](WPARAM wp, LPARAM lp) -> optional<LRESULT> {
		fun(wp != 0, static_cast<DWORD>(lp));
		return std::nullopt;
	});
}

void events::wm_cancel_mode(function<void()> fun)
{
	_add_msg(WM_CANCELMODE, [fun = std::move(fun)](WPARAM, LPARAM) -> optional<LRESULT> {
		fun();
		return std::nullopt;
	});
}

void events::wm_char(function<void(WORD char_code, DWORD flags)> fun)
{
	_add_msg(WM_CHAR, [fun = std::move(fun)](WPARAM wp, LPARAM lp) -> optional<LRESULT> {
		fun(static_cast<WORD>(wp), static_cast<DWORD>(lp));
		return std::nullopt;
	});
}

void events::wm_child_activate(function<void()> fun)
{
	_add_msg(WM_CHILDACTIVATE, [fun = std::move(fun)](WPARAM, LPARAM) -> optional<LRESULT> {
		fun();
		return std::nullopt;
	});
}

void events::wm_close(function<void()> fun)
{
	_add_msg(WM_CLOSE, [fun = std::move(fun)](WPARAM, LPARAM) -> optional<LRESULT> {
		fun();
		return std::nullopt;
	});
}

void events::wm_create(function<int(CREATESTRUCT& c)> fun)
{
	_add_msg(WM_CREATE, [fun = std::move(fun)](WPARAM, LPARAM lp) -> optional<LRESULT> {
		return {fun(*reinterpret_cast<CREATESTRUCT*>(lp))};
	});
}

void events::wm_destroy(function<void()> fun)
{
	_add_msg(WM_DESTROY, [fun = std::move(fun)](WPARAM, LPARAM) -> optional<LRESULT> {
		fun();
		return std::nullopt;
	});
}

void events::wm_display_change(function<void(DWORD bpp, SIZE screen)> fun)
{
	_add_msg(WM_DISPLAYCHANGE, [fun = std::move(fun)](WPARAM wp, LPARAM lp) -> optional<LRESULT> {
		fun(static_cast<DWORD>(wp), SIZE{LOWORD(lp), HIWORD(lp)});
		return std::nullopt;
	});
}

void events::wm_enable(function<void(bool being_enabled)> fun)
{
	_add_msg(WM_ENABLE, [fun = std::move(fun)](WPARAM wp, LPARAM) -> optional<LRESULT> {
		fun(wp != 0);
		return std::nullopt;
	});
}

void events::wm_enter_size_move(function<void()> fun)
{
	_add_msg(WM_ENTERSIZEMOVE, [fun = std::move(fun)](WPARAM, LPARAM) -> optional<LRESULT> {
		fun();
		return std::nullopt;
	});
}

void events::wm_exit_size_move(function<void()> fun)
{
	_add_msg(WM_EXITSIZEMOVE, [fun = std::move(fun)](WPARAM, LPARAM) -> optional<LRESULT> {
		fun();
		return std::nullopt;
	});
}

void events::wm_font_change(function<void()> fun)
{
	_add_msg(WM_FONTCHANGE, [fun = std::move(fun)](WPARAM, LPARAM) -> optional<LRESULT> {
		fun();
		return std::nullopt;
	});
}

void events::wm_get_icon(function<HICON(DWORD icon_type, DWORD dpi)> fun)
{
	_add_msg(WM_GETICON, [fun = std::move(fun)](WPARAM wp, LPARAM lp) -> optional<LRESULT> {
		return {reinterpret_cast<LRESULT>(fun(static_cast<DWORD>(wp), static_cast<DWORD>(lp)))};
	});
}

void events::wm_get_min_max_info(function<void(MINMAXINFO&)> fun)
{
	_add_msg(WM_GETMINMAXINFO, [fun = std::move(fun)](WPARAM, LPARAM lp) -> optional<LRESULT> {
		fun(*reinterpret_cast<MINMAXINFO*>(lp));
		return std::nullopt;
	});
}

void events::wm_init_dialog(function<bool(HWND hFocus)> fun)
{
	_add_msg(WM_INITDIALOG, [fun = std::move(fun)](WPARAM wp, LPARAM) -> optional<LRESULT> {
		return {fun(reinterpret_cast<HWND>(wp)) != 0};
	});
}

void events::wm_init_menu_popup(function<void(HMENU hmenu, WORD pos, bool is_window)> fun)
{
	_add_msg(WM_INITMENUPOPUP, [fun = std::move(fun)](WPARAM wp, LPARAM lp) -> optional<LRESULT> {
		fun(reinterpret_cast<HMENU>(wp), LOWORD(lp), HIWORD(lp) != 0);
		return std::nullopt;
	});
}

void events::wm_key_down(function<void(WORD vkey_code, DWORD flags)> fun)
{
	_add_msg(WM_KEYDOWN, [fun = std::move(fun)](WPARAM wp, LPARAM lp) -> optional<LRESULT> {
		fun(static_cast<WORD>(wp), static_cast<DWORD>(lp));
		return std::nullopt;
	});
}

void events::wm_menu_char(function<WORD(WORD char_code, WORD menu_type, HMENU hmenu)> fun)
{
	_add_msg(WM_MENUCHAR, [fun = std::move(fun)](WPARAM wp, LPARAM lp) -> optional<LRESULT> {
		return {fun(LOWORD(wp), HIWORD(lp), reinterpret_cast<HMENU>(lp))};
	});
}

void events::wm_menu_select(function<void(WORD index, WORD flags, HMENU hmenu)> fun)
{
	_add_msg(WM_MENUSELECT, [fun = std::move(fun)](WPARAM wp, LPARAM lp) -> optional<LRESULT> {
		fun(LOWORD(wp), HIWORD(lp), reinterpret_cast<HMENU>(lp));
		return std::nullopt;
	});
}

void events::wm_move(function<void(POINT)> fun)
{
	_add_msg(WM_MOVE, [fun = std::move(fun)](WPARAM, LPARAM lp) -> optional<LRESULT> {
		fun(POINT{LOWORD(lp), HIWORD(lp)});
		return std::nullopt;
	});
}

void events::wm_moving(function<void(RECT&)> fun)
{
	_add_msg(WM_MOVING, [fun = std::move(fun)](WPARAM, LPARAM lp) -> optional<LRESULT> {
		fun(*reinterpret_cast<RECT*>(lp));
		return {TRUE};
	});
}

void events::wm_nc_activate(function<bool(bool active, HRGN hrgn)> fun)
{
	_add_msg(WM_NCACTIVATE, [fun = std::move(fun)](WPARAM wp, LPARAM lp) -> optional<LRESULT> {
		return {fun(wp != 0, reinterpret_cast<HRGN>(lp)) ? TRUE : FALSE};
	});
}

void events::wm_nc_calc_size(function<WORD(variant<NCCALCSIZE_PARAMS*, RECT*> s)> fun)
{
	_add_msg(WM_NCCALCSIZE, [fun = std::move(fun)](WPARAM wp, LPARAM lp) -> optional<LRESULT> {
		auto s = wp
			? variant<NCCALCSIZE_PARAMS*, RECT*>{reinterpret_cast<NCCALCSIZE_PARAMS*>(lp)}
			: variant<NCCALCSIZE_PARAMS*, RECT*>{reinterpret_cast<RECT*>(lp)};
		return {static_cast<WORD>(fun(s))};
	});
}

void events::wm_nc_destroy(function<void()> fun)
{
	_add_msg(WM_NCDESTROY, [fun = std::move(fun)](WPARAM, LPARAM) -> optional<LRESULT> {
		fun();
		return std::nullopt;
	});
}

void events::wm_nc_paint(function<void(HRGN hrgn)> fun)
{
	_add_msg(WM_NCPAINT, [fun = std::move(fun)](WPARAM wp, LPARAM) -> optional<LRESULT> {
		fun(reinterpret_cast<HRGN>(wp));
		return std::nullopt;
	});
}

void events::wm_paint(function<void()> fun)
{
	_add_msg(WM_PAINT, [fun = std::move(fun)](WPARAM, LPARAM) -> optional<LRESULT> {
		fun();
		return std::nullopt;
	});
}

void events::wm_print(function<void(HDC hdc, WORD opts)> fun)
{
	_add_msg(WM_PRINT, [fun = std::move(fun)](WPARAM wp, LPARAM lp) -> optional<LRESULT> {
		fun(reinterpret_cast<HDC>(wp), static_cast<WORD>(lp));
		return std::nullopt;
	});
}

void events::wm_print_client(function<void(HDC hdc, WORD opts)> fun)
{
	_add_msg(WM_PRINTCLIENT, [fun = std::move(fun)](WPARAM wp, LPARAM lp) -> optional<LRESULT> {
		fun(reinterpret_cast<HDC>(wp), static_cast<WORD>(lp));
		return std::nullopt;
	});
}

void events::wm_query_open(function<bool()> fun)
{
	_add_msg(WM_QUERYOPEN, [fun = std::move(fun)](WPARAM, LPARAM) -> optional<LRESULT> {
		return {fun() ? TRUE : FALSE};
	});
}

void events::wm_set_focus(function<void(HWND hlost)> fun)
{
	_add_msg(WM_SETFOCUS, [fun = std::move(fun)](WPARAM wp, LPARAM) -> optional<LRESULT> {
		fun(reinterpret_cast<HWND>(wp));
		return std::nullopt;
	});
}

void events::wm_set_redraw(function<void(bool can_redraw)> fun)
{
	_add_msg(WM_SETREDRAW, [fun = std::move(fun)](WPARAM wp, LPARAM) -> optional<LRESULT> {
		fun(wp != 0);
		return std::nullopt;
	});
}

void events::wm_show_window(function<void(bool being_shown, WORD status)> fun)
{
	_add_msg(WM_SHOWWINDOW, [fun = std::move(fun)](WPARAM wp, LPARAM lp) -> optional<LRESULT> {
		fun(wp != 0, static_cast<WORD>(lp));
		return std::nullopt;
	});
}

void events::wm_size(function<void(WORD request, SIZE client_area)> fun)
{
	_add_msg(WM_SIZE, [fun = std::move(fun)](WPARAM wp, LPARAM lp) -> optional<LRESULT> {
		fun(static_cast<WORD>(wp), SIZE{LOWORD(lp), HIWORD(lp)});
		return std::nullopt;
	});
}

void events::wm_sizing(function<void(WORD edge, RECT& drag)> fun)
{
	_add_msg(WM_SIZING, [fun = std::move(fun)](WPARAM wp, LPARAM lp) -> optional<LRESULT> {
		fun(static_cast<WORD>(wp), *reinterpret_cast<RECT*>(lp));
		return {TRUE};
	});
}

void events::wm_sync_paint(function<void()> fun)
{
	_add_msg(WM_SYNCPAINT, [fun = std::move(fun)](WPARAM, LPARAM) -> optional<LRESULT> {
		fun();
		return std::nullopt;
	});
}

void events::wm_sys_char(function<void(WORD char_code, DWORD flags)> fun)
{
	_add_msg(WM_SYSCHAR, [fun = std::move(fun)](WPARAM wp, LPARAM lp) -> optional<LRESULT> {
		fun(static_cast<WORD>(wp), static_cast<DWORD>(lp));
		return std::nullopt;
	});
}

void events::wm_sys_command(function<void(WORD cmd, POINT pos)> fun)
{
	_add_msg(WM_SYSCOMMAND, [fun = std::move(fun)](WPARAM wp, LPARAM lp) -> optional<LRESULT> {
		fun(static_cast<WORD>(wp), POINT{LOWORD(lp), HIWORD(lp)});
		return std::nullopt;
	});
}

void events::wm_time_change(function<void()> fun)
{
	_add_msg(WM_TIMECHANGE, [fun = std::move(fun)](WPARAM, LPARAM) -> optional<LRESULT> {
		fun();
		return std::nullopt;
	});
}
