#pragma once
#include "lib-include-win.h"
#include <CommCtrl.h>
#include "layout.h"
#include "str.h"

namespace wl {
	class StatusBar;
}

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
		DWORD exStyle = WS_EX_LEFT;
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
		DWORD exStyle = WS_EX_LEFT;

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
		DWORD windowStyle = WS_CHILD | WS_TABSTOP | WS_GROUP | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// Example adding a border:
		///
		/// ```cpp
		/// wnd.setup().windowExStyle |= WS_EX_CLIENTEDGE;
		/// ```
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD windowExStyle = WS_EX_LEFT;
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

	/** Options to create a `Button` programmatically. */
	struct ButtonOpts final {
		/// Control text passed to [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		std::wstring text{};
		/// The [window style] passed to [`CreateWindowEx`].
		///
		/// [window style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD windowStyle = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE;
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD windowExStyle = WS_EX_LEFT;
		/// The [Button style] passed to [`CreateWindowEx`].
		///
		/// [Button style]: https://learn.microsoft.com/en-us/windows/win32/controls/button-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD ctrlStyle = BS_PUSHBUTTON;
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// btn.setup().pos = wl::dpi::pt(10, 10);
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		POINT pos{};
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// btn.setup().size = wl::dpi::sz(88, 26);
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SIZE size = {.cx = 88, .cy = 26};
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		Lay layout = Lay::hold_hold;
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		WORD ctrlId = 0;
	};

	/** Options to create a `CheckBox` programmatically. */
	struct CheckBoxOpts final {
		/// Control text passed to [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		std::wstring text{};
		/// The [window style] passed to [`CreateWindowEx`].
		///
		/// [window style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD windowStyle = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE;
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD windowExStyle = WS_EX_LEFT;
		/// The [CheckBox style] passed to [`CreateWindowEx`].
		///
		/// [CheckBox style]: https://learn.microsoft.com/en-us/windows/win32/controls/button-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD ctrlStyle = BS_AUTOCHECKBOX;
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// chk.setup().pos = wl::dpi::pt(10, 10);
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		POINT pos{};
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// chk.setup().size = wl::dpi::sz(88, 26);
		/// ```
		///
		/// If not defined, the control will resize to automatically fit its initial text.
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SIZE size{};
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		WORD ctrlId = 0;
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		Lay layout = Lay::hold_hold;
		/** Initial state. */
		WORD state = BST_UNCHECKED;
	};

	/** Options to create a `ComboBox` programmatically. */
	struct ComboBoxOpts final {
		/// The [window style] passed to [`CreateWindowEx`].
		///
		/// [window style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD windowStyle = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE;
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD windowExStyle = WS_EX_LEFT;
		/// The [ComboBox style] passed to [`CreateWindowEx`].
		///
		/// [ComboBox style]: https://learn.microsoft.com/en-us/windows/win32/controls/button-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD ctrlStyle = CBS_DROPDOWNLIST;
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// cmb.setup().pos = wl::dpi::pt(10, 10);
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		POINT pos{};
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// cmb.setup().width = wl::dpi::x(100);
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		int width = 100;
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		WORD ctrlId = 0;
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		Lay layout = Lay::hold_hold;
		/// Texts to be added.
		///
		/// Example:
		///
		/// ```cpp
		/// cmb.setup().texts = {L"Hello", L"World"};
		/// ```
		std::vector<std::wstring> texts{};
	};

	/** Options to create a `DateTimePicker` programmatically. */
	struct DateTimePickerOpts final {
		/// The [window style] passed to [`CreateWindowEx`].
		///
		/// [window style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD windowStyle = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE;
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD windowExStyle = WS_EX_LEFT | WS_EX_CLIENTEDGE;
		/// The [ComboBox style] passed to [`CreateWindowEx`].
		///
		/// [ComboBox style]: https://learn.microsoft.com/en-us/windows/win32/controls/button-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD ctrlStyle = DTS_LONGDATEFORMAT;
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// dtp.setup().pos = wl::dpi::pt(10, 10);
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		POINT pos{};
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// dtp.setup().size = wl::dpi::sz(230, 23);
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SIZE size = {.cx = 230, .cy = 23};
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		WORD ctrlId = 0;
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		Lay layout = Lay::hold_hold;
		/// Initial [`SYSTEMTIME`].
		///
		/// Example:
		///
		/// ```cpp
		/// SYSTEMTIME st{};
		/// GetLocalTime(&st);
		/// dtp.setup().value = st;
		/// ```
		///
		/// [`SYSTEMTIME`]: https://learn.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-systemtime
		SYSTEMTIME value{};
	};

	/** Options to create an `Edit` programmatically. */
	struct EditOpts final {
		/// Control text passed to [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		std::wstring text{};
		/// The [window style] passed to [`CreateWindowEx`].
		///
		/// [window style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD windowStyle = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE;
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD windowExStyle = WS_EX_LEFT | WS_EX_CLIENTEDGE;
		/// The [Edit style] passed to [`CreateWindowEx`].
		///
		/// [Edit style]: https://learn.microsoft.com/en-us/windows/win32/controls/button-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD ctrlStyle = ES_AUTOHSCROLL | ES_NOHIDESEL;
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// txt.setup().pos = wl::dpi::pt(10, 10);
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		POINT pos{};
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// btn.setup().size = wl::dpi::sz(100, 23);
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SIZE size = {.cx = 100, .cy = 23};
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		Lay layout = Lay::hold_hold;
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		WORD ctrlId = 0;
	};

	/** Options to create a `ListView` programmatically. */
	struct ListViewOpts final {
		/// The [window style] passed to [`CreateWindowEx`].
		///
		/// [window style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD windowStyle = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE;
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD windowExStyle = WS_EX_LEFT | WS_EX_CLIENTEDGE;
		/// The [ListView style] passed to [`CreateWindowEx`].
		///
		/// Note that, for safety reasons, `LVS_SHAREIMAGELISTS` will always be set.
		///
		/// [ListView style]: https://learn.microsoft.com/en-us/windows/win32/controls/list-view-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD ctrlStyle = LVS_REPORT | LVS_NOSORTHEADER | LVS_SHOWSELALWAYS;
		/// The [ListView extended styles] applied right after the control is created.
		///
		/// [ListView extended styles]: https://learn.microsoft.com/en-us/windows/win32/controls/extended-list-view-styles
		DWORD ctrlExStyle = LVS_EX_FULLROWSELECT;
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// lv.setup().pos = wl::dpi::pt(10, 10);
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		POINT pos{};
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// lv.setup().size = wl::dpi::sz(120, 120);
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SIZE size = {.cx = 120, .cy = 120};
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		Lay layout = Lay::hold_hold;
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		WORD ctrlId = 0;
		/// Context menu resource to be loaded as the context menu with [`LoadMenu`].
		/// If defined, overwrites `hMenuContext`.
		///
		/// This menu will be owned by the list view, and destroyed automatically.
		///
		/// [`LoadMenu`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-loadmenuw
		WORD contextMenuId = 0;
		/// Optional list view context menu, usually created programmatically with [`CreatePopupMenu`].
		///
		/// This menu will be owned by the list view, and destroyed automatically.
		///
		/// Ignored if you define `contextMenuId`.
		///
		/// [`CreatePopupMenu`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createpopupmenu
		HMENU hMenuContext = nullptr;

		/** A column to be added to the `ListView`. */
		struct Col final {
			std::wstring name{};
			int width = 0;
		};
		/// Columns to be added, title and width.
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// lv.setup().columns = {
		///     {L"First", wl::dpi::x(100)},
		///     {L"Second", wl::dpi::x(120)},
		/// };
		/// ```
		std::vector<Col> columns{};
	};

	/** Options to create a `MonthCalendar` programmatically. */
	struct MonthCalendarOpts final {
		/// The [window style] passed to [`CreateWindowEx`].
		///
		/// [window style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD windowStyle = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE;
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD windowExStyle = WS_EX_LEFT;
		/// The [Static style] passed to [`CreateWindowEx`].
		///
		/// [Static style]: https://learn.microsoft.com/en-us/windows/win32/controls/static-control-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD ctrlStyle = 0;
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// mcal.setup().pos = wl::dpi::pt(10, 10);
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		POINT pos{};
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		WORD ctrlId = 0;
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		Lay layout = Lay::hold_hold;
		/// Initial [`SYSTEMTIME`].
		///
		/// Example:
		///
		/// ```cpp
		/// SYSTEMTIME st{};
		/// GetLocalTime(&st);
		/// mcal.setup().value = st;
		/// ```
		///
		/// [`SYSTEMTIME`]: https://learn.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-systemtime
		SYSTEMTIME value{};
	};

	/** Options to create a `RadioButton` programmatically. */
	struct RadioButtonOpts final {
		/// Control text passed to [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		std::wstring text{};
		/// The [window style] passed to [`CreateWindowEx`].
		///
		/// [window style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD windowStyle = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE;
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD windowExStyle = WS_EX_LEFT;
		/// The [RadioButton style] passed to [`CreateWindowEx`].
		///
		/// [RadioButton style]: https://learn.microsoft.com/en-us/windows/win32/controls/button-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD ctrlStyle = BS_AUTORADIOBUTTON;
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// rad.setup().pos = wl::dpi::pt(10, 10);
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		POINT pos{};
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// rad.setup().size = wl::dpi::sz(88, 26);
		/// ```
		///
		/// If not defined, the control will resize to automatically fit its initial text.
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SIZE size{};
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		WORD ctrlId = 0;
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		Lay layout = Lay::hold_hold;
		/// Initial state.
		///
		/// Only one radio button can be selected at once in its group.
		bool selected = false;
	};

	/** Options to create a `Static` programmatically. */
	struct StaticOpts final {
		/// Control text passed to [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		std::wstring text{};
		/// The [window style] passed to [`CreateWindowEx`].
		///
		/// [window style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD windowStyle = WS_CHILD | WS_VISIBLE;
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD windowExStyle = WS_EX_LEFT;
		/// The [Static style] passed to [`CreateWindowEx`].
		///
		/// [Static style]: https://learn.microsoft.com/en-us/windows/win32/controls/static-control-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD ctrlStyle = SS_LEFT | SS_NOTIFY;
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// lbl.setup().pos = wl::dpi::pt(10, 10);
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		POINT pos{};
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// lbl.setup().size = wl::dpi::sz(88, 26);
		/// ```
		///
		/// If not defined, the control will resize to automatically fit its initial text.
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SIZE size{};
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		WORD ctrlId = 0;
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		Lay layout = Lay::hold_hold;
	};

	/** Options to create a `StatusBar` programmatically. */
	struct StatusBarOpts final {
		/// Adds a fixed part to the `StatusBar`. When the parent window is resized, this part will keep its width.
		///
		/// Prefer using a DPI-aware width:
		///
		/// ```cpp
		/// sb.setup().part_fixed(wl::dpi::x(200), L"Foo");
		/// ```
		///
		/// If you want to pass a zero-based `iconIndex`, you must feed the icon first:
		///
		/// ```cpp
		/// sb.icons().add_resource(ICO_FOO);
		/// sb.setup().part_fixed(wl::dpi::x(200), L"Foo", 0);
		/// ```
		void part_fixed(UINT width, WStrView text = L"", int iconIndex = -1) {
			_parts.emplace_back(width, 0, text.c_str(), iconIndex);
		}

		/// Adds a resizable part to the `StatusBar`. When the parent window is resized, this part will resize as well.
		///
		/// How `resizeWeight` works:
		/// - Suppose you have 3 parts, respectively with weights of 1, 1 and 2.
		/// - If available client area is 400px, respective part widths will be 100, 100 and 200px.
		///
		/// Example:
		///
		/// ```cpp
		/// sb.setup().part_fixed(1, L"Foo");
		/// ```
		///
		/// If you want to pass a zero-based `iconIndex`, you must feed the icon first:
		///
		/// ```cpp
		/// sb.icons().add_resource(ICO_FOO);
		/// sb.setup().part_fixed(1, L"Foo", 0);
		/// ```
		void part_resizable(UINT resizeWeight, WStrView text = L"", int iconIndex = -1) {
			_parts.emplace_back(0, resizeWeight, text.c_str(), iconIndex);
		}

	private:
		struct Part final {
			int sizePixels = 0; // one used, the other zero
			int resizeWeight = 0;
			std::wstring text{};
			int iconIndex = -1;
		};
		std::vector<Part> _parts{};
		friend wl::StatusBar;
	};

}
