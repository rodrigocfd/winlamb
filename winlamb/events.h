
#pragma once
#include <functional>
#include <optional>
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
	void any(UINT msg, std::function<LRESULT(WPARAM wp, LPARAM lp)> fun);

	// Adds a lambda to handle a WM_TIMER message, for the given timer ID.
	void timer(UINT_PTR id, std::function<void()> fun);

	// Adds a lambda to handle any WM_COMMAND message.
	// Menu events have notif_code=0, accelerators have notif_code=1, anything else is control-specific.
	// Unless you're handling a custom notification, prefer the specific control events.
	void command(WORD cmd_id, WORD notif_code, std::function<void()> fun);
	
	// Adds a lambda to handle a WM_COMMAND message from both menu and accelerator.
	void command_menu_accel(WORD cmd_id, std::function<void()> fun);

	// Adds a lambda to handle any WM_NOTIFY message.
	// Unless you're handling a custom notification, prefer the specific control events.
	void notify(int id_from, WORD notif_code, std::function<LRESULT(NMHDR* hdr)> fun);

	void activate(std::function<void(WORD state, bool is_minimized, HWND hwnd)> fun);
	void activate_app(std::function<void(bool being_activated, DWORD thread_id)> fun);
	void char_(std::function<void(WORD char_code, DWORD flags)> fun);
	void close(std::function<void()> fun);
	void create(std::function<int(CREATESTRUCT& c)> fun);
	void destroy(std::function<void()> fun);
	void enable(std::function<void(bool being_enabled)> fun);
	void init_dialog(std::function<bool(HWND hFocus)> fun);
	void init_menu_popup(std::function<void(HMENU hmenu, WORD pos, bool is_window)> fun);
	void key_down(std::function<void(WORD vkey_code, DWORD flags)> fun);
	void nc_destroy(std::function<void()> fun);
	void size(std::function<void(WORD request, SIZE client_area)> fun);
	void sizing(std::function<void(WORD edge, RECT& drag)> fun);
};

}
