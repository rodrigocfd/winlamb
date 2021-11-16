
#include <system_error>
#include "base_raw.h"
#include "run_funcs.h"
using namespace _wli;
using std::pair;

base_raw::~base_raw()
{
	if (hwnd()) {
		SetWindowLongPtrW(hwnd(), GWLP_USERDATA, 0);
	}
}

WNDCLASSEXW base_raw::_generate_wcx(wchar_t* auto_class_name_buf,
	const wchar_t* class_name, HINSTANCE hinst, UINT class_style, HICON hicon,
	HCURSOR hcursor, HBRUSH hbrush_bg) const
{
	WNDCLASSEXW wcx = {sizeof(WNDCLASSEXW)};
	wcx.style = class_style;
	wcx.lpfnWndProc = _wnd_proc;
	wcx.hInstance = hinst;
	wcx.hIcon = hicon;
	wcx.hCursor = hcursor;
	wcx.hbrBackground = hbrush_bg;

	if (!lstrcmpW(class_name, L"")) {
		wsprintfW(auto_class_name_buf, L"%x.%Ix.%x.%x.%Ix.%Ix.%Ix.%Ix.%Ix.%Ix",
			wcx.style, reinterpret_cast<UINT64>(wcx.lpfnWndProc), wcx.cbClsExtra, wcx.cbWndExtra,
			reinterpret_cast<UINT64>(wcx.hInstance), reinterpret_cast<UINT64>(wcx.hIcon),
			reinterpret_cast<UINT64>(wcx.hCursor), reinterpret_cast<UINT64>(wcx.hbrBackground),
			reinterpret_cast<UINT64>(wcx.lpszMenuName), reinterpret_cast<UINT64>(wcx.hIconSm));
		wcx.lpszClassName = auto_class_name_buf;
	} else {
		wcx.lpszClassName = class_name;
	}

	return wcx;
}

ATOM base_raw::_register_class(WNDCLASSEXW& wcx) const
{
	ATOM atom = RegisterClassExW(&wcx);
	if (!atom) {
		DWORD err_code = GetLastError();
		if (err_code == ERROR_CLASS_ALREADY_EXISTS) {
			// https://devblogs.microsoft.com/oldnewthing/20150429-00/?p=44984
			// https://devblogs.microsoft.com/oldnewthing/20041011-00/?p=37603
			// Retrieve atom from existing window class.
			atom = static_cast<ATOM>(GetClassInfoExW(wcx.hInstance, wcx.lpszClassName, &wcx));
			if (!atom) {
				throw std::system_error(err_code, std::system_category(),
					"GetClassInfoEx failed");
			}
		} else {
			throw std::system_error(err_code, std::system_category(),
				"RegisterClassEx failed");
		}
	}
	return atom;
}

pair<POINT, SIZE> base_raw::_calc_coords(SIZE* client_area,
	DWORD wnd_style, DWORD wnd_style_ex, HMENU hmenu) const
{
	multiply_dpi(nullptr, client_area);
	RECT rc = {
		.right = client_area->cx,
		.bottom = client_area->cy,
	};
	if (!AdjustWindowRectEx(&rc, wnd_style, hmenu && GetMenuItemCount(hmenu), wnd_style_ex)) {
		throw std::system_error(GetLastError(), std::system_category(),
			"AdjustWindowRectEx failed");
	}

	*client_area = { // as if left/top coordinate is 0,0
		.cx = rc.right - rc.left,
		.cy = rc.bottom - rc.top,
	};

	SIZE sz_screen = {GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)};
	POINT pos = {
		sz_screen.cx / 2 - client_area->cx / 2, // center on screen
		sz_screen.cy / 2 - client_area->cy / 2,
	};

	return {pos, *client_area};
}

void base_raw::_create_window(DWORD wnd_style_ex, const wchar_t* class_name,
	const wchar_t* wnd_title, DWORD wnd_style, POINT pos, SIZE size,
	HWND hparent, HMENU hmenu, HINSTANCE hinst) const
{
	if (hwnd()) {
		throw std::logic_error("Window already created");
	}

	if (!CreateWindowExW(wnd_style_ex, class_name, wnd_title, wnd_style,
		pos.x, pos.y, size.cx, size.cy, hparent, hmenu, hinst,
		static_cast<LPVOID>(const_cast<base_raw*>(this))))
	{
		throw std::system_error(GetLastError(), std::system_category(),
			"CreateWindowEx failed");
	}
}

LRESULT base_raw::_wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	base_raw* _this = nullptr;

	if (msg == WM_NCCREATE) {
		const CREATESTRUCT* cs = reinterpret_cast<const CREATESTRUCT*>(lp);
		_this = reinterpret_cast<base_raw*>(cs->lpCreateParams);
		_this->set_hwnd(hwnd); // assign actual HWND
		SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(_this));
	} else {
		_this = reinterpret_cast<base_raw*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
	}

	// If object pointer is not stored, then no processing is done.
	// Prevents processing before WM_NCCREATE and after WM_NCDESTROY.
	if (_this) {
		// Process all internal events.
		_this->_events_internal._process_all(msg, wp, lp);

		// Try to process the message with an user callback.
		auto [was_handled, ret] = _this->_events._process_last(msg, wp, lp);

		// No further messages processed after this one.
		if (msg == WM_NCDESTROY) {
			SetWindowLongPtrW(hwnd, GWLP_USERDATA, 0);
			_this->set_hwnd(nullptr); // clear actual HWND
		}

		if (was_handled) {
			return ret.has_value() ? ret.value() : 0; // message processed
		}
	}

	return DefWindowProcW(hwnd, msg, wp, lp); // message not processed
}
