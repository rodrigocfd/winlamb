#pragma once
#include "window.h"

namespace _wl_internal {

	// Base to all dialog container windows.
	class DialogBase final {
	public:
		DialogBase() = delete;
		DialogBase(const DialogBase&) = delete;
		DialogBase(DialogBase&&) = delete;
		DialogBase& operator=(const DialogBase&) = delete;
		DialogBase& operator=(DialogBase&&) = delete;

		constexpr DialogBase(WORD dlgId) : _wndMsg{true}, _dlgId{dlgId} { }

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

	// Main dialog window.
	class DialogMain final {
	public:
		DialogMain() = delete;
		DialogMain(const DialogMain&) = delete;
		DialogMain(DialogMain&&) = delete;
		DialogMain& operator=(const DialogMain&) = delete;
		DialogMain& operator=(DialogMain&&) = delete;

		DialogMain(WORD dlgId, WORD iconId, WORD accelTblId);

		[[nodiscard]] constexpr HWND hwnd() const { return _dlgBase.hwnd(); }
		int run(HINSTANCE hInst, int cmdShow);

		DialogBase _dlgBase;
		WORD _iconId, _accelTblId;
	};

}

namespace wl { class WindowParent; }

namespace _wl_internal {

	// Modal dialog window.
	class DialogModal final {
	public:
		DialogModal() = delete;
		DialogModal(const DialogModal&) = delete;
		DialogModal(DialogModal&&) = delete;
		DialogModal& operator=(const DialogModal&) = delete;
		DialogModal& operator=(DialogModal&&) = delete;

		DialogModal(const wl::WindowParent &parent, WORD dlgId);

		[[nodiscard]] constexpr HWND hwnd() const { return _dlgBase.hwnd(); }
		void show();

		DialogBase _dlgBase;
		const wl::WindowParent &_parent; // mandatory
	};

}
