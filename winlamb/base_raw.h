
#pragma once
#include "base_parent.h"

namespace wl { class window_main; } // friend forward declaration
namespace wl { class window_modal; }

namespace _wli {

// A window created programmatically.
class base_raw : public base_parent {
private:
	friend wl::window_main;
	friend wl::window_modal;

public:
	virtual ~base_raw() = 0;
	base_raw()
		: base_parent{base_parent::creation_type::raw} { }

private:
	WNDCLASSEXW _generate_wcx(wchar_t* auto_class_name_buf,
		const wchar_t* class_name, HINSTANCE hinst, UINT class_style, HICON hicon,
		HCURSOR hcursor, HBRUSH hbrush_bg) const;
	ATOM _register_class(WNDCLASSEXW& wcx) const;
	std::pair<POINT, SIZE> _calc_coords(SIZE* client_area,
		DWORD wnd_style, DWORD wnd_style_ex, HMENU hmenu) const;
	void _create_window(DWORD wnd_style_ex, const wchar_t* class_name,
		const wchar_t* wnd_title, DWORD wnd_style, POINT pos, SIZE size,
		HWND hparent, HMENU hmenu, HINSTANCE hinst) const;

	static LRESULT _wnd_proc(HWND hdlg, UINT msg, WPARAM wp, LPARAM lp);
};

}
