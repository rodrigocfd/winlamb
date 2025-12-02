#include <system_error>
#include "wnd-raw.h"
#include "str.h"
#include "wnd-funcs.h"
using namespace wl;
using namespace _wl_internal;

ATOM RawBase::register_class(HINSTANCE hInst, std::wstring &&className, DWORD classStyle,
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
			throw std::system_error(GetLastError(), std::system_category(), "RawBase: LoadIcon failed");
		#endif
		wcx.hIcon = hIcon;
		wcx.hIconSm = hIcon;
	}

	if (!hCursor) {
		HCURSOR hCur = LoadCursorW(nullptr, IDC_ARROW);
		#ifdef _DEBUG
		if (!hCur)
			throw std::system_error(GetLastError(), std::system_category(), "RawBase: LoadCursor failed");
		#endif
		wcx.hCursor = hCur;
	}

	if (className.empty()) {
		className = str::fmt(L"WNDCLASS %x.%x.%x.%x.%x.%x.%x.%x.%x", // generate shared class name based on fields
			wcx.style, wcx.lpfnWndProc, wcx.hInstance, wcx.hCursor, wcx.hbrBackground,
			wcx.hIcon, wcx.hIconSm,
			wcx.cbClsExtra, wcx.cbWndExtra);
	}
	wcx.lpszClassName = className.c_str();

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
				throw std::system_error(GetLastError(), std::system_category(), "RawBase: GetClassInfoEx failed");
			#endif
		}
		#ifdef _DEBUG
		throw std::system_error(GetLastError(), std::system_category(), "RawBase: RegisterClassEx failed");
		#endif
	}
	return atom;
}

void RawBase::create_window(DWORD exStyle, ATOM className, std::wstring &&title, DWORD style,
	POINT pos, SIZE sz, HWND hParent, HMENU hMenu, HINSTANCE hInst)
{
	#ifdef _DEBUG
	if (_wndBase._hWnd)
		throw std::logic_error("Cannot create window twice.");
	#endif

	HWND hWnd = CreateWindowExW(exStyle, MAKEINTATOM(className), title.c_str(), style,
		pos.x, pos.y, sz.cx, sz.cy, hParent, hMenu, hInst, reinterpret_cast<LPVOID>(this));
	#ifdef _DEBUG
	if (!hWnd)
		throw std::system_error(GetLastError(), std::system_category(), "RawBase: CreateWindowEx failed");
	#endif
}

void RawBase::focus_first_child() const {
	if (HWND hWndFocus = GetFocus(); hWndFocus == _wndBase._hWnd) {
		// https://stackoverflow.com/a/2835220/6923555
		HWND hWndFirstChild = GetWindow(_wndBase._hWnd, GW_CHILD);
		if (hWndFirstChild) // the window may be chidless
			SetFocus(hWndFirstChild);
	}
}

LRESULT CALLBACK RawBase::raw_proc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	RawBase *pSelf = nullptr;
	switch (msg) {
	case WM_NCCREATE:
		pSelf = reinterpret_cast<RawBase*>(reinterpret_cast<const CREATESTRUCTW*>(lp)->lpCreateParams);
		pSelf->_wndBase._hWnd = hWnd;
		SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pSelf));
		break;
	default:
		pSelf = reinterpret_cast<RawBase*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
	}

	// If no pointer stored, then no processing is done.
	// Prevents processing before WM_NCCREATE and after WM_NCDESTROY.
	if (!pSelf) return 0;

	// Execute the event handlers.
	WndBase::ProcResult ret = pSelf->_wndBase.process_msgs(msg, wp, lp);

	if (msg == WM_NCDESTROY) { // always check
		SetWindowLongPtrW(hWnd, GWLP_USERDATA, 0);
		pSelf->_wndBase._hWnd = nullptr;
	}

	if (ret.userRet.has_value()) {
		return ret.userRet.value();
	} else {
		return (ret.hasPre || ret.hasPost) ? 0 : DefWindowProcW(hWnd, msg, wp, lp);
	}
}

////////////////////////////////////////////////////////////////////////////////

RawMain::RawMain() {
	_rawBase._wndBase._preEvents.wm(WM_ACTIVATE, [this](wm::Activate p) -> void {
		if (!p.is_minimized()) { // https://devblogs.microsoft.com/oldnewthing/20140521-00/?p=943
			if (p.active_state() == WA_INACTIVE) {
				if (HWND hWndFocus = GetFocus(); hWndFocus && IsChild(_rawBase._wndBase._hWnd, hWndFocus)) {
					_hWndChildPrevFocus = hWndFocus; // save previously focused control
				}
			} else if (_hWndChildPrevFocus) {
				SetFocus(_hWndChildPrevFocus); // put focus back
			}
		}
	});

	_rawBase._wndBase._preEvents.wm(WM_SETFOCUS, [this](wm::SetFocus) -> void {
		_rawBase.focus_first_child();
	});

	_rawBase._wndBase._userEvents.wm_nc_destroy([]() -> void {
		PostQuitMessage(0);
	});
}

