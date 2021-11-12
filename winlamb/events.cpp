
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

void events::any(UINT msg, function<LRESULT(WPARAM wp, LPARAM lp)> fun)
{
	_add_msg(msg, [fun = std::move(fun)](WPARAM wp, LPARAM lp) -> optional<LRESULT> {
		return {fun(wp, lp)};
	});
}

void events::timer(UINT_PTR id, function<void()> fun)
{
	if (_timers.empty()) _timers.reserve(4); // arbitrary
	_timers.emplace_back(id, std::move(fun));
}

void events::command(WORD cmd_id, WORD notif_code, function<void()> fun)
{
	if (_cmds.empty()) _cmds.reserve(10); // arbitrary
	_cmds.emplace_back(cmd_id, notif_code, std::move(fun));
}

void events::command_menu_accel(WORD cmd_id, function<void()> fun)
{
	struct fun_pack { function<void()> fun; };
	shared_ptr<fun_pack> pack = std::make_shared<fun_pack>(std::move(fun));

	command(cmd_id, 0, [shared_pack = pack]() {
		shared_pack.get()->fun();
	});
	command(cmd_id, 1, [shared_pack = pack]() {
		shared_pack.get()->fun();
	});
}

void events::notify(int id_from, WORD notif_code, function<LRESULT(NMHDR* hdr)> fun)
{
	_add_nfy(id_from, notif_code, [fun = std::move(fun)](NMHDR* hdr) -> optional<LRESULT> {
		return {fun(hdr)};
	});
}

void events::activate(function<void(WORD state, bool is_minimized, HWND hwnd)> fun)
{
	_add_msg(WM_ACTIVATE, [fun = std::move(fun)](WPARAM wp, LPARAM lp) -> optional<LRESULT> {
		fun(LOWORD(wp), HIWORD(wp) != 0, reinterpret_cast<HWND>(lp));
		return std::nullopt;
	});
}

void events::activate_app(function<void(bool being_activated, DWORD thread_id)> fun)
{
	_add_msg(WM_ACTIVATEAPP, [fun = std::move(fun)](WPARAM wp, LPARAM lp) -> optional<LRESULT> {
		fun(wp != 0, static_cast<DWORD>(lp));
		return std::nullopt;
	});
}

void events::char_(function<void(WORD char_code, DWORD flags)> fun)
{
	_add_msg(WM_CHAR, [fun = std::move(fun)](WPARAM wp, LPARAM lp) -> optional<LRESULT> {
		fun(static_cast<WORD>(wp), static_cast<DWORD>(lp));
		return std::nullopt;
	});
}

void events::close(function<void()> fun)
{
	_add_msg(WM_CLOSE, [fun = std::move(fun)](WPARAM, LPARAM) -> optional<LRESULT> {
		fun();
		return std::nullopt;
	});
}

void events::create(function<int(CREATESTRUCT& c)> fun)
{
	_add_msg(WM_CREATE, [fun = std::move(fun)](WPARAM, LPARAM lp) -> optional<LRESULT> {
		return {fun(*reinterpret_cast<CREATESTRUCT*>(lp))};
	});
}

void events::destroy(function<void()> fun)
{
	_add_msg(WM_DESTROY, [fun = std::move(fun)](WPARAM, LPARAM) -> optional<LRESULT> {
		fun();
		return std::nullopt;
	});
}

void events::enable(function<void(bool being_enabled)> fun)
{
	_add_msg(WM_ENABLE, [fun = std::move(fun)](WPARAM wp, LPARAM)->optional<LRESULT> {
		fun(wp != 0);
		return std::nullopt;
	});
}

void events::init_dialog(function<bool(HWND hFocus)> fun)
{
	_add_msg(WM_INITDIALOG, [fun = std::move(fun)](WPARAM wp, LPARAM) -> optional<LRESULT> {
		return {fun(reinterpret_cast<HWND>(wp)) != 0};
	});
}

void events::init_menu_popup(function<void(HMENU hmenu, WORD pos, bool is_window)> fun)
{
	_add_msg(WM_INITMENUPOPUP, [fun = std::move(fun)](WPARAM wp, LPARAM lp) -> optional<LRESULT> {
		fun(reinterpret_cast<HMENU>(wp), LOWORD(lp), HIWORD(lp) != 0);
		return std::nullopt;
	});
}

void events::key_down(function<void(WORD vkey_code, DWORD flags)> fun)
{
	_add_msg(WM_KEYDOWN, [fun = std::move(fun)](WPARAM wp, LPARAM lp) -> optional<LRESULT> {
		fun(static_cast<WORD>(wp), static_cast<DWORD>(lp));
		return std::nullopt;
	});
}

void events::nc_activate(function<bool(bool active, HRGN hrgn)> fun)
{
	_add_msg(WM_NCACTIVATE, [fun = std::move(fun)](WPARAM wp, LPARAM lp) -> optional<LRESULT> {
		return {fun(wp != 0, reinterpret_cast<HRGN>(lp)) ? TRUE : FALSE};
	});
}

void events::nc_calc_size(function<WORD(variant<NCCALCSIZE_PARAMS*, RECT*> s)> fun)
{
	_add_msg(WM_NCCALCSIZE, [fun = std::move(fun)](WPARAM wp, LPARAM lp) -> optional<LRESULT> {
		auto s = wp
			? variant<NCCALCSIZE_PARAMS*, RECT*>{reinterpret_cast<NCCALCSIZE_PARAMS*>(lp)}
			: variant<NCCALCSIZE_PARAMS*, RECT*>{reinterpret_cast<RECT*>(lp)};
		return {static_cast<WORD>(fun(s))};
	});
}

void events::nc_destroy(function<void()> fun)
{
	_add_msg(WM_NCDESTROY, [fun = std::move(fun)](WPARAM, LPARAM) -> optional<LRESULT> {
		fun();
		return std::nullopt;
	});
}

void events::query_open(function<bool()> fun)
{
	_add_msg(WM_QUERYOPEN, [fun = std::move(fun)](WPARAM, LPARAM) -> optional<LRESULT> {
		return {fun() ? TRUE : FALSE};
	});
}

void events::show_window(function<void(bool being_shown, WORD status)> fun)
{
	_add_msg(WM_SHOWWINDOW, [fun = std::move(fun)](WPARAM wp, LPARAM lp) -> optional<LRESULT> {
		fun(wp != 0, static_cast<WORD>(lp));
		return std::nullopt;
	});
}

void events::size(function<void(WORD request, SIZE client_area)> fun)
{
	_add_msg(WM_SIZE, [fun = std::move(fun)](WPARAM wp, LPARAM lp) -> optional<LRESULT> {
		fun(static_cast<WORD>(wp), SIZE{LOWORD(lp), HIWORD(lp)});
		return std::nullopt;
	});
}

void events::sizing(function<void(WORD edge, RECT& drag)> fun)
{
	_add_msg(WM_SIZING, [fun = std::move(fun)](WPARAM wp, LPARAM lp)->optional<LRESULT> {
		fun(static_cast<WORD>(wp), *reinterpret_cast<RECT*>(lp));
		return std::nullopt;
	});
}
