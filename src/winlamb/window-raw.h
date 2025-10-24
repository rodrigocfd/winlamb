#pragma once
#include "window.h"

namespace _wl_internal {

	// Base to all raw container windows.
	class RawBase final {
	public:
		DEL_COPY_MOVE(RawBase);
		constexpr RawBase() = default;

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

// Options to create windows and controls programmatically, by calling CreateWindowEx().
namespace wl::opts {

	// Options to create a WindowMain programmatically.
	struct Main {
		// Class name passed to WNDCLASSEX.
		// Defaults to an auto-generated string.
		LPCWSTR className = nullptr;
		// Class style passed to WNDCLASSEX.
		// Defaults to: CS_DBLCLKS.
		DWORD classStyle = CS_DBLCLKS;
		// Optional window icon resource ID.
		WORD iconId = 0;
		// Window background brush.
		HBRUSH hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
		// Window cursor.
		HCURSOR hCursor = nullptr;

		// Window title.
		LPCWSTR title = nullptr;
		// Window size.
		// Prefer using DPI-corrected values, like: dpi::sz(500, 300).
		SIZE size = {.cx = 500, .cy = 300};
		// Window style.
		// Defaults to: WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN | WS_BORDER | WS_VISIBLE | WS_MINIMIZEBOX.
		// For a resizable window, add: WS_SIZEBOX | WS_MAXIMIZEBOX.
		DWORD style =  WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN | WS_BORDER | WS_VISIBLE | WS_MINIMIZEBOX;
		// Window extended style.
		// Defaults to: WS_EX_LEFT.
		DWORD exStyle = WS_EX_LEFT;
		// Optional window main menu.
		HMENU hMenu = nullptr;
		// Optional window accelerator table.
		HACCEL hAccelTable = nullptr;

		// In most applications, the window loop calls IsDialogMessage() so child control messages will properly work.
		// However, this has the side-effect of inhibiting WM_CHAR messages from being sent, which is bad for applications like text editors.
		// So if your application has no child controls and needs to process WM_CHAR messages, pass false to suppress IsDialogMessage() call.
		bool processDlgMsgs = true;
	};

}

namespace _wl_internal {

	// Main raw window.
	class RawMain final {
	public:
		DEL_COPY_MOVE(RawMain);
		RawMain() = delete;
		explicit RawMain(wl::opts::Main opts);

		[[nodiscard]] constexpr HWND hwnd() const { return _rawBase.hwnd(); }
		int run(HINSTANCE hInst, int cmdShow);

		RawBase _rawBase{};
		wl::opts::Main _opts;
		HWND _hWndChildPrevFocus = nullptr;
	};

}

namespace wl { class WindowParent; }

namespace wl::opts {

	// Options to create a WindowModal programmatically.
	struct Modal {
		// Class name passed to WNDCLASSEX.
		// Defaults to an auto-generated string.
		LPCWSTR className = nullptr;
		// Class style passed to WNDCLASSEX.
		// Defaults to: CS_DBLCLKS.
		DWORD classStyle = CS_DBLCLKS;
		// Optional window icon resource ID.
		WORD iconId = 0;
		// Window background brush.
		HBRUSH hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
		// Window cursor.
		HCURSOR hCursor = nullptr;

		// Window title.
		LPCWSTR title = nullptr;
		// Window size.
		// Prefer using DPI-corrected values, like: dpi::sz(400, 200).
		SIZE size = {.cx = 400, .cy = 200};
		// Window style.
		// Defaults to: WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN | WS_BORDER | WS_VISIBLE.
		DWORD style =  WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN | WS_BORDER | WS_VISIBLE;
		// Window extended style.
		// Defaults to: WS_EX_LEFT.
		DWORD exStyle = WS_EX_LEFT;

		// In most applications, the window loop calls IsDialogMessage() so child control messages will properly work.
		// However, this has the side-effect of inhibiting WM_CHAR messages from being sent, which is bad for applications like text editors.
		// So if your application has no child controls and needs to process WM_CHAR messages, pass false to suppress IsDialogMessage() call.
		bool processDlgMsgs = true;
	};

}

namespace _wl_internal {

	// Modal raw window.
	class RawModal final {
	public:
		DEL_COPY_MOVE(RawModal);
		RawModal() = delete;
		RawModal(wl::WindowParent &parent, wl::opts::Modal opts);

		[[nodiscard]] constexpr HWND hwnd() const { return _rawBase.hwnd(); }
		void show();

		RawBase _rawBase{};
		wl::WindowParent &_parent;
		wl::opts::Modal _opts;
		HWND _hWndChildPrevFocusParent = nullptr;
	};

}
