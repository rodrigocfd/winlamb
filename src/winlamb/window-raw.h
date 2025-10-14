#pragma once
#include "window.h"

namespace _wl_internal {

	// Base to all raw container windows.
	class RawBase final {
	public:
		constexpr RawBase() = default;
		RawBase(const RawBase&) = delete;
		RawBase(RawBase&&) = delete;
		RawBase& operator=(const RawBase&) = delete;
		RawBase& operator=(RawBase&&) = delete;

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

namespace wl {

	// Options to create a main window programmatically.
	struct OptsMain {
		LPCWSTR className = nullptr;
		DWORD classStyle = CS_DBLCLKS;
		WORD iconId = 0;
		HBRUSH hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
		HCURSOR hCursor = nullptr;

		LPCWSTR title = nullptr;
		SIZE size = {.cx = 500, .cy = 300}; // you should use dpi::sz()
		DWORD style =  WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN | WS_BORDER | WS_VISIBLE | WS_MINIMIZEBOX;
		DWORD exStyle = WS_EX_LEFT;
		HMENU hMenu = nullptr;
		HACCEL hAccelTable = nullptr;
	};

}

namespace _wl_internal {

	// Main raw window.
	class RawMain final {
	public:
		RawMain() = delete;
		RawMain(const RawMain&) = delete;
		RawMain(RawMain&&) = delete;
		RawMain& operator=(const RawMain&) = delete;
		RawMain& operator=(RawMain&&) = delete;

		explicit RawMain(wl::OptsMain opts);

		[[nodiscard]] constexpr HWND hwnd() const { return _rawBase.hwnd(); }
		int run(HINSTANCE hInst, int cmdShow);

		RawBase _rawBase{};
		wl::OptsMain _opts;
		HWND _hWndChildPrevFocus = nullptr;
	};

}

namespace wl {

	// Options to create a modal window programmatically.
	struct OptsModal {
		LPCWSTR className = nullptr;
		DWORD classStyle = CS_DBLCLKS;
		WORD iconId = 0;
		HBRUSH hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
		HCURSOR hCursor = nullptr;

		LPCWSTR title = nullptr;
		SIZE size = {.cx = 400, .cy = 200}; // you should use dpi::sz()
		DWORD style =  WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN | WS_BORDER | WS_VISIBLE;
		DWORD exStyle = WS_EX_LEFT;
	};

}

namespace _wl_internal {

	// Modal raw window.
	class RawModal final {
	public:
		RawModal() = delete;
		RawModal(const RawModal&) = delete;
		RawModal(RawModal&&) = delete;
		RawModal& operator=(const RawModal&) = delete;
		RawModal& operator=(RawModal&&) = delete;

		explicit RawModal(wl::OptsModal opts);

		[[nodiscard]] constexpr HWND hwnd() const { return _rawBase.hwnd(); }
		void show(const wl::WindowMain &owner);
		void show(const wl::WindowModal &owner);
		void show_modal(HWND hParent);

		RawBase _rawBase{};
		wl::OptsModal _opts;
		HWND _hWndChildPrevFocusParent = nullptr;
	};

}
