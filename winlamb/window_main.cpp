
#include <system_error>
#include "window_main.h"
#include "run_funcs.h"
using namespace _wli;
using namespace wl;
using std::pair;

window_main::~window_main() { }

window_main::window_main(opts creation_opts)
	: _opts{creation_opts}
{
	if (!_opts.hcursor) {
		_opts.hcursor = LoadCursorW(nullptr, IDC_ARROW);
	}

	on().wm_activate([this](WORD state, bool is_minimized, HWND htarget) {
		// https://devblogs.microsoft.com/oldnewthing/20140521-00/?p=943
		if (!is_minimized) {
			if (state == WA_INACTIVE) {
				if (HWND hfocus = GetFocus(); hfocus && IsChild(hwnd(), hfocus)) {
					_hchild_prev_focus = hfocus; // save previously focused control
				}
			} else if (_hchild_prev_focus) {
				// Restoring: put focus back.
				SendMessageW(hwnd(), WM_NEXTDLGCTL, TRUE, reinterpret_cast<LPARAM>(_hchild_prev_focus));
			}
		}
	});

	on().wm_set_focus([this](HWND hlost) {
		if (hwnd() == GetFocus()) {
			// If window receives focus, delegate to first child.
			if (HWND hfirst_child = GetNextDlgTabItem(hwnd(), nullptr, FALSE); hfirst_child) {
				SendMessageW(hwnd(), WM_NEXTDLGCTL, TRUE, reinterpret_cast<LPARAM>(hfirst_child));
			}
		}
	});

	on().wm_nc_destroy([]() {
		PostQuitMessage(ERROR_SUCCESS);
	});
}

int window_main::run_main(HINSTANCE hinst, int cmd_show)
{
	first_calls();
	
	auto [hicon, hicon_sm] = _load_icons(hinst);
	wchar_t auto_class_name_buf[256 + 1] = {L'\0'};
	WNDCLASSEXW wcx = _generate_wcx(auto_class_name_buf,
		_opts.class_name, hinst, _opts.class_style, hicon, _opts.hcursor, _opts.hbrush_bg);
	ATOM atom = _register_class(wcx);

	auto [pos, size] = _calc_coords(&_opts.client_area,
		_opts.wnd_style, _opts.wnd_style_ex, _opts.hmenu);
	_create_window(_opts.wnd_style_ex, wcx.lpszClassName, _opts.wnd_title,
		_opts.wnd_style, pos, size, nullptr, _opts.hmenu, hinst);

	ShowWindow(hwnd(), _opts.cmd_show);
	UpdateWindow(hwnd());

	return run_main_loop(hwnd(), _opts.haccel);
}

pair<HICON, HICON> window_main::_load_icons(HINSTANCE hinst) const
{
	HICON hicon = nullptr, hicon_sm = nullptr;
	if (_opts.icon_id) {
		hicon = reinterpret_cast<HICON>(
			LoadImageW(hinst, MAKEINTRESOURCEW(_opts.icon_id), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR));
		if (!hicon) {
			throw std::system_error(GetLastError(), std::system_category(),
				"LoadImage failed for 32x32 icon");
		}

		hicon_sm = reinterpret_cast<HICON>(
			LoadImageW(hinst, MAKEINTRESOURCEW(_opts.icon_id), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR));
		if (!hicon) {
			throw std::system_error(GetLastError(), std::system_category(),
				"LoadImage failed for 16x16 icon");
		}
	}
	return {hicon, hicon_sm};
}
