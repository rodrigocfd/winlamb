
#include <system_error>
#include "window_dlg.h"
#include "run_funcs.h"
using namespace _wli;
using namespace wl;

void window_dlg::_create_dialog(HINSTANCE hinst, HWND hparent) const
{
	if (hwnd() != nullptr) {
		throw std::logic_error("Dialog already created.");
	}

	if (!CreateDialogParamW(hinst, MAKEINTRESOURCEW(_dialog_id), hparent,
		_dlg_proc, reinterpret_cast<LPARAM>(this)))
	{
		throw std::system_error(GetLastError(), std::system_category(),
			"CreateDialogParam failed");
	}
}

void window_dlg::_dialog_box(HINSTANCE hinst, HWND hparent) const
{
	if (hwnd() != nullptr) {
		throw std::logic_error("Dialog already created.");
	}

	DialogBoxParamW(hinst, MAKEINTRESOURCEW(_dialog_id), hparent,
		_dlg_proc, reinterpret_cast<LPARAM>(this));
}

INT_PTR window_dlg::_dlg_proc(HWND hdlg, UINT msg, WPARAM wp, LPARAM lp)
{
	window_dlg* _this = nullptr;

	if (msg == WM_INITDIALOG) {
		_this = reinterpret_cast<window_dlg*>(lp);
		_this->_hwnd = hdlg; // assign actual HWND
		SetWindowLongPtrW(hdlg, DWLP_USER, reinterpret_cast<LONG_PTR>(_this));
	} else {
		_this = reinterpret_cast<window_dlg*>(GetWindowLongPtrW(hdlg, DWLP_USER));
	}

	// If object pointer is not stored, then no processing is done.
	// Prevents processing before WM_INITDIALOG and after WM_NCDESTROY.
	if (_this != nullptr) {
		// Process all internal events.
		_this->_events_internal._process_all(msg, wp, lp);

		// Child controls are created in internal events closures,
		// so we put the system font after running them.
		const font& sys_ui_font = ui_font();
		if (msg == WM_INITDIALOG) {
			SendMessageW(hdlg, WM_SETFONT,
				reinterpret_cast<WPARAM>(sys_ui_font.hfont()), MAKELPARAM(0, FALSE));
			EnumChildWindows(hdlg, [](HWND hchild, LPARAM lp) -> BOOL {
				HFONT hfont = reinterpret_cast<HFONT>(lp);
				SendMessageW(hchild, WM_SETFONT, reinterpret_cast<WPARAM>(hfont), MAKELPARAM(0, FALSE));
				return TRUE;
			}, reinterpret_cast<LPARAM>(sys_ui_font.hfont()));
		}

		// Try to process the message with an user callback.
		auto [was_handled, ret] = _this->_events._process_last(msg, wp, lp);

		// No further messages processed after this one.
		if (msg == WM_NCDESTROY) {
			SetWindowLongPtrW(hdlg, DWLP_USER, 0);
			_this->_hwnd = nullptr; // clear actual HWND
		}

		if (was_handled) {
			return ret.has_value() ? ret.value() : TRUE; // message processed
		}
	}

	return FALSE; // message not processed
}