int RawMain::run(HINSTANCE hInst, int cmdShow) {
	ATOM atom = _rawBase.register_class(hInst, std::move(_opts.className), _opts.classStyle,
		_opts.iconId, _opts.hbrBackground, _opts.hCursor);

	RECT rcWnd{
		.left = 0,
		.top = 0,
		.right = _opts.size.cx,
		.bottom = _opts.size.cy,
	};
	BOOL ret = AdjustWindowRectEx(&rcWnd, _opts.style, _opts.hMenu != nullptr, _opts.exStyle);
	#ifdef _DEBUG
	if (!ret)
		throw std::system_error(GetLastError(), std::system_category(), "AdjustWindowRectEx failed");
	#endif
	OffsetRect(&rcWnd, -rcWnd.left, -rcWnd.top);

	POINT ptWndCenter{
		.x = GetSystemMetrics(SM_CXSCREEN) / 2 - rcWnd.right / 2,
		.y = GetSystemMetrics(SM_CYSCREEN) / 2 - rcWnd.bottom / 2,
	};

	_rawBase.create_window(_opts.exStyle, atom, std::move(_opts.title), _opts.style,
		ptWndCenter, {.cx = rcWnd.right - rcWnd.left, .cy = rcWnd.bottom - rcWnd.top},
		nullptr, _opts.hMenu, hInst);

	ShowWindow(_rawBase._wndBase._hWnd, cmdShow);
	ret = UpdateWindow(_rawBase._wndBase._hWnd);
	#ifdef _DEBUG
	if (!ret)
		throw std::runtime_error("UpdateWindow failed.");
	#endif

	return _rawBase._wndBase.main_loop(_opts.hAccelTable, _opts.processDlgMsgs);
}

////////////////////////////////////////////////////////////////////////////////

RawModal::RawModal(const WindowParent &parent)
	: _parent{parent}
{
	_rawBase._wndBase._preEvents.wm(WM_SETFOCUS, [this](wm::SetFocus) -> void {
		_rawBase.focus_first_child();
	});

	_rawBase._wndBase._userEvents.wm_close([this]() -> void {
		EnableWindow(_parent.hwnd(), TRUE); // re-enable parent
		if (_hWndChildPrevFocusParent)
			SetFocus(_hWndChildPrevFocusParent); // could be on WM_DESTROY as well
		DestroyWindow(_rawBase._wndBase._hWnd); // then destroy modal
	});
}

void RawModal::show() {
	HINSTANCE hInst = wnd_hinst(_parent.hwnd());
	ATOM atom = _rawBase.register_class(hInst, std::move(_opts.className), _opts.classStyle,
		_opts.iconId, _opts.hbrBackground, _opts.hCursor);

	_hWndChildPrevFocusParent = GetFocus();
	EnableWindow(_parent.hwnd(), FALSE); // https://devblogs.microsoft.com/oldnewthing/20040227-00/?p=40463

	RECT rcWnd{
		.left = 0,
		.top = 0,
		.right = _opts.size.cx,
		.bottom = _opts.size.cy,
	};
	BOOL ret = AdjustWindowRectEx(&rcWnd, _opts.style, FALSE, _opts.exStyle);
	#ifdef _DEBUG
	if (!ret)
		throw std::system_error(GetLastError(), std::system_category(), "AdjustWindowRectEx failed");
	#endif
	OffsetRect(&rcWnd, -rcWnd.left, -rcWnd.top);

	RECT rcParent{};
	GetWindowRect(_parent.hwnd(), &rcParent); // relative to screen

	POINT ptWndCenter{
		.x = rcParent.left + (rcParent.right - rcParent.left) / 2 - rcWnd.right / 2, // center on parent
		.y = rcParent.top + (rcParent.bottom - rcParent.top) / 2 - rcWnd.bottom / 2,
	};

	_rawBase.create_window(_opts.exStyle, atom, std::move(_opts.title), _opts.style,
		ptWndCenter, {.cx = rcWnd.right - rcWnd.left, .cy = rcWnd.bottom - rcWnd.top},
		nullptr, nullptr, hInst);

	_rawBase._wndBase.modal_loop(_opts.processDlgMsgs);
}

////////////////////////////////////////////////////////////////////////////////

RawControl::RawControl(WindowParent &parent) {
	parent.wnd_base()._preEvents.wm_create_or_init_dialog([this, pParent = &parent]() -> void {
		HINSTANCE hInst = wnd_hinst(pParent->hwnd());
		ATOM atom = _rawBase.register_class(hInst, std::move(_opts.className), _opts.classStyle,
			0, _opts.hbrBackground, _opts.hCursor);
		_rawBase.create_window(_opts.windowExStyle, atom, {}, _opts.windowStyle,
			_opts.pos, _opts.size, pParent->hwnd(), reinterpret_cast<HMENU>(valid_ctrl_id(_opts.ctrlId)), hInst);
		pParent->wnd_base()._layout.add(_rawBase._wndBase._hWnd, _opts.layout);
	});
}
