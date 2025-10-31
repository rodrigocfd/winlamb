#pragma once
#include "window-dlg.h"
#include "window-raw.h"

namespace wl {

	class DropFiles;

	/** @brief Pure abstract class; implemented by any window which can contain child controls. */
	class WindowParent : NonMovable {
	public:
		/** Returns the window handle. */
		[[nodiscard]] constexpr virtual HWND hwnd() const { return wnd_msg().hwnd(); }

		/// Allows message events to be added.
		///
		/// The events must be added before the window is created on the screen.
		[[nodiscard]] virtual events::WindowEvents& on() { return wnd_msg().on(); }

		/// Blocks the current thread and runs `cb` in the UI thread.
		///
		/// Useful if you need to update the window from another thread.
		virtual void ui_thread(std::function<void()> cb) const { return wnd_msg().ui_thread(cb); }

	private:
		[[nodiscard]] virtual const _wl_internal::WindowMsg& wnd_msg() const = 0;
		[[nodiscard]] virtual _wl_internal::WindowMsg& wnd_msg() = 0;

		friend _wl_internal::EventsNativeCtrl; // so events can be added to parent via wnd_msg()
		friend _wl_internal::NativeCtrl; // here and below: so events can be added to parent via wnd_msg()
		friend _wl_internal::RawControl;
		friend _wl_internal::DlgControl;
		friend DropFiles;
	};

}

namespace _wl_internal {

	template<typename R, typename D>
	[[nodiscard]] constexpr const WindowMsg& get_wnd_msg(const std::optional<R> &raw, const std::optional<D> &dlg) {
		return raw.has_value() ? raw.value()._rawBase._wndMsg : dlg.value()._dlgBase._wndMsg;
	}
	template<typename R, typename D>
	[[nodiscard]] constexpr WindowMsg& get_wnd_msg(std::optional<R> &raw, std::optional<D> &dlg) {
		return raw.has_value() ? raw.value()._rawBase._wndMsg : dlg.value()._dlgBase._wndMsg;
	}

}

namespace wl {

	/// @brief Main application window.
	///
	/// Example of creating a window programmatically, .h and .cpp files:
	///
	/// ```cpp
	/// class MyMain final : wl::NonMovable {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd;
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// wl::opts::Main wndOpts{
	///     .title = L"My main window",
	///     .size  = wl::dpi::sz(500, 300),
	///     .style = wl::opts::Main{}.style | WS_SIZEBOX | WS_MAXIMIZEBOX,
	/// };
	///
	/// MyMain::MyMain()
	///     : wnd{wndOpts}
	/// {
	/// }
	/// ```
	///
	/// Example of creating a window from a dialog resource, .h and .cpp files:
	///
	/// ```cpp
	/// class MyMain final : wl::NonMovable {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{DLG_MAIN, ICO_MAIN};
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
	/// }
	/// ```
	class WindowMain final : public WindowParent {
	public:
		/// Constructs the main window programmatically with [`CreateWindowEx`].
		///
		/// Example:
		///
		/// ```cpp
		/// wl::opts::Main wndOpts{
		///     .title = L"My main window",
		///     .size  = wl::dpi::sz(500, 300),
		///     .style = wl::opts::Main{}.style | WS_SIZEBOX | WS_MAXIMIZEBOX,
		/// };
		/// wl::WindowMain wnd{wndOpts};
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		WindowMain(opts::Main options)
			: _rawMain{std::make_optional<_wl_internal::RawMain>(options)} { }

		/// Constructs the main window from a dialog resource with [`CreateDialogParam`].
		///
		/// Example:
		///
		/// ```cpp
		/// wl::WindowMain wnd{DLG_MAIN, ICO_MAIN, ACC_MAIN};
		/// ```
		///
		/// [`CreateDialogParam`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createdialogparamw
		WindowMain(WORD dlgId, WORD iconId = 0, WORD accelTblId = 0)
			: _dlgMain{std::make_optional<_wl_internal::DlgMain>(dlgId, iconId, accelTblId)} { }

		/// Calls [`GetWindowText`] to retrieve the window title.
		///
		/// [`GetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowtextw
		[[nodiscard]] std::wstring title() const { return wnd_msg()._wnd.text(); }

