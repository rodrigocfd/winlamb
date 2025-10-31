#include <system_error>
#include "window-dlg.h"
#include "window-user.h"
using namespace _wl_internal;
using namespace wl;

void DlgBase::create_dialog_param(HINSTANCE hInst, HWND hParent) {
	#ifdef _DEBUG
	if (hwnd())
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
	if (hwnd())
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

	SendMessageW(hwnd(), WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIcon16));
	SendMessageW(hwnd(), WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hIcon32));
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
		pSelf->_wndMsg._wnd._hWnd = hDlg;
		SetWindowLongPtrW(hDlg, DWLP_USER, reinterpret_cast<LONG_PTR>(pSelf));
		break;
	default:
		pSelf = reinterpret_cast<DlgBase*>(GetWindowLongPtrW(hDlg, DWLP_USER));
	}

	// If no pointer stored, then no processing is done.
	// Prevents processing before WM_INITDIALOG and after WM_NCDESTROY.
	if (!pSelf) return FALSE;

	// Execute the event handlers.
	WindowMsg::ProcResult ret = pSelf->_wndMsg.process_msgs(msg, wp, lp);

	if (msg == WM_NCDESTROY) { // always check
		SetWindowLongPtrW(hDlg, DWLP_USER, 0);
		pSelf->_wndMsg._wnd._hWnd = nullptr;
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
	_dlgBase._wndMsg._userEvents.wm_close([this]() {
		DestroyWindow(hwnd());
	});

	_dlgBase._wndMsg._userEvents.wm_nc_destroy([]() {
		PostQuitMessage(0);
	});
}

int DlgMain::run(HINSTANCE hInst, int cmdShow) {
	_dlgBase.create_dialog_param(hInst, nullptr);
	_dlgBase.set_icon(hInst, _iconId);
	HACCEL hAccel = _dlgBase.load_accel(hInst, _accelTblId);
	ShowWindow(hwnd(), cmdShow);

	return _dlgBase._wndMsg.main_loop(hAccel, true);
}

////////////////////////////////////////////////////////////////////////////////

DlgModal::DlgModal(const WindowParent &parent, WORD dlgId)
	: _parent{parent}, _dlgBase{dlgId}
{
	_dlgBase._wndMsg._userEvents.wm_close([this]() {
		EndDialog(hwnd(), 0);
	});
}

void DlgModal::show() {
	HINSTANCE hInst = reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(_parent.hwnd(), GWLP_HINSTANCE));
	_dlgBase.dialog_box_param(hInst, _parent.hwnd());
}

////////////////////////////////////////////////////////////////////////////////

DlgControl::DlgControl(WindowParent &parent, WORD dlgId, WORD ctrlId, POINT pos, Lay layout)
	: _dlgBase{dlgId}
{
	parent.wnd_msg()._preEvents.wm_create_or_init_dialog([this, pParent = &parent, ctrlId, pos, layout]() {
		HINSTANCE hInst = reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(pParent->hwnd(), GWLP_HINSTANCE));
		_dlgBase.create_dialog_param(hInst, pParent->hwnd());
		SetWindowLongPtrW(hwnd(), GWLP_ID, reinterpret_cast<LONG_PTR>(NativeCtrl::valid_ctrl_id(ctrlId))); // give the control its ID
		SetWindowPos(hwnd(), nullptr, pos.x, pos.y, 0, 0, SWP_NOZORDER | SWP_NOMOVE);
		pParent->wnd_msg()._layout.add(hwnd(), layout);
	});
}
