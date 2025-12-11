#include <system_error>
#include "wnd-dlg.h"
#include "wnd-app.h"
using namespace wl;
using namespace _wl_internal;

void DlgBase::create_dialog_param(HINSTANCE hInst, HWND hParent) {
	#ifdef _DEBUG
	if (_wndBase._hWnd)
		throw std::logic_error("Cannot create dialog twice.");
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
		throw std::logic_error("Cannot create dialog twice.");
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

DlgModal::DlgModal(const WindowParent &parent, WORD dlgId)
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

DlgControl::DlgControl(WindowParent &parent, WORD dlgId, WORD ctrlId, POINT pos, Lay layout)
	: _dlgBase{dlgId}
{
	parent.wnd_base()._preEvents.wm_create_or_init_dialog([this, pParent = &parent, ctrlId, pos, layout]() -> void {
		_dlgBase.create_dialog_param(wnd_hinst(pParent->hwnd()), pParent->hwnd());
		SetWindowLongPtrW(_dlgBase._wndBase._hWnd, GWLP_ID, valid_ctrl_id(ctrlId)); // give the control its ID
		SetWindowPos(_dlgBase._wndBase._hWnd, nullptr, pos.x, pos.y, 0, 0, SWP_NOZORDER | SWP_NOMOVE);
		pParent->wnd_base()._layout.add(_dlgBase._wndBase._hWnd, layout);
	});
}
