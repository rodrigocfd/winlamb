#include <system_error>
#include "window-dialog.h"
using namespace _wl_internal;

void DialogBase::create_dialog_param(HINSTANCE hInst, HWND hParent) {
	#ifdef _DEBUG
	if (hwnd())
		throw std::logic_error("Cannot create dialog twice.");
	#endif

	HWND hDlg = CreateDialogParamW(hInst, MAKEINTRESOURCEW(_dlgId), hParent,
		dlg_proc, reinterpret_cast<LPARAM>(this));
	#ifdef _DEBUG
	if (!hDlg)
		throw std::system_error(GetLastError(), std::system_category(), "CreateDialogParam failed.");
	#endif
}

void DialogBase::dialog_box_param(HINSTANCE hInst, HWND hParent) {
	#ifdef _DEBUG
	if (hwnd())
		throw std::logic_error("Cannot create dialog twice.");
	#endif

	INT_PTR ret = DialogBoxParamW(hInst, MAKEINTRESOURCEW(_dlgId), hParent,
		dlg_proc, reinterpret_cast<LPARAM>(this));
	#ifdef _DEBUG
	if (!ret)
		throw std::system_error(GetLastError(), std::system_category(), "DialogBoxParam failed.");
	#endif
}

void DialogBase::set_icon(HINSTANCE hInst, WORD iconId) const {
	if (!iconId) return;

	HANDLE hIcon16 = LoadImageW(hInst, MAKEINTRESOURCEW(iconId), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	#ifdef _DEBUG
	if (!hIcon16)
		throw std::system_error(GetLastError(), std::system_category(), "LoadImage 16x16 failed.");
	#endif

	HANDLE hIcon32 = LoadImageW(hInst, MAKEINTRESOURCEW(iconId), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
	#ifdef _DEBUG
	if (!hIcon32)
		throw std::system_error(GetLastError(), std::system_category(), "LoadImage 32x32 failed.");
	#endif

	SendMessageW(hwnd(), WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIcon16));
	SendMessageW(hwnd(), WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hIcon32));
}

HACCEL DialogBase::load_accel(HINSTANCE hInst, WORD accelTblId) const {
	if (!accelTblId) return nullptr;

	HACCEL hAccel = LoadAcceleratorsW(hInst, MAKEINTRESOURCEW(accelTblId));
	#ifdef _DEBUG
	if (!hAccel)
		throw std::system_error(GetLastError(), std::system_category(), "LoadAccelerators failed.");
	#endif
	return hAccel;
}

INT_PTR CALLBACK DialogBase::dlg_proc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp) {
	DialogBase *pSelf = nullptr;
	switch (msg) {
	case WM_INITDIALOG:
		pSelf = reinterpret_cast<DialogBase*>(lp);
		pSelf->_wndMsg._wnd._hWnd = hDlg;
		SetWindowLongPtrW(hDlg, DWLP_USER, reinterpret_cast<LONG_PTR>(pSelf));
		break;
	default:
		pSelf = reinterpret_cast<DialogBase*>(GetWindowLongPtrW(hDlg, DWLP_USER));
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

DialogMain::DialogMain(WORD dlgId, WORD iconId, WORD accelTblId)
	: _dlgBase{dlgId}, _iconId{iconId}, _accelTblId{accelTblId}
{
	_dlgBase._wndMsg.on().wm_close([this]() {
		DestroyWindow(hwnd());
	});

	_dlgBase._wndMsg.on().wm_nc_destroy([]() {
		PostQuitMessage(0);
	});
}

int DialogMain::run(HINSTANCE hInst, int cmdShow) {
	_dlgBase.create_dialog_param(hInst, nullptr);
	_dlgBase.set_icon(hInst, _iconId);
	HACCEL hAccel = _dlgBase.load_accel(hInst, _accelTblId);
	ShowWindow(hwnd(), cmdShow);

	return _dlgBase._wndMsg.main_loop(hAccel);
}
