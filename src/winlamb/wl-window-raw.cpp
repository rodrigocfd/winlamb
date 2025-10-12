#include <system_error>
#include "window-raw.h"
using namespace _wl_internal;
using namespace wl;

ATOM RawBase::register_class(HINSTANCE hInst, LPCWSTR className, DWORD classStyle,
	WORD iconId, HBRUSH hbrBackground, HCURSOR hCursor)
{
	WNDCLASSEXW wcx{
		.cbSize = sizeof(WNDCLASSEXW),
		.style = classStyle,
		.lpfnWndProc = raw_proc,
		.hInstance = hInst,
		.hCursor = hCursor,
		.hbrBackground = hbrBackground,
	};

	if (iconId) {
		HICON hIcon = LoadIconW(hInst, MAKEINTRESOURCEW(iconId));
		#ifdef _DEBUG
		if (!hIcon)
			throw std::system_error(GetLastError(), std::system_category(), "LoadIcon failed.");
		#endif
		wcx.hIcon = hIcon;
		wcx.hIconSm = hIcon;
	}

	if (!hCursor) {
		HCURSOR hCur = LoadCursorW(nullptr, IDC_ARROW);
		#ifdef _DEBUG
		if (!hCur)
			throw std::system_error(GetLastError(), std::system_category(), "LoadCursor failed.");
		#endif
		wcx.hCursor = hCur;
	}

	std::wstring clsNameBuf;
	if (className) {
		clsNameBuf = className;
	} else {
		clsNameBuf = str::fmt(L"WNDCLASS %x.%x.%x.%x.%x.%x.%x.%x.%x",
			wcx.style, wcx.lpfnWndProc, wcx.hInstance, wcx.hCursor, wcx.hbrBackground,
			wcx.hIcon, wcx.hIconSm,
			wcx.cbClsExtra, wcx.cbWndExtra);
	}
	wcx.lpszClassName = clsNameBuf.c_str();

	ATOM atom = RegisterClassExW(&wcx);
	if (!atom) {
		DWORD err = GetLastError();
		if (err == ERROR_CLASS_ALREADY_EXISTS) {
			// https://devblogs.microsoft.com/oldnewthing/20150429-00/?p=44984
			// https://devblogs.microsoft.com/oldnewthing/20041011-00/?p=37603
			// Retrieve atom from existing window class.
			atom = GetClassInfoExW(hInst, wcx.lpszClassName, &wcx);
			#ifdef _DEBUG
			if (!atom)
				throw std::system_error(GetLastError(), std::system_category(), "GetClassInfoEx failed.");
			#endif
		}
		#ifdef _DEBUG
		throw std::system_error(GetLastError(), std::system_category(), "RegisterClassEx failed.");
		#endif
	}
	return atom;
}

void RawBase::create_window(DWORD exStyle, ATOM className, LPCWSTR title, DWORD style,
	POINT pos, SIZE sz, HWND hParent, HMENU hMenu, HINSTANCE hInst)
{
	#ifdef _DEBUG
	if (hwnd())
		throw std::logic_error("Cannot create window twice.");
	#endif

	HWND hWnd = CreateWindowExW(exStyle, MAKEINTATOM(className), title, style,
		pos.x, pos.y, sz.cx, sz.cy, hParent, hMenu, hInst, reinterpret_cast<LPVOID>(this));
	#ifdef _DEBUG
	if (!hWnd) {
		throw std::system_error(GetLastError(), std::system_category(), "CreateWindowEx failed.");
	}
	#endif
}

void RawBase::focus_first_child() const {
	if (HWND hWndFocus = GetFocus(); hWndFocus == hwnd()) {
		// https://stackoverflow.com/a/2835220/6923555
		HWND hWndFirstChild = GetWindow(hwnd(), GW_CHILD);
		if (hWndFirstChild) // the window may be chidless
			SetFocus(hWndFirstChild);
	}
}

LRESULT CALLBACK RawBase::raw_proc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	RawBase *pSelf = nullptr;
	switch (msg) {
	case WM_NCCREATE:
		pSelf = reinterpret_cast<RawBase*>(reinterpret_cast<CREATESTRUCTW*>(lp)->lpCreateParams);
		pSelf->_wndMsg._wnd._hWnd = hWnd;
		SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pSelf));
		break;
	default:
		pSelf = reinterpret_cast<RawBase*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
	}

	// If no pointer stored, then no processing is done.
	// Prevents processing before WM_NCCREATE and after WM_NCDESTROY.
	if (!pSelf) return 0;

	// Execute the event handlers.
	WindowMsg::ProcResult ret = pSelf->_wndMsg.process_msgs(msg, wp, lp);

	if (msg == WM_NCDESTROY) { // always check
		SetWindowLongPtrW(hWnd, GWLP_USERDATA, 0);
		pSelf->_wndMsg._wnd._hWnd = nullptr;
	}

	if (ret.userRet.has_value()) {
		return ret.userRet.value();
	} else {
		return (ret.hasPre || ret.hasPost) ? 0 : DefWindowProcW(hWnd, msg, wp, lp);
	}
}

////////////////////////////////////////////////////////////////////////////////

RawMain::RawMain(OptsMain opts)
	: _opts{opts}
{
	_rawBase._wndMsg._preEvents.wm(WM_ACTIVATE, [this](wm::Activate p) {
		if (!p.is_minimized()) { // https://devblogs.microsoft.com/oldnewthing/20140521-00/?p=943
			if (p.active_state() == WA_INACTIVE) {
				if (HWND hWndFocus = GetFocus(); hWndFocus && IsChild(hwnd(), hWndFocus)) {
					_hWndChildPrevFocus = hWndFocus; // save previously focused control
				}
			} else if (_hWndChildPrevFocus) {
				SetFocus(_hWndChildPrevFocus); // put focus back
			}
		}
	});

	_rawBase._wndMsg._preEvents.wm(WM_SETFOCUS, [this](wm::SetFocus) {
		_rawBase.focus_first_child();
	});

	_rawBase._wndMsg._userEvents.wm_nc_destroy([]() {
		PostQuitMessage(0);
	});
}

int RawMain::run(HINSTANCE hInst, int cmdShow) {
	ATOM atom = _rawBase.register_class(hInst, _opts.className, _opts.classStyle,
		_opts.iconId, _opts.hbrBackground, _opts.hCursor);

	RECT rcWnd{
		.left = 0,
		.top = 0,
		.right = _opts.size.cx,
		.bottom = _opts.size.cy,
	};
	AdjustWindowRectEx(&rcWnd, _opts.style, _opts.hMenu != nullptr, _opts.exStyle);
	OffsetRect(&rcWnd, -rcWnd.left, -rcWnd.top);

	POINT ptWndCenter{
		.x = GetSystemMetrics(SM_CXSCREEN) / 2 - rcWnd.right / 2,
		.y = GetSystemMetrics(SM_CYSCREEN) / 2 - rcWnd.bottom / 2,
	};

	_rawBase.create_window(_opts.exStyle, atom, _opts.title, _opts.style,
		ptWndCenter, {.cx = rcWnd.right - rcWnd.left, .cy = rcWnd.bottom - rcWnd.top},
		nullptr, _opts.hMenu, hInst);

	ShowWindow(hwnd(), cmdShow);
	UpdateWindow(hwnd());

	return _rawBase._wndMsg.main_loop(_opts.hAccelTable);
}
