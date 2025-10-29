#pragma once
#include "window-dialog.h"
#include "window-raw.h"

namespace wl {

	class DropFiles;

	/** @brief Pure abstract class; implemented by any window which can contain child controls. */
	class WindowParent : NonMovable {
	public:
		/** Returns the window handle. */
		[[nodiscard]] virtual HWND hwnd() const = 0;

		/// Calls [`GetWindowText`] to retrieve the window title.
		///
		/// [`GetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowtextw
		[[nodiscard]] virtual std::wstring title() const = 0;

		/// Calls [`SetWindowText`] to set the window title.
		///
		/// [`SetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowtextw
		virtual void set_title(WStrPtr title) const = 0;

		/// Allows message events to be added.
		///
		/// The events must be added before the window is created on the screen.
		[[nodiscard]] virtual events::WindowEvents& on() = 0;

		/// Blocks the current thread and runs `cb` in the UI thread.
		///
		/// Useful if you need to update the window from another thread.
		virtual void ui_thread(std::function<void()> cb) const = 0;

	private:
		[[nodiscard]] virtual const _wl_internal::WindowMsg& wnd_msg() const = 0;
		[[nodiscard]] virtual _wl_internal::WindowMsg& wnd_msg() = 0;
		friend _wl_internal::NativeCtrl; // controls acess wnd_msg() to add creation events
		friend _wl_internal::EventsNativeCtrl; // so events can be added to parent via wnd_msg()
		friend DropFiles;
	};

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
			: _dlgMain{std::make_optional<_wl_internal::DialogMain>(dlgId, iconId, accelTblId)} { }

		/** Returns the window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _rawMain.has_value() ? _rawMain.value().hwnd() : _dlgMain.value().hwnd(); }

		/// Calls [`GetWindowText`] to retrieve the window title.
		///
		/// [`GetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowtextw
		[[nodiscard]] std::wstring title() const override { return wnd_msg()._wnd.text(); }

		/// Calls [`SetWindowText`] to set the window title.
		///
		/// [`SetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowtextw
		void set_title(WStrPtr title) const override { wnd_msg()._wnd.set_text(title); }

		/// Allows message events to be added.
		///
		/// The events must be added before the window is created on the screen.
		[[nodiscard]] events::WindowEvents& on() override { return wnd_msg().on(); }

		/// Blocks the current thread and runs `cb` in the UI thread.
		///
		/// Useful if you need to update the window from another thread.
		void ui_thread(std::function<void()> cb) const override { wnd_msg().ui_thread(cb); }

		/// Runs the main window, blocking the UI thread until the window is closed.
		///
		/// Prefer using the `RUN_MAIN` macro instead of calling this method directly.
		int run(HINSTANCE hInst, int cmdShow);

	private:
		[[nodiscard]] const _wl_internal::WindowMsg& wnd_msg() const override;
		[[nodiscard]] _wl_internal::WindowMsg& wnd_msg() override;

		std::optional<_wl_internal::RawMain> _rawMain{}; // either one
		std::optional<_wl_internal::DialogMain> _dlgMain{};
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
		WindowModal(WindowParent &parent, opts::Modal opts)
			: _rawModal{std::make_optional<_wl_internal::RawModal>(parent, opts)} { }

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
			: _dlgModal{std::make_optional<_wl_internal::DialogModal>(parent, dlgId)} { }

		/** Returns the window handle. */
		[[nodiscard]] constexpr HWND hwnd() const { return _rawModal.has_value() ? _rawModal.value().hwnd() : _dlgModal.value().hwnd(); }

		/// Calls [`GetWindowText`] to retrieve the window title.
		///
		/// [`GetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowtextw
		[[nodiscard]] std::wstring title() const override { return wnd_msg()._wnd.text(); }

		/// Calls [`SetWindowText`] to set the window title.
		///
		/// [`SetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowtextw
		void set_title(WStrPtr title) const override { wnd_msg()._wnd.set_text(title); }

		/// Allows message events to be added.
		///
		/// The events must be added before the window is created on the screen.
		[[nodiscard]] events::WindowEvents& on() override { return wnd_msg().on(); }

		/// Blocks the current thread and runs `cb` in the UI thread.
		///
		/// Useful if you need to update the window from another thread.
		void ui_thread(std::function<void()> cb) const override { wnd_msg().ui_thread(cb); }

		/** Displays the modal window, blocking the UI thread until the window is closed. */
		void show();

	private:
		[[nodiscard]] const _wl_internal::WindowMsg& wnd_msg() const override;
		[[nodiscard]] _wl_internal::WindowMsg& wnd_msg() override;

		std::optional<_wl_internal::RawModal> _rawModal{}; // either one
		std::optional<_wl_internal::DialogModal> _dlgModal{};
	};

}
