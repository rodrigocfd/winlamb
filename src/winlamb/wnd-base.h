#pragma once
#include <functional>
#include <optional>
#include <string>
#include "lib-include-win.h"
#include "wnd-interfaces.h"
#include "wnd-opts.h"
#include "events-wnd.h"
#include "layout.h"

namespace _wl_internal {

	/// Base to all raw and dialog windows.
	/// Stores the pre, user and post window messages for container windows.
	/// Exposes exception-safe multi-threading operations.
	class WndBase final {
	public:
		WndBase(WndBase&&) = delete; // non-copyable, non-movable

		constexpr explicit WndBase(bool isDlg)
			: _isDlg{isDlg}, _preEvents{isDlg}, _userEvents{isDlg}, _postEvents{isDlg} { }

		void ui_thread(std::function<void()> &&cb) const;

		struct ProcResult final {
			bool hasPre, hasPost;
			std::optional<LRESULT> userRet;
		};
		ProcResult process_msgs(UINT msg, WPARAM wp, LPARAM lp);

		int main_loop(HACCEL hAccel, bool processDlgMsgs);
		void modal_loop(bool processDlgMsgs);

		bool _isDlg;
		HWND _hWnd = nullptr; // _hWnd member is set in wndproc
		Layout _layout{};
		InternalEvents _preEvents;
		wl::events::WindowEvents _userEvents;
		InternalEvents _postEvents;
	};

	/// Base to all native controls.
	/// Stores the subclass messages.
	class NativeCtrlBase final {
	public:
		NativeCtrlBase(NativeCtrlBase&&) = delete; // non-copyable, non-movable

		explicit NativeCtrlBase(wl::WindowParent &owner);

		void create_wnd(WORD ctrlId, DWORD exStyle, const wchar_t *className,
			std::wstring &&title, DWORD style, POINT pos, SIZE size);
		void assign_dlg(WORD ctrlId);
		void install_subclass();

		static LRESULT CALLBACK subclass_proc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp,
			UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

		HWND _hWnd = nullptr; // _hWnd member is set during control creation
		WndBase &_parentWndBase;
		wl::events::WindowEvents _subclassEvents{false};
	};

	/** Base to all raw container windows. */
	class RawBase final {
	public:
		RawBase(RawBase&&) = delete; // non-copyable, non-movable

		constexpr RawBase() = default;

		[[nodiscard]] ATOM register_class(HINSTANCE hInst, std::wstring &&className, DWORD classStyle,
			WORD iconId, HBRUSH hbrBackground, HCURSOR hCursor);
		void create_window(DWORD exStyle, ATOM className, std::wstring &&title, DWORD style,
			POINT pos, SIZE sz, HWND hParent, HMENU hMenu, HINSTANCE hInst);
		void focus_first_child() const;

		static LRESULT CALLBACK raw_proc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

		WndBase _wndBase{false};
	};

	/** Main raw window. */
	class RawMain final {
	public:
		RawMain(RawMain&&) = delete; // non-copyable, non-movable

		RawMain();
		int run(HINSTANCE hInst, int cmdShow);

		RawBase _rawBase{};
		wl::opts::MainOpts _opts{};
		HWND _hWndChildPrevFocus = nullptr;
	};

	/** Modal raw window. */
	class RawModal final {
	public:
		RawModal(RawModal&&) = delete; // non-copyable, non-movable

		explicit RawModal(const wl::WindowParent &parent);
		void show();

		RawBase _rawBase{};
		const wl::WindowParent &_parent;
		wl::opts::ModalOpts _opts{};
		HWND _hWndChildPrevFocusParent = nullptr;
	};

	/** Control raw window. */
	class RawControl final {
	public:
		RawControl(RawControl&&) = delete; // non-copyable, non-movable

		RawControl(wl::WindowParent &parent);

		RawBase _rawBase{};
		wl::opts::ControlOpts _opts{};
	};

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