		/// Calls [`SetWindowText`] to set the window title.
		///
		/// [`SetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowtextw
		void set_title(WStrPtr title) const { wnd_msg()._wnd.set_text(title); }

		/// Runs the main window, blocking the UI thread until the window is closed.
		///
		/// Prefer using the `RUN_MAIN` macro instead of calling this method directly.
		int run(HINSTANCE hInst, int cmdShow);

	private:
		[[nodiscard]] constexpr const _wl_internal::WindowMsg& wnd_msg() const override { return get_wnd_msg(_rawMain, _dlgMain); }
		[[nodiscard]] constexpr _wl_internal::WindowMsg& wnd_msg() override             { return get_wnd_msg(_rawMain, _dlgMain); }

		std::optional<_wl_internal::RawMain> _rawMain{}; // either one
		std::optional<_wl_internal::DlgMain> _dlgMain{};
	};

}

namespace wl {

	/** @brief Modal window. */
	class WindowModal final : public WindowParent {
	public:
		/// Constructs the modal window programmatically with [`CreateWindowEx`].
		///
		/// Example:
		///
		/// ```cpp
		/// void show_my_modal(wl::WindowParent &wnd) {
		///     wl::opts::Modal opts{
		///         .title = L"My modal",
		///         .size  = wl::dpi::sz(400, 200),
		///     };
		///     wl::WindowModal myModal{wnd, opts};
		///     myModal.show();
		/// }
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		WindowModal(WindowParent &parent, opts::Modal options)
			: _rawModal{std::make_optional<_wl_internal::RawModal>(parent, options)} { }

		/// Constructs the modal window from a dialog resource with [`DialogBoxParam`].
		///
		/// Example:
		///
		/// ```cpp
		/// void show_my_modal(wl::WindowParent &wnd) {
		///     wl::WindowModal myModal{wnd, DLG_MAIN};
		///     myModal.show();
		/// }
		/// ```
		///
		/// [`DialogBoxParam`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-dialogboxparamw
		WindowModal(const WindowParent &parent, WORD dlgId)
			: _dlgModal{std::make_optional<_wl_internal::DlgModal>(parent, dlgId)} { }

		/// Calls [`GetWindowText`] to retrieve the window title.
		///
		/// [`GetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowtextw
		[[nodiscard]] std::wstring title() const { return wnd_msg()._wnd.text(); }

		/// Calls [`SetWindowText`] to set the window title.
		///
		/// [`SetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowtextw
		void set_title(WStrPtr title) const { wnd_msg()._wnd.set_text(title); }

		/** Displays the modal window, blocking the UI thread until the window is closed. */
		void show();

	private:
		[[nodiscard]] constexpr const _wl_internal::WindowMsg& wnd_msg() const override { return get_wnd_msg(_rawModal, _dlgModal); }
		[[nodiscard]] constexpr _wl_internal::WindowMsg& wnd_msg() override             { return get_wnd_msg(_rawModal, _dlgModal); }

		std::optional<_wl_internal::RawModal> _rawModal{}; // either one
		std::optional<_wl_internal::DlgModal> _dlgModal{};
	};

}

namespace wl {

	/** @brief Custom control window. */
	class WindowControl final : public WindowParent {
	public:
		/// Constructs the custom control window programmatically with [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		WindowControl(WindowParent &parent, opts::Control options)
			: _rawControl{std::make_optional<_wl_internal::RawControl>(parent, options)} { }

		/// Constructs the custom control window from a dialog resource with [`CreateDialogParam`].
		///
		/// [`CreateDialogParam`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createdialogparamw
		WindowControl(wl::WindowParent &parent, WORD dlgId, WORD ctrlId, POINT pos, wl::Lay layout = wl::Lay::none_none)
			: _dlgControl{std::make_optional<_wl_internal::DlgControl>(parent, dlgId, ctrlId, pos, layout)} { }

	private:
		[[nodiscard]] constexpr const _wl_internal::WindowMsg& wnd_msg() const override { return get_wnd_msg(_rawControl, _dlgControl); }
		[[nodiscard]] constexpr _wl_internal::WindowMsg& wnd_msg() override             { return get_wnd_msg(_rawControl, _dlgControl); }

		std::optional<_wl_internal::RawControl> _rawControl{}; // either one
		std::optional<_wl_internal::DlgControl> _dlgControl{};
	};

}
