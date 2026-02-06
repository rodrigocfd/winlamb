#pragma once
#include "ui-base.hpp"

namespace wl {

	/// Options to create a `WindowMain` programmatically.
	///
	/// The fields are declared in alphabetical order to make it easy to work
	/// with [designated initializers], which require the fields to be set in the
	/// same order they appear in the struct.
	///
	/// [designated initializers]: https://en.cppreference.com/w/cpp/language/aggregate_initialization.html#Designated_initializers
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
		/// Optional window [accelerator table], to define keyboard shortcuts.
		///
		/// [accelerator table]: https://learn.microsoft.com/en-us/windows/win32/learnwin32/accelerator-tables
		HACCEL hAccelTable = nullptr;
		/// The window background brush passed to [`WNDCLASSEX`].
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		HBRUSH hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
		/// The window cursor passed to [`WNDCLASSEX`].
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		HCURSOR hCursor = nullptr;
		/// Optional window main menu, passed to [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		HMENU hMenu = nullptr;
		/** Optional window icon resource ID. */
		WORD iconId = 0;
		/// In most applications, the window loop calls [`IsDialogMessage`] so
		/// child control messages will properly work. However, this has the
		/// side-effect of inhibiting [`WM_CHAR`] messages from being sent, which
		/// is bad for applications like text editors.
		///
		/// So if your application has no child controls and needs to process
		/// `WM_CHAR` messages, pass `false` to suppress `IsDialogMessage` call.
		///
		/// [`IsDialogMessage`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-isdialogmessagew
		/// [`WM_CHAR`]: https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-char
		bool processDlgMsgs = true;
		/// The window size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::MainOpts myOpts{
		///     .size = wl::dpi::sz(500, 300),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SIZE size = {.cx = 500, .cy = 300};
		/// The [window style] passed to [`CreateWindowEx`].
		///
		/// For a resizable window, use:
		///
		/// ```cpp
		/// wl::MainOpts myOpts{
		///     .style = wl::MainOpts{}.style | WS_SIZEBOX | WS_MAXIMIZEBOX,
		/// };
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
		/// The window title, passed to [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		std::wstring title{};
	};

	/// Options to create a `WindowModal` programmatically.
	///
	/// The fields are declared in alphabetical order to make it easy to work
	/// with [designated initializers], which require the fields to be set in the
	/// same order they appear in the struct.
	///
	/// [designated initializers]: https://en.cppreference.com/w/cpp/language/aggregate_initialization.html#Designated_initializers
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
		/// In most applications, the window loop calls [`IsDialogMessage`] so
		/// child control messages will properly work. However, this has the
		/// side-effect of inhibiting [`WM_CHAR`] messages from being sent, which
		/// is bad for applications like text editors.
		///
		/// So if your application has no child controls and needs to process
		/// `WM_CHAR` messages, pass `false` to suppress `IsDialogMessage` call.
		///
		/// [`IsDialogMessage`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-isdialogmessagew
		/// [`WM_CHAR`]: https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-char
		bool processDlgMsgs = true;
		/// The window size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::ModalOpts myOpts{
		///     .size = wl::dpi::sz(400, 200),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SIZE size = {.cx = 400, .cy = 200};
		/// The [window style] passed to [`CreateWindowEx`].
		///
		/// For a resizable window, use:
		///
		/// ```cpp
		/// wl::ModalOpts myOpts{
		///     .style = wl::ModalOpts{}.style | WS_SIZEBOX | WS_MAXIMIZEBOX,
		/// };
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
		/// The window title, passed to [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		std::wstring title{};
	};

	/// Options to create a `WindowControl` programmatically.
	///
	/// The fields are declared in alphabetical order to make it easy to work
	/// with [designated initializers], which require the fields to be set in the
	/// same order they appear in the struct.
	///
	/// [designated initializers]: https://en.cppreference.com/w/cpp/language/aggregate_initialization.html#Designated_initializers
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
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		WORD ctrlId = 0;
		/// The window background brush passed to [`WNDCLASSEX`].
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		HBRUSH hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
		/// The window cursor passed to [`WNDCLASSEX`].
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		HCURSOR hCursor = nullptr;
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		Lay layout = Lay::hold_hold;
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::ControlOpts myOpts{
		///     .pos = wl::dpi::pt(10, 10),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		POINT pos{};
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::ControlOpts myOpts{
		///     .pos = wl::dpi::sz(100, 100),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SIZE size = {.cx = 100, .cy = 100};
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
		/// wl::ControlOpts myOpts{
		///     .style = wl::ControlOpts{}.style | WS_EX_CLIENTEDGE,
		/// };
		/// ```
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD styleEx = WS_EX_LEFT;
	};

}

namespace _wl_internal {

	/** Base to all raw container windows. */
	class RawBase final : private wl::NoCopyNoMove {
	public:
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
	class RawMain final : private wl::NoCopyNoMove {
	public:
		explicit RawMain(wl::MainOpts creationOpts);
		int run(HINSTANCE hInst, int cmdShow);

		RawBase _rawBase{};
		wl::MainOpts _opts;
		HWND _hWndChildPrevFocus = nullptr;
	};

	/** Modal raw window. */
	class RawModal final : private wl::NoCopyNoMove {
	public:
		RawModal(const WndBase &parentWndBase, wl::ModalOpts creationOpts);
		void show();

		RawBase _rawBase{};
		const WndBase &_parent;
		wl::ModalOpts _opts;
		HWND _hWndChildPrevFocusParent = nullptr;
	};

	/** Control raw window. */
	class RawControl final : private wl::NoCopyNoMove {
	public:
		RawControl(WndBase &parentWndBase, wl::ControlOpts creationOpts);

		RawBase _rawBase{};
	};

	/** Base to all dialog container windows. */
	class DlgBase final : private wl::NoCopyNoMove {
	public:
		constexpr explicit DlgBase(WORD dlgId) : _wndBase{true}, _dlgId{dlgId} { }

		void create_dialog_param(HINSTANCE hInst, HWND hParent);
		void dialog_box_param(HINSTANCE hInst, HWND hParent);
		void set_icon(HINSTANCE hInst, WORD iconId) const;
		HACCEL load_accel(HINSTANCE hInst, WORD accelTblId) const;

		static INT_PTR CALLBACK dlg_proc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);

		WndBase _wndBase;
		WORD _dlgId;
	};

	/** Main dialog window. */
	class DlgMain final : private wl::NoCopyNoMove {
	public:
		DlgMain(WORD dlgId, WORD iconId, WORD accelTblId);
		int run(HINSTANCE hInst, int cmdShow);

		DlgBase _dlgBase;
		WORD _iconId, _accelTblId;
	};

	/** Modal dialog window. */
	class DlgModal final : private wl::NoCopyNoMove {
	public:
		DlgModal(const WndBase &parentWndBase, WORD dlgId);
		void show();

		DlgBase _dlgBase;
		const WndBase &_parent;
	};

	/** Control dialog window. */
	class DlgControl final : private wl::NoCopyNoMove {
	public:
		DlgControl(WndBase &parentWndBase, WORD dlgId, WORD ctrlId, POINT pos, wl::Lay layout);

		DlgBase _dlgBase;
	};

}
