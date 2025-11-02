#pragma once
#include "window.h"

namespace _wl_internal {

	/** Base to all dialog container windows. */
	class DlgBase final : wl::NonMovable {
	public:
		constexpr DlgBase(WORD dlgId) : _wndMsg{true}, _dlgId{dlgId} { }

		[[nodiscard]] constexpr HWND hwnd() const { return _wndMsg.hwnd(); }
		void create_dialog_param(HINSTANCE hInst, HWND hParent);
		void dialog_box_param(HINSTANCE hInst, HWND hParent);
		void set_icon(HINSTANCE hInst, WORD iconId) const;
		HACCEL load_accel(HINSTANCE hInst, WORD accelTblId) const;

		static INT_PTR CALLBACK dlg_proc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);

		WindowMsg _wndMsg;
		WORD _dlgId;
	};

}

namespace _wl_internal {

	/** Main dialog window. */
	class DlgMain final : wl::NonMovable {
	public:
		DlgMain(WORD dlgId, WORD iconId, WORD accelTblId);

		[[nodiscard]] constexpr HWND hwnd() const { return _dlgBase.hwnd(); }
		int run(HINSTANCE hInst, int cmdShow);

		DlgBase _dlgBase;
		WORD _iconId, _accelTblId;
	};

}

namespace wl { class WindowParent; }

namespace _wl_internal {

	/** Modal dialog window. */
	class DlgModal final : wl::NonMovable {
	public:
		DlgModal(const wl::WindowParent &parent, WORD dlgId);

		[[nodiscard]] constexpr HWND hwnd() const { return _dlgBase.hwnd(); }
		void show();

		DlgBase _dlgBase;
		const wl::WindowParent &_parent;
	};

}

namespace _wl_internal {

	/** Control dialog window. */
	class DlgControl final : wl::NonMovable {
	public:
		DlgControl(wl::WindowParent &parent, WORD dlgId, WORD ctrlId, POINT pos, wl::Lay layout = wl::Lay::hold_hold);

		[[nodiscard]] constexpr HWND hwnd() const { return _dlgBase.hwnd(); }

		DlgBase _dlgBase;
	};

}
