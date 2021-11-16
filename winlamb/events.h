
#pragma once
#include <functional>
#include <optional>
#include <variant>
#include <vector>
#include <Windows.h>

namespace _wli { class base_dlg; } // friend forward declaration
namespace _wli { class base_raw; }

namespace _wli {

class events {
private:
	friend base_dlg;
	friend base_raw;

	struct msg_elem {
		UINT msg;
		std::function<std::optional<LRESULT>(WPARAM, LPARAM)> fun;
	};
	std::vector<msg_elem> _msgs;

	struct timer_elem {
		UINT_PTR id;
		std::function<void()> fun;
	};
	std::vector<timer_elem> _timers;

	struct cmd_elem {
		WORD cmd_id;
		WORD notif_code;
		std::function<void()> fun;
	};
	std::vector<cmd_elem> _cmds;

	struct nfy_elem {
		int id_from;
		WORD notif_code;
		std::function<std::optional<LRESULT>(NMHDR*)> fun;
	};
	std::vector<nfy_elem> _nfys;

public:
	virtual ~events() { }

private:
	void _add_msg(UINT msg, std::function<std::optional<LRESULT>(WPARAM, LPARAM)> fun);
	void _add_nfy(int id_from, WORD notif_code, std::function<std::optional<LRESULT>(NMHDR*)> fun);
	void _process_all(UINT msg, WPARAM wp, LPARAM lp) const;
	std::pair<bool, std::optional<LRESULT>> _process_last(UINT msg, WPARAM wp, LPARAM lp) const;

public:
	// Adds a lambda to handle any message.
	// Unless you're handling a custom message, prefer the specific events.
	void wm(UINT msg, std::function<LRESULT(WPARAM wp, LPARAM lp)> fun);

	// Adds a lambda to handle a WM_TIMER message, for the given timer ID.
	void wm_timer(UINT_PTR id, std::function<void()> fun);

	// Adds a lambda to handle any WM_COMMAND message.
	// Menu events have notif_code=0, accelerators have notif_code=1, anything else is control-specific.
	// Unless you're handling a custom notification, prefer the specific control events.
	void wm_command(WORD cmd_id, WORD notif_code, std::function<void()> fun);
	
	// Adds a lambda to handle a WM_COMMAND message from both menu and accelerator.
	void wm_command_menu_accel(WORD cmd_id, std::function<void()> fun);

	// Adds a lambda to handle any WM_NOTIFY message.
	// Unless you're handling a custom notification, prefer the specific control events.
	void wm_notify(int id_from, WORD notif_code, std::function<LRESULT(NMHDR* hdr)> fun);

	void wm_activate(std::function<void(WORD state, bool is_minimized, HWND htarget)> fun);
	void wm_activate_app(std::function<void(bool being_activated, DWORD thread_id)> fun);
	void wm_cancel_mode(std::function<void()> fun);
	void wm_char(std::function<void(WORD char_code, DWORD flags)> fun);
	void wm_child_activate(std::function<void()> fun);
	void wm_close(std::function<void()> fun);
	void wm_create(std::function<int(CREATESTRUCT& c)> fun);
	void wm_destroy(std::function<void()> fun);
	void wm_display_change(std::function<void(DWORD bpp, SIZE screen)> fun);
	void wm_enable(std::function<void(bool being_enabled)> fun);
	void wm_enter_size_move(std::function<void()> fun);
	void wm_exit_size_move(std::function<void()> fun);
	void wm_font_change(std::function<void()> fun);
	void wm_get_icon(std::function<HICON(DWORD icon_type, DWORD dpi)> fun);
	void wm_get_min_max_info(std::function<void(MINMAXINFO&)> fun);
	void wm_init_dialog(std::function<bool(HWND hFocus)> fun);
	void wm_init_menu_popup(std::function<void(HMENU hmenu, WORD pos, bool is_window)> fun);
	void wm_key_down(std::function<void(WORD vkey_code, DWORD flags)> fun);
	void wm_menu_char(std::function<WORD(WORD char_code, WORD menu_type, HMENU hmenu)> fun);
	void wm_menu_select(std::function<void(WORD index, WORD flags, HMENU hmenu)> fun);
	void wm_move(std::function<void(POINT)> fun);
	void wm_moving(std::function<void(RECT&)> fun);
	void wm_nc_activate(std::function<bool(bool active, HRGN hrgn)> fun);
	void wm_nc_calc_size(std::function<WORD(std::variant<NCCALCSIZE_PARAMS*, RECT*> s)> fun);
	void wm_nc_destroy(std::function<void()> fun);
	void wm_nc_paint(std::function<void(HRGN hrgn)> fun);
	void wm_paint(std::function<void()> fun);
	void wm_print(std::function<void(HDC hdc, WORD opts)> fun);
	void wm_print_client(std::function<void(HDC hdc, WORD opts)> fun);
	void wm_query_open(std::function<bool()> fun);
	void wm_set_focus(std::function<void(HWND hlost)> fun);
	void wm_set_redraw(std::function<void(bool can_redraw)> fun);
	void wm_show_window(std::function<void(bool being_shown, WORD status)> fun);
	void wm_size(std::function<void(WORD request, SIZE client_area)> fun);
	void wm_sizing(std::function<void(WORD edge, RECT& drag)> fun);
	void wm_sync_paint(std::function<void()> fun);
	void wm_sys_char(std::function<void(WORD char_code, DWORD flags)> fun);
	void wm_sys_command(std::function<void(WORD cmd, POINT pos)> fun);
	void wm_time_change(std::function<void()> fun);
};

}
