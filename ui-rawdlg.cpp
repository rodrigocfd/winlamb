#include <memory>
#include <system_error>
#include "ui-rawdlg.hpp"
#include "ui-wnd.hpp"
using namespace _wl_internal;
using namespace wl;

static constexpr UINT WM_THREAD = WM_APP + 0x3fff; // last WM_APP value

void WndBase::ui_thread(std::function<void()> &&cb) const {
	auto pPack = std::make_unique<ThreadPack>(std::move(cb));
	SendMessageW(_hWnd, WM_THREAD, WM_THREAD, reinterpret_cast<LPARAM>(pPack.release())); // leak ptr
}

WndBase::ProcResult WndBase::process_msgs(UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case WM_THREAD:
		if (wp == WM_THREAD) { // additional safety check
			std::unique_ptr<ThreadPack> pPack{reinterpret_cast<ThreadPack*>(lp)}; // rebuild from ptr
			pPack->cb();
			return {.hasPre = true, .hasPost = false, .userRet = std::nullopt};
		}
		break;
	case WM_SIZE:
		_layout.rearrange(wp, lp);
	}

	bool hasPre = _preEvents.process_all({msg, wp, lp});
	std::optional<LRESULT> userRet = _userEvents.process_last({msg, wp, lp});
	bool hasPost = _postEvents.process_all({msg, wp, lp});

	switch (msg) {
	case WM_CREATE:
	case WM_INITDIALOG:
		_preEvents.clear_inis(); // since initial messages will never be called again, release
		_userEvents.clear_inis();
		_postEvents.clear_inis();
		break;
	case WM_NCDESTROY:
		_preEvents.clear();
		_userEvents.clear();
		_postEvents.clear();
	}

	return {hasPre, hasPost, userRet};
}

int WndBase::main_loop(HACCEL hAccel, bool processDlgMsgs) {
	MSG msg{};
	for (;;) {
		if (BOOL ret = GetMessageW(&msg, nullptr, 0, 0); ret == -1) [[unlikely]] {
			throw std::system_error(GetLastError(), std::system_category(), "Main loop: GetMessage failed");
		} else if (!ret) {
			// WM_QUIT was sent, gracefully terminate the program, wParam is the program exit code.
			// https://learn.microsoft.com/en-us/windows/win32/winmsg/using-messages-and-message-queues
			break;
		}

		// If a child window, will retrieve its top-level parent.
		// If a top-level, use itself.
		HWND hWndTopLevel = GetAncestor(_hWnd, GA_ROOT);
		if (!hWndTopLevel) hWndTopLevel = msg.hwnd;

		// If we have an accelerator table, try to translate the message.
		if (hAccel && TranslateAcceleratorW(_hWnd, hAccel, &msg)) continue;

		// Try to process keyboard actions for child controls.
		if (processDlgMsgs && IsDialogMessageW(_hWnd, &msg)) continue;

		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return static_cast<int>(msg.wParam); // can be used as program return value
}

void WndBase::modal_loop(bool processDlgMsgs) {
	MSG msg{};
	for (;;) {
		if (BOOL ret = GetMessageW(&msg, nullptr, 0, 0); ret == -1) [[unlikely]] {
			throw std::system_error(GetLastError(), std::system_category(), "Modal loop: GetMessage failed");
		} else if (!ret) {
			break; // our modal was destroyed
		}

		if (!_hWnd || !IsWindow(_hWnd)) break; // our modal was destroyed

		// If a child window, will retrieve its top-level parent.
		// If a top-level, use itself.
		HWND hWndTopLevel = GetAncestor(_hWnd, GA_ROOT);
		if (!hWndTopLevel) hWndTopLevel = _hWnd;

		// Try to process keyboard actions for child controls.
		if (processDlgMsgs && IsDialogMessageW(_hWnd, &msg)) {
			// Processed all keyboard actions for child controls.
			if (!_hWnd) break; // our modal was destroyed
			else continue;
		}

		TranslateMessage(&msg);
		DispatchMessageW(&msg);

		if (!_hWnd || !IsWindow(_hWnd)) break; // our modal was destroyed
	}
}

////////////////////////////////////////////////////////////////////////////////

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
		} else {
			#ifdef _DEBUG
			throw std::system_error(GetLastError(), std::system_category(), "RawBase: RegisterClassEx failed");
			#endif
		}
	}
	return atom;
}

