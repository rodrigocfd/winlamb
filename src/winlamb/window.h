#pragma once
#include "events.h"
#include "str.h"

namespace _wl_internal { class DialogBase; }
namespace _wl_internal { class NativeCtrl; }

namespace wl {

	// Base to any window.
	class Window final {
	public:
		constexpr Window() = default;
		constexpr Window(const Window&) = default;
		constexpr Window(Window&&) = default;
		constexpr Window& operator=(const Window&) = default;
		constexpr Window& operator=(Window&&) = default;

		constexpr explicit Window(HWND hWnd) : _hWnd{hWnd} { }

		constexpr operator HWND() const { return _hWnd; }

		// Returns the window handle.
		[[nodiscard]] constexpr HWND hwnd() const { return _hWnd; }

		// Calls GetWindowText().
		[[nodiscard]] std::wstring text() const;

		// Calls SetWindowText().
		void set_text(WStrPtr text) const;

	private:
		HWND _hWnd = nullptr;
		friend _wl_internal::DialogBase; // set dialog hWnd
		friend _wl_internal::NativeCtrl; // set control hWnd
	};

}

namespace _wl_internal {

	// Stores the pre, user and post window messages for container windows.
	class WindowMsg final {
	public:
		WindowMsg() = delete;
		WindowMsg(const WindowMsg&) = delete;
		WindowMsg(WindowMsg&&) = delete;
		WindowMsg& operator=(const WindowMsg&) = delete;
		WindowMsg& operator=(WindowMsg&&) = delete;

		constexpr explicit WindowMsg(bool isDlg)
			: _isDlg{isDlg}, _preEvents{isDlg}, _userEvents{isDlg}, _postEvents{isDlg} { }

		[[nodiscard]] constexpr HWND hwnd() const { return _wnd.hwnd(); }
		[[nodiscard]] _wl_internal::Events& on();
		void thread_detach(std::function<void()> cb) const;
		void thread_ui(std::function<void()> cb) const;

		struct ProcResult final {
			bool hasPre, hasPost;
			std::optional<LRESULT> userRet;
		};
		ProcResult process_msgs(UINT msg, WPARAM wp, LPARAM lp);
		int main_loop(HACCEL hAccel);

		bool _isDlg;
		wl::Window _wnd{};
		Events _preEvents, _userEvents, _postEvents;
	};

}

namespace _wl_internal {

	// Base to all native controls.
	class NativeCtrl final {
	public:
		NativeCtrl() = delete;
		NativeCtrl(const NativeCtrl&) = delete;
		NativeCtrl(NativeCtrl&&) = delete;
		NativeCtrl& operator=(const NativeCtrl&) = delete;
		NativeCtrl& operator=(NativeCtrl&&) = delete;

		NativeCtrl(wl::WindowMain& owner);

		[[nodiscard]] constexpr HWND hwnd() const { return _wnd.hwnd(); }
		[[nodiscard]] _wl_internal::Events& subclass_on();
		void set_hwnd(HWND hWnd);
		void install_subclass();

		static LRESULT CALLBACK subclass_proc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp,
			UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

		wl::Window _wnd{};
		WindowMsg& _owner;
		Events _subclassEvents{false};
		static UINT_PTR _subclassId;
	};

}
