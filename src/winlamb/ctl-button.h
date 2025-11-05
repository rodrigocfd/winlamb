#pragma once
#include "lib-include-win.h"
#include <CommCtrl.h>
#include "window-user.h"

namespace wl { class Button; }

namespace wl::events {

	/** @brief Native `Button` control message callbacks. */
	class ButtonEvents final {
	private:
		ButtonEvents(ButtonEvents&&) = delete; // non-copyable, non-movable

		ButtonEvents(wl::WindowParent &owner, WORD ctrlId) : _events{owner, ctrlId} { }

	public:
		void bcn_drop_down(std::function<void(NMBCDROPDOWN&)> &&cb);
		void bcn_hot_item_change(std::function<void(NMBCHOTITEM&)> &&cb);
		void bn_clicked(std::function<void()> &&cb);
		void bn_dbl_clk(std::function<void()> &&cb);
		void bn_kill_focus(std::function<void()> &&cb);
		void bn_set_focus(std::function<void()> &&cb);
		void nm_custom_draw(std::function<DWORD(NMCUSTOMDRAW&)> &&cb);

	private:
		_wl_internal::EventsNativeCtrl _events;
		friend wl::Button; // ctor
	};

}

namespace wl::opts {

	/** Options to create a `Button` programmatically. */
	struct ButtonOpts final {
		/// Control text passed to [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		LPCWSTR text = nullptr;
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
		/// lv.setup().size = wl::dpi::sz(88, 26);
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SIZE size = {.cx = 88, .cy = 26};
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
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		WORD ctrlId = 0;
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		Lay layout = Lay::hold_hold;
	};

}

namespace wl {

	/// @brief Native [button] control.
	///
	/// [button]: https://learn.microsoft.com/en-us/windows/win32/controls/button-types-and-styles#push-buttons
	class Button final : public WindowNativeControl {
	public:
		/// Constructs the button programmatically with [`CreateWindowEx`].
		///
		/// The `ctrlId` parameter is optional. If not set, the control will receive an auto-generated ID.
		///
		/// Further options can be defined with the `setup` method.
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit Button(WindowParent &owner, WORD ctrlId = 0);

		/// Constructs the button from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		Button(WindowParent &owner, WORD ctrlId, Lay layout);

		/** For controls created programmatically, defines additional creation options. */
		[[nodiscard]] constexpr opts::ButtonOpts& setup() { return _opts; }

		/// Allows message events to be added.
		///
		/// The events must be added before the control is created on the screen.
		///
		/// Example:
		///
		/// ```cpp
		/// btn.on().bn_clicked([]() -> void {
		///     // ...
		/// });
		/// ```
		[[nodiscard]] events::ButtonEvents& on() { return _wl_internal::valid_event(hwnd(), _events); }

		/// [Subclasses] the control allowing message events to be added.
		///
		/// The events must be added before the control is created on the screen.
		///
		/// Note that subclassing is a potentially slow technique, prefer using ordinary events.
		///
		/// [Subclasses]: https://learn.microsoft.com/en-us/windows/win32/controls/subclassing-overview
		[[nodiscard]] events::WindowEvents& subclass_on() { return _wl_internal::valid_event(hwnd(), _ctrl._subclassEvents); }

		/// Calls [`GetWindowText`] to return the button text.
		///
		/// [`GetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowtextw
		[[nodiscard]] std::wstring text() const { return _ctrl._wnd.text(); }

		/// Calls [`SetWindowText`] to set the button text.
		///
		/// [`SetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowtextw
		const Button& set_text(WStrPtr text) const;

		/// Sends a [`BM_CLICK`] message to fire the button click.
		///
		/// [`BM_CLICK`]: https://learn.microsoft.com/en-us/windows/win32/controls/bm-click
		const Button& trigger_click() const;

	private:
		[[nodiscard]] constexpr const _wl_internal::NativeCtrl& nat_ctrl() const override          { return _ctrl; }
		[[nodiscard]] constexpr const _wl_internal::EventsNativeCtrl& ev_nat_ctrl() const override { return _events._events; }

		_wl_internal::NativeCtrl _ctrl;
		events::ButtonEvents _events;
		opts::ButtonOpts _opts{};
	};

}
