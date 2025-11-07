#pragma once
#include "lib-include-win.h"
#include "wnd-base.h"

namespace _wl_internal {

	/** Base to all dialog container windows. */
	class DlgBase final {
	public:
		DlgBase(DlgBase&&) = delete; // non-copyable, non-movable

		constexpr DlgBase(WORD dlgId) : _wndBase{true}, _dlgId{dlgId} { }

		void create_dialog_param(HINSTANCE hInst, HWND hParent);
		void dialog_box_param(HINSTANCE hInst, HWND hParent);
		void set_icon(HINSTANCE hInst, WORD iconId) const;
		HACCEL load_accel(HINSTANCE hInst, WORD accelTblId) const;

		static INT_PTR CALLBACK dlg_proc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);

		WndBase _wndBase;
		WORD _dlgId;
	};

	/** Main dialog window. */
	class DlgMain final {
	public:
		DlgMain(DlgMain&&) = delete; // non-copyable, non-movable

		DlgMain(WORD dlgId, WORD iconId, WORD accelTblId);
		int run(HINSTANCE hInst, int cmdShow);

		DlgBase _dlgBase;
		WORD _iconId, _accelTblId;
	};

	/** Modal dialog window. */
	class DlgModal final {
	public:
		DlgModal(DlgModal&&) = delete; // non-copyable, non-movable

		DlgModal(const wl::WindowParent &parent, WORD dlgId);
		void show();

		DlgBase _dlgBase;
		const wl::WindowParent &_parent;
	};

	/** Control dialog window. */
	class DlgControl final {
	public:
		DlgControl(DlgControl&&) = delete; // non-copyable, non-movable

		DlgControl(wl::WindowParent &parent, WORD dlgId, WORD ctrlId, POINT pos, wl::Lay layout);

		DlgBase _dlgBase;
	};

}
