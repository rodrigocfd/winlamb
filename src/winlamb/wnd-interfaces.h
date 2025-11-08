#pragma once
#include <functional>
#include "lib-include-win.h"

namespace _wl_internal {
	class WndBase;
	class RawControl;
	class DlgControl;
	class NativeCtrlBase;
	class NativeCtrlEvents;
}
namespace wl {
	class DropFiles;
}
namespace wl::events {
	class WindowEvents;
}

namespace wl {

	/** @brief Pure abstract class; implemented by all windows. */
	class Window {
	public:
		/** Returns the window handle. */
		[[nodiscard]] virtual HWND hwnd() const = 0;
	};

	/** @brief Pure abstract class; implemented by all child controls. */
	class WindowChild : public Window {
	public:
		/** Returns the control ID. */
		[[nodiscard]] virtual WORD ctrl_id() const = 0;
	};

	/** @brief Pure abstract class; implemented by all windows which can host child controls. */
	class WindowParent : public Window {
	public:
		/// Allows message events to be added.
		///
		/// The events must be added before the window is created on the screen.
		[[nodiscard]] virtual events::WindowEvents& on() = 0;

		/// Calls [`SendMessage`] to block the current thread, then runs `cb` in the UI thread.
		/// After `cb` finishes, returns back to the calling thread.
		///
		/// Useful if you are processing something in a parallel thread, but need to update the UI.
		///
		/// Example:
		///
		/// ```cpp
		/// std::thread([this]() {
		///
		///     Sleep(5000); // do some parallel process
		///
		///     wnd.ui_thread([this]() {
		///         wnd.set_title(L"First part complete"); // update UI
		///     });
		///
		///     Sleep(5000); // do more parallel process
		///
		///     wnd.ui_thread([this]() {
		///         wnd.set_title(L"Process finished"); // update UI again
		///     });
		///
		/// }).detach();
		/// ```
		///
		/// [`SendMessage`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-sendmessagew
		virtual void ui_thread(std::function<void()> &&cb) const = 0;

	private:
		[[nodiscard]] virtual const _wl_internal::WndBase& wnd_base() const = 0;
		[[nodiscard]] virtual _wl_internal::WndBase& wnd_base() = 0;
		friend _wl_internal::RawControl;
		friend _wl_internal::DlgControl;
		friend _wl_internal::NativeCtrlBase;
		friend _wl_internal::NativeCtrlEvents;
		friend DropFiles;
	};

}
