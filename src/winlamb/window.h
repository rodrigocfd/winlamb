#pragma once
#include "layout.h"
#include "str.h"

namespace _wl_internal {
	class RawBase;
	class DlgBase;
	class NativeCtrl;
}

namespace wl {

	/// @brief Base to any window.
	///
	/// Simply wraps a window handle.
	class Window final {
	public:
		/// Default constructor.
		///
		/// Initializes the wrapped window handle to `nullptr`.
		constexpr Window() = default;

		/** Constructs `Window` by wrapping the given window handle. */
		constexpr explicit Window(HWND hWnd) : _hWnd{hWnd} { }

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const { return _hWnd; }

		/// Calls [`GetWindowText`].
		///
		/// [`GetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowtextw
		[[nodiscard]] std::wstring text() const;

		/// Calls [`SetWindowText`].
		///
		/// [`SetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowtextw
		void set_text(WStrPtr text) const;

	private:
		HWND _hWnd = nullptr;
		friend _wl_internal::RawBase; // set raw hWnd
		friend _wl_internal::DlgBase; // set dialog hDlg
		friend _wl_internal::NativeCtrl; // set control hWnd
	};

}

namespace _wl_internal {

	/// Stores the pre, user and post window messages for container windows.
	/// Exposes exception-safe multi-threading operations.
	class WindowMsg final : wl::NonMovable {
	public:
		constexpr explicit WindowMsg(bool isDlg)
			: _isDlg{isDlg}, _preEvents{isDlg}, _userEvents{isDlg}, _postEvents{isDlg} { }

		[[nodiscard]] constexpr HWND hwnd() const { return _wnd.hwnd(); }
		[[nodiscard]] wl::events::WindowEvents& on();
		void ui_thread(std::function<void()> cb) const;

		struct ProcResult final {
			bool hasPre, hasPost;
			std::optional<LRESULT> userRet;
		};
		ProcResult process_msgs(UINT msg, WPARAM wp, LPARAM lp);
		int main_loop(HACCEL hAccel, bool processDlgMsgs);
		void modal_loop(bool processDlgMsgs);

		bool _isDlg;
		wl::Window _wnd{}; // _hWnd member is set during window/control creation
		Layout _layout{};
		EventsInternal _preEvents;
		wl::events::WindowEvents _userEvents;
		EventsInternal _postEvents;
	};

}

namespace wl { class WindowParent; }

namespace _wl_internal {

	/** Base to all native controls. */
	class NativeCtrl final : wl::NonMovable {
	public:
		explicit NativeCtrl(wl::WindowParent &owner);

		[[nodiscard]] constexpr HWND hwnd() const { return _wnd.hwnd(); }
		[[nodiscard]] wl::events::WindowEvents& subclass_on();
		void create_wnd(WORD ctrlId, DWORD exStyle, LPCWSTR className,
			LPCWSTR title, DWORD style, POINT pos, SIZE size);
		void assign_dlg(WORD ctrlId);
		void install_subclass();

		static LRESULT CALLBACK subclass_proc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp,
			UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

		wl::Window _wnd{}; // _hWnd member is set during control creation
		WindowMsg &_owner;
		wl::events::WindowEvents _subclassEvents{false};

		static HMENU valid_ctrl_id(WORD ctrlId);
	};

}
