#pragma once
#include "window.h"

namespace _wl_internal {

	/** Base to all raw container windows. */
	class RawBase final : wl::NonMovable {
	public:
		[[nodiscard]] constexpr HWND hwnd() const { return _wndMsg.hwnd(); }
		[[nodiscard]] ATOM register_class(HINSTANCE hInst, LPCWSTR className, DWORD classStyle,
			WORD iconId, HBRUSH hbrBackground, HCURSOR hCursor);
		void create_window(DWORD exStyle, ATOM className, LPCWSTR title, DWORD style,
			POINT pos, SIZE sz, HWND hParent, HMENU hMenu, HINSTANCE hInst);
		void focus_first_child() const;

		static LRESULT CALLBACK raw_proc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

		WindowMsg _wndMsg{false};
	};

}

namespace wl::opts {

	/** Options to create a WindowMain programmatically. */
	struct Main final {
		/// Class name passed to [`WNDCLASSEX`].
		///
		/// Defaults to an auto-generated string.
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		LPCWSTR className = nullptr;
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
		LPCWSTR title = nullptr;
		/// The window size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::opts::Main wndOpts{
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
		/// wl::opts::Main wndOpts{
		///     .style = wl::opts::Main{}.style | WS_SIZEBOX | WS_MAXIMIZEBOX,
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
		DWORD exStyle = WS_EX_LEFT;
		/** Optional window main menu. */
		HMENU hMenu = nullptr;
		/// Optional window [accelerator table].
		///
		/// [accelerator table]: https://learn.microsoft.com/en-us/windows/win32/learnwin32/accelerator-tables
		HACCEL hAccelTable = nullptr;

		/// In most applications, the window loop calls [`IsDialogMessage`] so child control messages will properly work.
		/// However, this has the side-effect of inhibiting [`WM_CHAR`] messages from being sent, which is bad for applications like text editors.
		///
		/// So if your application has no child controls and needs to process `WM_CHAR` messages, pass false to suppress `IsDialogMessage` call.
		///
		/// [`IsDialogMessage`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-isdialogmessagew
		/// [`WM_CHAR`]: https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-char
		bool processDlgMsgs = true;
	};

}

namespace _wl_internal {

	/** Main raw window. */
	class RawMain final : wl::NonMovable {
	public:
		explicit RawMain(wl::opts::Main options);

		[[nodiscard]] constexpr HWND hwnd() const { return _rawBase.hwnd(); }
		int run(HINSTANCE hInst, int cmdShow);

		RawBase _rawBase{};
		wl::opts::Main _opts;
		HWND _hWndChildPrevFocus = nullptr;
	};

}

namespace wl { class WindowParent; }

namespace wl::opts {

	/** Options to create a WindowModal programmatically. */
	struct Modal final {
		/// Class name passed to [`WNDCLASSEX`].
		///
		/// Defaults to an auto-generated string.
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		LPCWSTR className = nullptr;
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
		LPCWSTR title = nullptr;
		/// The window size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::opts::Main wndOpts{
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
		/// wl::opts::Main wndOpts{
		///     .style = wl::opts::Modal{}.style | WS_SIZEBOX | WS_MAXIMIZEBOX,
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
		DWORD exStyle = WS_EX_LEFT;

		/// In most applications, the window loop calls [`IsDialogMessage`] so child control messages will properly work.
		/// However, this has the side-effect of inhibiting [`WM_CHAR`] messages from being sent, which is bad for applications like text editors.
		///
		/// So if your application has no child controls and needs to process `WM_CHAR` messages, pass false to suppress `IsDialogMessage` call.
		///
		/// [`IsDialogMessage`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-isdialogmessagew
		/// [`WM_CHAR`]: https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-char
		bool processDlgMsgs = true;
	};

}

namespace _wl_internal {

	/** Modal raw window. */
	class RawModal final : wl::NonMovable {
	public:
		RawModal(wl::WindowParent &parent, wl::opts::Modal opts);

		[[nodiscard]] constexpr HWND hwnd() const { return _rawBase.hwnd(); }
		void show();

		RawBase _rawBase{};
		wl::WindowParent &_parent;
		wl::opts::Modal _opts;
		HWND _hWndChildPrevFocusParent = nullptr;
	};

}
