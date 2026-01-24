#pragma once
#include "aux-str.hpp"
#include "ui-base.hpp"
#include <CommCtrl.h>

/** @brief Options to create window and controls programmatically. */
namespace wl::opts {

	/** Options to create a `WindowMain` programmatically. */
	struct MainOpts final {
		/// Class name passed to [`WNDCLASSEX`].
		///
		/// Defaults to an auto-generated string.
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		std::wstring className{};
		/// Class style passed to [`WNDCLASSEX`].
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		DWORD classStyle = CS_DBLCLKS;
		/// The window background brush passed to [`WNDCLASSEX`].
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		HBRUSH hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
		/// The window cursor passed to [`WNDCLASSEX`].
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		HCURSOR hCursor = nullptr;
		/** Optional window icon resource ID. */
		WORD iconId = 0;

		/// The window title, passed to [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		std::wstring title{};
		/// The window size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wnd.setup().size = wl::dpi::sz(500, 300);
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SIZE size = {.cx = 500, .cy = 300};
		/// The [window style] passed to [`CreateWindowEx`].
		///
		/// For a resizable window, use:
		///
		/// ```cpp
		/// wnd.setup().style |= WS_SIZEBOX | WS_MAXIMIZEBOX;
		/// ```
		///
		/// [window style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD style =  WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN | WS_BORDER | WS_VISIBLE | WS_MINIMIZEBOX;
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD styleEx = WS_EX_LEFT;
		/// Optional window main menu, passed to [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		HMENU hMenu = nullptr;
		/// Optional window [accelerator table], to define keyboard shortcuts.
		///
		/// [accelerator table]: https://learn.microsoft.com/en-us/windows/win32/learnwin32/accelerator-tables
		HACCEL hAccelTable = nullptr;

		/// In most applications, the window loop calls [`IsDialogMessage`] so child control messages will properly work.
		/// However, this has the side-effect of inhibiting [`WM_CHAR`] messages from being sent, which is bad for applications like text editors.
		///
		/// So if your application has no child controls and needs to process `WM_CHAR` messages, pass `false` to suppress `IsDialogMessage` call.
		///
		/// [`IsDialogMessage`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-isdialogmessagew
		/// [`WM_CHAR`]: https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-char
		bool processDlgMsgs = true;
	};

	/** Options to create a `WindowModal` programmatically. */
	struct ModalOpts final {
		/// Class name passed to [`WNDCLASSEX`].
		///
		/// Defaults to an auto-generated string.
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		std::wstring className{};
		/// Class style passed to [`WNDCLASSEX`].
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		DWORD classStyle = CS_DBLCLKS;
		/// The window background brush passed to [`WNDCLASSEX`].
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		HBRUSH hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
		/// The window cursor passed to [`WNDCLASSEX`].
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		HCURSOR hCursor = nullptr;
		/** Optional window icon resource ID. */
		WORD iconId = 0;

		/// The window title, passed to [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		std::wstring title{};
		/// The window size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wnd.setup().size = wl::dpi::sz(400, 200);
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SIZE size = {.cx = 400, .cy = 200};
		/// The [window style] passed to [`CreateWindowEx`].
		///
		/// For a resizable window, use:
		///
		/// ```cpp
		/// wnd.setup().style |= WS_SIZEBOX | WS_MAXIMIZEBOX;
		/// ```
		///
		/// [window style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD style =  WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN | WS_BORDER | WS_VISIBLE;
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD styleEx = WS_EX_LEFT;

		/// In most applications, the window loop calls [`IsDialogMessage`] so child control messages will properly work.
		/// However, this has the side-effect of inhibiting [`WM_CHAR`] messages from being sent, which is bad for applications like text editors.
		///
		/// So if your application has no child controls and needs to process `WM_CHAR` messages, pass `false` to suppress `IsDialogMessage` call.
		///
		/// [`IsDialogMessage`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-isdialogmessagew
		/// [`WM_CHAR`]: https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-char
		bool processDlgMsgs = true;
	};

	/** Options to create a `WindowControl` programmatically. */
	struct ControlOpts final {
		/// Class name passed to [`WNDCLASSEX`].
		///
		/// Defaults to an auto-generated string.
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		std::wstring className{};
		/// Class style passed to [`WNDCLASSEX`].
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		DWORD classStyle = CS_DBLCLKS;
		/// The window background brush passed to [`WNDCLASSEX`].
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		HBRUSH hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
		/// The window cursor passed to [`WNDCLASSEX`].
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		HCURSOR hCursor = nullptr;

		/// The [window style] passed to [`CreateWindowEx`].
		///
		/// [window style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD style = WS_CHILD | WS_TABSTOP | WS_GROUP | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// Example adding a border:
		///
		/// ```cpp
		/// wnd.setup().styleEx |= WS_EX_CLIENTEDGE;
		/// ```
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD styleEx = WS_EX_LEFT;
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wnd.setup().pos = wl::dpi::pt(10, 10);
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		POINT pos{};
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wnd.setup().pos = wl::dpi::sz(100, 100);
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SIZE size = {.cx = 100, .cy = 100};
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		Lay layout = Lay::hold_hold;
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		WORD ctrlId = 0;
	};

}

namespace _wl_internal {

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
		explicit RawModal(const WndBase &parentWndBase);
		void show();

		RawBase _rawBase{};
		const WndBase &_parent;
		wl::opts::ModalOpts _opts{};
		HWND _hWndChildPrevFocusParent = nullptr;
	};

	/** Control raw window. */
	class RawControl final {
	public:
		RawControl(RawControl&&) = delete; // non-copyable, non-movable
		RawControl(WndBase &parentWndBase);

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
		DlgModal(const WndBase &parentWndBase, WORD dlgId);
		void show();

		DlgBase _dlgBase;
		const WndBase &_parent;
	};

	/** Control dialog window. */
	class DlgControl final {
	public:
		DlgControl(DlgControl&&) = delete; // non-copyable, non-movable
		DlgControl(WndBase &parentWndBase, WORD dlgId, WORD ctrlId, POINT pos, wl::Lay layout);

		DlgBase _dlgBase;
	};

}
