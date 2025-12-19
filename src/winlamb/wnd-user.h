#pragma once
#include <optional>
#include "lib-include-win.h"
#include "str.h"
#include "wnd-interfaces.h"
#include "wnd-base.h"
#include "wnd-app.h"

namespace _wl_internal {

	template<typename R, typename D>
	[[nodiscard]] constexpr const WndBase& get_wnd_base(const std::optional<R> &raw, const std::optional<D> &dlg) {
		return raw.has_value() ? raw.value()._rawBase._wndBase : dlg.value()._dlgBase._wndBase;
	}
	template<typename R, typename D>
	[[nodiscard]] constexpr WndBase& get_wnd_base(std::optional<R> &raw, std::optional<D> &dlg) {
		return raw.has_value() ? raw.value()._rawBase._wndBase : dlg.value()._dlgBase._wndBase;
	}

}

namespace wl {

	/// @brief Main application window.
	///
	/// Example of creating a window programmatically, .h and .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{};
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
	///     wnd.setup().title = L"My main window";
	///     wnd.setup().size = wl::dpi::sz(500, 300);
	///     wnd.setup().style |= WS_SIZEBOX | WS_MAXIMIZEBOX;
	///
	///     wnd.on().wm_create([this](wl::wm::Create p) -> int {
	///         wnd.set_title(L"A new title");
	///         return 0;
	///     });
	/// }
	/// ```
	///
	/// Example of creating a window from a dialog resource, .h and .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
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
	///     wnd.on().wm_init_dialog([this](wl::wm::InitDialog p) -> bool {
	///         wnd.set_title(L"A new title");
	///         return true;
	///     });
	/// }
	/// ```
	///
	/// Instead of writing lambdas directly inside your class constructor, you can alternatively [`std::bind`] methods:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{DLG_MAIN, ICO_MAIN};
	/// private:
	///     bool on_init_dialog(wl::wm::InitDialog p);
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
	///     wnd.on().wm_init_dialog(std::bind(&MyMain::on_init_dialog, this, std::placeholders::_1));
	/// }
	///
	/// bool MyMain::on_init_dialog(wl::wm::InitDialog p) {
	///     wnd.set_title(L"A new title");
	///     return true;
	/// }
	/// ```
	///
	/// [`std::bind`]: https://en.cppreference.com/w/cpp/utility/functional/bind.html
	class WindowMain final : public WindowParent {
	public:
		/// Constructs the main window, which will be created programmatically with [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		WindowMain()
			: _rawMain{std::make_optional<_wl_internal::RawMain>()} { }

		/// Constructs the main window, which will be loaded from a dialog resource with [`CreateDialogParam`].
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

		/** Returns the window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return wnd_base()._hWnd; };

		/// For a window created programmatically, defines additional creation options.
		///
		/// If a dialog window, throws an exception.
		[[nodiscard]] constexpr opts::MainOpts& setup() { return _wl_internal::valid_setup(hwnd(), _rawMain.value()._opts); }

		/// Allows message events to be added.
		///
		/// The events must be added before the window is created on the screen.
		[[nodiscard]] constexpr events::WindowEvents& on() override { return _wl_internal::valid_event(hwnd(), wnd_base()._userEvents); };

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
		void ui_thread(std::function<void()> &&cb) const override { wnd_base().ui_thread(std::move(cb)); }

		/// Calls [`GetWindowText`] to retrieve the window title.
		///
		/// [`GetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowtextw
		[[nodiscard]] std::wstring title() const { return _wl_internal::wnd_text(hwnd()); }

		/// Calls [`SetWindowText`] to set the window title.
		///
		/// [`SetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowtextw
		const WindowMain& set_title(WStrView title) const;

		/// Runs the main window, blocking the UI thread until the window is closed.
		///
		/// Prefer using the `RUN_MAIN` macro instead of calling this method directly.
		int run(HINSTANCE hInst, int cmdShow);

	private:
		[[nodiscard]] constexpr const _wl_internal::WndBase& wnd_base() const override { return get_wnd_base(_rawMain, _dlgMain); }
		[[nodiscard]] constexpr _wl_internal::WndBase& wnd_base() override             { return get_wnd_base(_rawMain, _dlgMain); }

		std::optional<_wl_internal::RawMain> _rawMain{}; // either one
		std::optional<_wl_internal::DlgMain> _dlgMain{};
	};

	/** @brief Modal window. */
	class WindowModal final : public WindowParent {
	public:
		/// Constructs the modal window, which will be created programmatically with [`CreateWindowEx`].
		///
		/// Example:
		///
		/// ```cpp
		/// void show_my_modal(const wl::WindowParent &wnd) {
		///     wl::WindowModal myModal{wnd};
		///     myModal.setup().title = L"My modal title";
		///     myModal.setup().size = wl::dpi::sz(400, 200);
		///     myModal.show();
		/// }
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit WindowModal(const WindowParent &parent)
			: _rawModal{std::make_optional<_wl_internal::RawModal>(parent)} { }