void RawBase::create_window(DWORD exStyle, ATOM className, std::wstring &&title, DWORD style,
	POINT pos, SIZE sz, HWND hParent, HMENU hMenu, HINSTANCE hInst)
{
	#ifdef _DEBUG
	if (_wndBase._hWnd)
		throw std::logic_error{"Cannot create window twice."};
	#endif

	HWND hWnd = CreateWindowExW(exStyle, MAKEINTATOM(className),
		title.empty() ? nullptr : title.c_str(),
		style, pos.x, pos.y, sz.cx, sz.cy, hParent, hMenu, hInst, reinterpret_cast<LPVOID>(this));
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
			_wl_internal::focus(hWndFirstChild);
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

RawMain::RawMain(WindowMainOpts creationOpts)
	: _opts{std::move(creationOpts)}
{
	_rawBase._wndBase._preEvents.wm(WM_ACTIVATE, [this](wm::Activate p) -> void {
		if (!p.is_minimized()) { // https://devblogs.microsoft.com/oldnewthing/20140521-00/?p=943
			if (p.active_state() == WA_INACTIVE) {
				if (HWND hWndFocus = GetFocus(); hWndFocus && IsChild(_rawBase._wndBase._hWnd, hWndFocus)) {
					_hWndChildPrevFocus = hWndFocus; // save previously focused control
				}
			} else if (_hWndChildPrevFocus) {
				_wl_internal::focus(_hWndChildPrevFocus); // put focus back
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
	ATOM atom = _rawBase.register_class(hInst, std::move(_opts._className), _opts._classStyle,
		_opts._iconId, _opts._hbrBackground, _opts._hCursor);

	RECT rcWnd{
		.left = 0,
		.top = 0,
		.right = _opts._size.cx == -1 ? dpi::x(500) : _opts._size.cx,
		.bottom = _opts._size.cy == -1 ? dpi::y(300) : _opts._size.cy,
	};
	BOOL ok = AdjustWindowRectEx(&rcWnd, _opts._style, _opts._hMenu != nullptr, _opts._styleEx);
	#ifdef _DEBUG
	if (!ok)
		throw std::system_error(GetLastError(), std::system_category(), "AdjustWindowRectEx failed");
	#endif
	OffsetRect(&rcWnd, -rcWnd.left, -rcWnd.top);

	POINT ptWndCenter{
		.x = GetSystemMetrics(SM_CXSCREEN) / 2 - rcWnd.right / 2,
		.y = GetSystemMetrics(SM_CYSCREEN) / 2 - rcWnd.bottom / 2,
	};

	_rawBase.create_window(_opts._styleEx, atom, std::move(_opts._title), _opts._style,
		ptWndCenter, {.cx = rcWnd.right - rcWnd.left, .cy = rcWnd.bottom - rcWnd.top},
		nullptr, _opts._hMenu, hInst);

	ShowWindow(_rawBase._wndBase._hWnd, cmdShow);
	ok = UpdateWindow(_rawBase._wndBase._hWnd);
	#ifdef _DEBUG
	if (!ok)
		throw std::runtime_error{"UpdateWindow failed."};
	#endif

	return _rawBase._wndBase.main_loop(_opts._hAccelTable, _opts._processDlgMsgs);
}

////////////////////////////////////////////////////////////////////////////////

RawModal::RawModal(WindowModalOpts creationOpts)
	: _opts{std::move(creationOpts)}
{
	_rawBase._wndBase._preEvents.wm(WM_SETFOCUS, [this](wm::SetFocus) -> void {
		_rawBase.focus_first_child();
	});

	_rawBase._wndBase._userEvents.wm_close([this]() -> void {
		EnableWindow(_opts._parent.hwnd(), TRUE); // re-enable parent
		if (_hWndChildPrevFocusParent)
			_wl_internal::focus(_hWndChildPrevFocusParent); // could be on WM_DESTROY as well
		DestroyWindow(_rawBase._wndBase._hWnd); // then destroy modal
	});
}

void RawModal::show() {
	HINSTANCE hInst = wnd_hinst(_opts._parent.hwnd());
	ATOM atom = _rawBase.register_class(hInst, std::move(_opts._className), _opts._classStyle,
		_opts._iconId, _opts._hbrBackground, _opts._hCursor);

	_hWndChildPrevFocusParent = GetFocus();
	EnableWindow(_opts._parent.hwnd(), FALSE); // https://devblogs.microsoft.com/oldnewthing/20040227-00/?p=40463

	RECT rcWnd{
		.left = 0,
		.top = 0,
		.right = _opts._size.cx == -1 ? dpi::x(400) : _opts._size.cx,
		.bottom = _opts._size.cy == -1 ? dpi::y(200) : _opts._size.cy,
	};
	BOOL ok = AdjustWindowRectEx(&rcWnd, _opts._style, FALSE, _opts._styleEx);
	#ifdef _DEBUG
	if (!ok)
		throw std::system_error(GetLastError(), std::system_category(), "AdjustWindowRectEx failed");
	#endif
	OffsetRect(&rcWnd, -rcWnd.left, -rcWnd.top);

	RECT rcParent{};
	GetWindowRect(_opts._parent.hwnd(), &rcParent); // relative to screen

	POINT ptWndCenter{
		.x = rcParent.left + (rcParent.right - rcParent.left) / 2 - rcWnd.right / 2, // center on parent
		.y = rcParent.top + (rcParent.bottom - rcParent.top) / 2 - rcWnd.bottom / 2,
	};

	_rawBase.create_window(_opts._styleEx, atom, std::move(_opts._title), _opts._style,
		ptWndCenter, {.cx = rcWnd.right - rcWnd.left, .cy = rcWnd.bottom - rcWnd.top},
		nullptr, nullptr, hInst);

	_rawBase._wndBase.modal_loop(_opts._processDlgMsgs);
}

////////////////////////////////////////////////////////////////////////////////

RawControl::RawControl(WindowControlOpts creationOpts) {
	creationOpts._owner.base()._preEvents.wm_create_or_init_dialog(
		[this, opts = std::move(creationOpts)]() mutable -> void {
			if (opts._size.cx == -1) opts._size.cx = dpi::x(100); // default size
			if (opts._size.cy == -1) opts._size.cy = dpi::y(100);

			ATOM atom = _rawBase.register_class(wnd_hinst(opts._owner.hwnd()), std::move(opts._className),
				opts._classStyle, 0, opts._hbrBackground, opts._hCursor);
			_rawBase.create_window(opts._styleEx, atom, {}, opts._style,
				opts._pos, opts._size, opts._owner.hwnd(), reinterpret_cast<HMENU>(valid_ctrl_id(opts._ctrlId)),
				wnd_hinst(opts._owner.hwnd()));

			opts._owner.base()._layout.add(_rawBase._wndBase._hWnd, opts._layout);
		});
}

////////////////////////////////////////////////////////////////////////////////

void DlgBase::create_dialog_param(HINSTANCE hInst, HWND hParent) {
	#ifdef _DEBUG
	if (_wndBase._hWnd)
		throw std::logic_error{"Cannot create dialog twice."};
	#endif

	HWND hDlg = CreateDialogParamW(hInst, MAKEINTRESOURCEW(_dlgId), hParent,
		dlg_proc, reinterpret_cast<LPARAM>(this));
	#ifdef _DEBUG
	if (!hDlg)
		throw std::system_error(GetLastError(), std::system_category(), "DlgBase: CreateDialogParam failed");
	#endif
}

void DlgBase::dialog_box_param(HINSTANCE hInst, HWND hParent) {
	#ifdef _DEBUG
	if (_wndBase._hWnd)
		throw std::logic_error{"Cannot create dialog twice."};
	#endif

	DialogBoxParamW(hInst, MAKEINTRESOURCEW(_dlgId), hParent,
		dlg_proc, reinterpret_cast<LPARAM>(this)); // returns the INT_PTR from EndDialog()
}

void DlgBase::set_icon(HINSTANCE hInst, WORD iconId) const {
	if (!iconId) return;

	HANDLE hIcon16 = LoadImageW(hInst, MAKEINTRESOURCEW(iconId), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	#ifdef _DEBUG
	if (!hIcon16)
		throw std::system_error(GetLastError(), std::system_category(), "DlgBase: LoadImage 16x16 failed");
	#endif

	HANDLE hIcon32 = LoadImageW(hInst, MAKEINTRESOURCEW(iconId), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
	#ifdef _DEBUG
	if (!hIcon32)
		throw std::system_error(GetLastError(), std::system_category(), "DlgBase: LoadImage 32x32 failed");
	#endif

	SendMessageW(_wndBase._hWnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIcon16));
	SendMessageW(_wndBase._hWnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hIcon32));
}

HACCEL DlgBase::load_accel(HINSTANCE hInst, WORD accelTblId) const {
	if (!accelTblId) return nullptr;

	HACCEL hAccel = LoadAcceleratorsW(hInst, MAKEINTRESOURCEW(accelTblId));
	#ifdef _DEBUG
	if (!hAccel)
		throw std::system_error(GetLastError(), std::system_category(), "DlgBase: LoadAccelerators failed");
	#endif
	return hAccel;
}

INT_PTR CALLBACK DlgBase::dlg_proc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp) {
	DlgBase *pSelf = nullptr;
	switch (msg) {
	case WM_INITDIALOG:
		pSelf = reinterpret_cast<DlgBase*>(lp);
		pSelf->_wndBase._hWnd = hDlg;
		SetWindowLongPtrW(hDlg, DWLP_USER, reinterpret_cast<LONG_PTR>(pSelf));
		break;
	default:
		pSelf = reinterpret_cast<DlgBase*>(GetWindowLongPtrW(hDlg, DWLP_USER));
	}

	// If no pointer stored, then no processing is done.
	// Prevents processing before WM_INITDIALOG and after WM_NCDESTROY.
	if (!pSelf) return FALSE;

	// Execute the event handlers.
	WndBase::ProcResult ret = pSelf->_wndBase.process_msgs(msg, wp, lp);

	if (msg == WM_NCDESTROY) { // always check
		SetWindowLongPtrW(hDlg, DWLP_USER, 0);
		pSelf->_wndBase._hWnd = nullptr;
	}

	if (ret.userRet.has_value()) {
		switch (msg) {
		case WM_GETDLGCODE: // demand special treatment
		case WM_SETCURSOR:
			SetWindowLongPtr(hDlg, DWLP_MSGRESULT, ret.userRet.value());
			return TRUE;
		default:
			return ret.userRet.value();
		}
	}
	return (ret.hasPre || ret.hasPost) ? TRUE : FALSE;
}

////////////////////////////////////////////////////////////////////////////////

DlgMain::DlgMain(WORD dlgId, WORD iconId, WORD accelTblId)
	: _dlgBase{dlgId}, _iconId{iconId}, _accelTblId{accelTblId}
{
	_dlgBase._wndBase._userEvents.wm_close([this]() -> void {
		DestroyWindow(_dlgBase._wndBase._hWnd);
	});

	_dlgBase._wndBase._userEvents.wm_nc_destroy([]() -> void {
		PostQuitMessage(0);
	});
}

int DlgMain::run(HINSTANCE hInst, int cmdShow) {
	_dlgBase.create_dialog_param(hInst, nullptr);
	_dlgBase.set_icon(hInst, _iconId);
	HACCEL hAccel = _dlgBase.load_accel(hInst, _accelTblId);
	ShowWindow(_dlgBase._wndBase._hWnd, cmdShow);

	return _dlgBase._wndBase.main_loop(hAccel, true);
}

////////////////////////////////////////////////////////////////////////////////

DlgModal::DlgModal(const IWindowParent &parent, WORD dlgId)
	: _parent{parent}, _dlgBase{dlgId}
{
	_dlgBase._wndBase._userEvents.wm_close([this]() -> void {
		EndDialog(_dlgBase._wndBase._hWnd, 0);
	});
}

void DlgModal::show() {
	_dlgBase.dialog_box_param(wnd_hinst(_parent.hwnd()), _parent.hwnd());
}

////////////////////////////////////////////////////////////////////////////////

DlgControl::DlgControl(IWindowParent &owner, WORD dlgId, WORD ctrlId, POINT pos, Lay layout)
	: _dlgBase{dlgId}
{
	owner.base()._preEvents.wm_create_or_init_dialog(
		[this, pOwner = &owner, ctrlId, pos, layout]() -> void {
			_dlgBase.create_dialog_param(wnd_hinst(pOwner->hwnd()), pOwner->hwnd());
			SetWindowLongPtrW(_dlgBase._wndBase._hWnd, GWLP_ID, valid_ctrl_id(ctrlId)); // give the control its ID
			SetWindowPos(_dlgBase._wndBase._hWnd, nullptr, pos.x, pos.y, 0, 0, SWP_NOZORDER | SWP_NOMOVE);
			pOwner->base()._layout.add(_dlgBase._wndBase._hWnd, layout);
		});
}
