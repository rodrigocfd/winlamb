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

		/// Blocks the current thread and runs `cb` in the UI thread.
		///
		/// Useful if you need to update the window from another thread.
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
