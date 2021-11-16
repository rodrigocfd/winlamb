
#pragma once
#include "base_raw.h"

namespace wl {

// A main application window created programmatically.
class window_main : public _wli::base_raw {
public:
	// Window creation options.
	// Aggregate initialization: fields are in alphabetical order.
	struct opts {
		const wchar_t* class_name = L"";
		UINT class_style = CS_DBLCLKS;
		SIZE client_area = SIZE{500, 400};
		int cmd_show = SW_SHOW;
		HACCEL haccel = nullptr;
		HBRUSH hbrush_bg = nullptr;
		HCURSOR hcursor = nullptr;
		HMENU hmenu = nullptr;
		int icon_id = 0;
		DWORD wnd_style = WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN | WS_BORDER | WS_VISIBLE | WS_MINIMIZEBOX;
		DWORD wnd_style_ex = 0;
		const wchar_t* wnd_title = L"";
	};

private:
	opts _opts;
	HWND _hchild_prev_focus = nullptr;

public:
	virtual ~window_main() = 0;
	explicit window_main(opts creation_opts);

	int run_main(HINSTANCE hinst, int cmd_show);

private:
	std::pair<HICON, HICON> _load_icons(HINSTANCE hinst) const;
};

}