		/// Constructs the modal window, which will be loaded from a dialog resource with [`DialogBoxParam`].
		///
		/// The `dlgId` parameter must identify a dialog resource.
		///
		/// Example:
		///
		/// ```cpp
		/// void show_my_modal(const wl::WindowParent &wnd) {
		///     wl::WindowModal myModal{wnd, DLG_MAIN};
		///     myModal.show();
		/// }
		/// ```
		///
		/// [`DialogBoxParam`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-dialogboxparamw
		WindowModal(const WindowParent &parent, WORD dlgId)
			: _dlgModal{std::make_optional<_wl_internal::DlgModal>(parent, dlgId)} { }

		/** Returns the window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return wnd_base()._hWnd; };

		/// For a window created programmatically, defines additional creation options.
		///
		/// If a dialog window, throws an exception.
		[[nodiscard]] constexpr opts::ModalOpts& setup() { return _wl_internal::valid_setup(hwnd(), _rawModal.value()._opts); }

		/// Allows message events to be added.
		///
		/// The events must be added before the window is created on the screen.
		[[nodiscard]] constexpr events::WindowEvents& on() override { return _wl_internal::valid_event(hwnd(), wnd_base()._userEvents); };

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
		void ui_thread(std::function<void()> &&cb) const override { wnd_base().ui_thread(std::move(cb)); }

		/// Calls [`GetWindowText`] to retrieve the window title.
		///
		/// [`GetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowtextw
		[[nodiscard]] std::wstring title() const { return _wl_internal::wnd_text(hwnd()); }

		/// Calls [`SetWindowText`] to set the window title.
		///
		/// [`SetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowtextw
		const WindowModal& set_title(WStrView title) const;

		/** Displays the modal window, blocking the UI thread until the window is closed. */
		void show();

	private:
		[[nodiscard]] constexpr const _wl_internal::WndBase& wnd_base() const override { return get_wnd_base(_rawModal, _dlgModal); }
		[[nodiscard]] constexpr _wl_internal::WndBase& wnd_base() override             { return get_wnd_base(_rawModal, _dlgModal); }

		std::optional<_wl_internal::RawModal> _rawModal{}; // either one
		std::optional<_wl_internal::DlgModal> _dlgModal{};
	};

	/// @brief Custom control window.
	///
	/// Example of creating a custom control programmatically, .h and .cpp files:
	///
	/// ```cpp
	/// class MyControl final {
	/// public:
	///     MyControl(wl::WindowParent &parent);
	///     wl::WindowControl wnd;
	/// };
	/// ```
	///
	/// ```cpp
	/// MyControl::MyControl(wl::WindowParent &parent)
	///     : wnd{parent}
	/// {
	///     wnd.setup().pos = wl::dpi::pt(420, 10);
	///     wnd.setup().size = wl::dpi::sz(80, 80);
	///     wnd.setup().layout = wl::Lay::move_hold;
	///
	///     wnd.on().wm_paint([this]() -> void {
	///         PAINTSTRUCT ps{};
	///         HDC hdc = BeginPaint(wnd.hwnd(), &ps);
	///         LineTo(hdc, 60, 60);
	///         EndPaint(wnd.hwnd(), &ps);
	///     });
	/// }
	/// ```
	class WindowControl final : public WindowParent, public WindowChild {
	public:
		/// Constructs the custom control window, which will be created programmatically with [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit WindowControl(WindowParent &parent);

		/// Constructs the custom control window, which will be loaded from a dialog resource with [`CreateDialogParam`].
		///
		/// The `dlgId` parameter must identify a dialog resource.
		///
		/// [`CreateDialogParam`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createdialogparamw
		WindowControl(wl::WindowParent &parent, WORD dlgId, WORD ctrlId, POINT pos, wl::Lay layout = wl::Lay::hold_hold);

		/** Returns the window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return wnd_base()._hWnd; };

		/** Returns the control ID. */
		[[nodiscard]] WORD ctrl_id() const override;

		/// For a control created programmatically, defines additional creation options.
		///
		/// If a dialog control, throws an exception.
		[[nodiscard]] constexpr opts::ControlOpts& setup() { return _wl_internal::valid_setup(hwnd(), _rawControl.value()._opts); }

		/// Allows message events to be added.
		///
		/// The events must be added before the window is created on the screen.
		[[nodiscard]] constexpr events::WindowEvents& on() override { return _wl_internal::valid_event(hwnd(), wnd_base()._userEvents); };

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
		void ui_thread(std::function<void()> &&cb) const override { wnd_base().ui_thread(std::move(cb)); }

	private:
		[[nodiscard]] constexpr const _wl_internal::WndBase& wnd_base() const override { return get_wnd_base(_rawControl, _dlgControl); }
		[[nodiscard]] constexpr _wl_internal::WndBase& wnd_base() override             { return get_wnd_base(_rawControl, _dlgControl); }
		void paint_custom_border(wm::NcPaint p) const;

		std::optional<_wl_internal::RawControl> _rawControl{}; // either one
		std::optional<_wl_internal::DlgControl> _dlgControl{};
	};

}
