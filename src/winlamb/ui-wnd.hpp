#pragma once
#include "ui-rawdlg.hpp"
#include <oleidl.h>

namespace _wl_internal {

	/** Keeps either a raw or a dialog base. */
	template<typename R, typename D>
	struct RawOrDlg final {
		[[nodiscard]] constexpr const WndBase& base() const {
			return raw.has_value()
				? raw.value()._rawBase._wndBase
				: dlg.value()._dlgBase._wndBase;
		}
		[[nodiscard]] constexpr WndBase& base() {
			return raw.has_value()
				? raw.value()._rawBase._wndBase
				: dlg.value()._dlgBase._wndBase;
		}

		std::optional<R> raw{};
		std::optional<D> dlg{};
	};

}

namespace wl {

	class WindowModal; // forward declaration
	class WindowControl;
	class Button;
	class CheckBox;
	class ComboBox;
	class DateTimePicker;
	class Edit;
	class ListView;
	class MonthCalendar;
	class RadioButton;
	class RadioGroup;
	class Static;
	class StatusBar;
	class SysLink;
	class Tab;
	class Trackbar;
	class TreeView;
	class DropFiles;

	/** @brief Pure abstract class; implemented by all windows. */
	class IWindow {
	public:
		/** Returns the window handle. */
		[[nodiscard]] virtual HWND hwnd() const = 0;
	};

	/** @brief Pure abstract class; implemented by all child controls. */
	class IWindowChild : public IWindow {
	public:
		/** Returns the control ID. */
		[[nodiscard]] virtual WORD ctrl_id() const = 0;
	};

	/** @brief Pure abstract class; implemented by all windows which can host child controls. */
	class IWindowParent : public IWindow {
	public:
		/// Allows message events to be added.
		///
		/// The events must be added before the window is created on the screen.
		[[nodiscard]] virtual events::WindowEvents& on() = 0;

		/// Calls [`SendMessage`] to block the current thread, then runs `cb` in
		/// the UI thread. After `cb` finishes, returns back to the calling
		/// thread.
		///
		/// Useful if you are processing something in a parallel thread, then need
		/// to update the UI.
		///
		/// Example using [`std::thread`] to launch a thread:
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
		/// [`std::thread`]: https://en.cppreference.com/w/cpp/thread/thread.html
		virtual void ui_thread(std::function<void()> &&cb) const = 0;

	private:
		[[nodiscard]] virtual const _wl_internal::WndBase& base() const = 0;
		[[nodiscard]] virtual _wl_internal::WndBase& base() = 0;
		friend WindowModal;
		friend WindowControl;
		friend Button;
		friend CheckBox;
		friend ComboBox;
		friend DateTimePicker;
		friend Edit;
		friend ListView;
		friend MonthCalendar;
		friend RadioButton;
		friend Static;
		friend StatusBar;
		friend SysLink;
		friend Tab;
		friend Trackbar;
		friend TreeView;
		friend DropFiles;
	};

	/// @brief Main application window.
	///
	/// Example of creating a window programmatically, .h and .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{wl::MainOpts{
	///         .size = wl::dpi::sz(500, 300),
	///         .style = wl::MainOpts{}.style | WS_SIZEBOX | WS_MAXIMIZEBOX,
	///         .title = L"My main window",
	///     }};
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
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
	/// #define ICO_MAIN 101
	/// #define DLG_MAIN 1000
	///
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
	/// Instead of writing lambdas directly inside your class constructor, you
	/// can alternatively [`std::bind`] methods:
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
	class WindowMain final : public IWindowParent {
	public:
		/// Constructs the main window, which will be created programmatically with [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit WindowMain(MainOpts creationOpts)
			: _rawOrDlg{.raw = std::make_optional<_wl_internal::RawMain>(creationOpts)} { }

		/// Constructs the main window, which will be loaded from a dialog
		/// resource with [`CreateDialogParam`].
		///
		/// Example:
		///
		/// ```cpp
		/// wl::WindowMain wnd{DLG_MAIN, ICO_MAIN, ACC_MAIN};
		/// ```
		///
		/// [`CreateDialogParam`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createdialogparamw
		explicit WindowMain(WORD dlgId, WORD iconId = 0, WORD accelTblId = 0)
			: _rawOrDlg{.dlg = std::make_optional<_wl_internal::DlgMain>(dlgId, iconId, accelTblId)} { }

		/** Returns the window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _rawOrDlg.base()._hWnd; };

		/// Allows message events to be added.
		///
		/// The events must be added before the window is created on the screen.
		[[nodiscard]] constexpr events::WindowEvents& on() override {
			return _wl_internal::valid_event(hwnd(), _rawOrDlg.base()._userEvents);
		};

		/// Calls [`SendMessage`] to block the current thread, then runs `cb` in
		/// the UI thread. After `cb` finishes, returns back to the calling
		/// thread.
		///
		/// Useful if you are processing something in a parallel thread, then need
		/// to update the UI.
		///
		/// Example using [`std::thread`] to launch a thread:
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
		/// [`std::thread`]: https://en.cppreference.com/w/cpp/thread/thread.html
		void ui_thread(std::function<void()> &&cb) const override { _rawOrDlg.base().ui_thread(std::move(cb)); }

		/// Calls [`GetWindowText`] to retrieve the window title.
		///
		/// [`GetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowtextw
		[[nodiscard]] std::wstring title() const { return _wl_internal::wnd_text(hwnd()); }

		/// Calls [`SetWindowText`] to set the window title.
		///
		/// [`SetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowtextw
		const WindowMain& set_title(WStrView newTitle) const;

		/// Runs the main window, blocking the UI thread until the window is
		/// closed.
		///
		/// Prefer using the `RUN_MAIN` macro instead of calling this method
		/// directly.
		int run(HINSTANCE hInst, int cmdShow);

	private:
		[[nodiscard]] const _wl_internal::WndBase& base() const override { return _rawOrDlg.base(); }
		[[nodiscard]] _wl_internal::WndBase& base() override { return _rawOrDlg.base(); }
		_wl_internal::RawOrDlg<_wl_internal::RawMain, _wl_internal::DlgMain> _rawOrDlg{};
	};

	/** @brief Modal window. */
	class WindowModal final : public IWindowParent {
	public:
		/// Constructs the modal window, which will be created programmatically
		/// with [`CreateWindowEx`].
		///
		/// Example:
		///
		/// ```cpp
		/// void show_my_modal(const wl::IWindowParent &wnd) {
		///
		///     wl::WindowModal myModal{wnd, wl::ModalOpts{
		///         .size = wl::dpi::sz(200, 200),
		///         .title = L"My modal",
		///     }};
		///     myModal.show();
		///
		/// }
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		WindowModal(const IWindowParent &parent, ModalOpts creationOpts)
			: _rawOrDlg{.raw = std::make_optional<_wl_internal::RawModal>(parent.base(), creationOpts)} { }

		/// Constructs the modal window, which will be loaded from a dialog
		/// resource with [`DialogBoxParam`].
		///
		/// The `dlgId` parameter must identify a dialog resource.
		///
		/// Example:
		///
		/// ```cpp
		/// void show_my_modal(const wl::IWindowParent &wnd) {
		///     wl::WindowModal myModal{wnd, DLG_MAIN};
		///     myModal.show();
		/// }
		/// ```
		///
		/// [`DialogBoxParam`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-dialogboxparamw
		WindowModal(const IWindowParent &parent, WORD dlgId)
			: _rawOrDlg{.dlg = std::make_optional<_wl_internal::DlgModal>(parent.base(), dlgId)} { }

		/** Returns the window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _rawOrDlg.base()._hWnd; };

		/// Allows message events to be added.
		///
		/// The events must be added before the window is created on the screen.
		[[nodiscard]] constexpr events::WindowEvents& on() override {
			return _wl_internal::valid_event(hwnd(), _rawOrDlg.base()._userEvents);
		};

		/// Calls [`SendMessage`] to block the current thread, then runs `cb` in
		/// the UI thread. After `cb` finishes, returns back to the calling
		/// thread.
		///
		/// Useful if you are processing something in a parallel thread, then need
		/// to update the UI.
		///
		/// Example using [`std::thread`] to launch a thread:
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
		/// [`std::thread`]: https://en.cppreference.com/w/cpp/thread/thread.html
		void ui_thread(std::function<void()> &&cb) const override { _rawOrDlg.base().ui_thread(std::move(cb)); }

		/// Calls [`GetWindowText`] to retrieve the window title.
		///
		/// [`GetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowtextw
		[[nodiscard]] std::wstring title() const { return _wl_internal::wnd_text(hwnd()); }

		/// Calls [`SetWindowText`] to set the window title.
		///
		/// [`SetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowtextw
		const WindowModal& set_title(WStrView newTitle) const;

		/** Displays the modal window, blocking the UI thread until the window is closed. */
		void show();

	private:
		[[nodiscard]] const _wl_internal::WndBase& base() const override { return _rawOrDlg.base(); }
		[[nodiscard]] _wl_internal::WndBase& base() override { return _rawOrDlg.base(); }
		_wl_internal::RawOrDlg<_wl_internal::RawModal, _wl_internal::DlgModal> _rawOrDlg{};
	};

	/// @brief Custom control window.
	///
	/// Example of creating a custom control programmatically, .h and .cpp files:
	///
	/// ```cpp
	/// class MyControl final {
	/// public:
	///     MyControl(wl::IWindowParent &parent, POINT pos, SIZE sz);
	///     wl::WindowControl wnd;
	/// };
	/// ```
	///
	/// ```cpp
	/// MyControl::MyControl(wl::IWindowParent &parent, POINT pos, SIZE sz)
	///     : wnd{parent, wl::ControlOpts{
	///         .layout = wl::Lay::hold_hold,
	///         .pos = pos,
	///         .size = sz,
	///         .styleEx = wl::ControlOpts{}.styleEx | WS_EX_CLIENTEDGE,
	///     }}
	/// {
	///     wnd.on().wm_paint([this]() -> void {
	///         PAINTSTRUCT ps{};
	///         HDC hdc = BeginPaint(wnd.hwnd(), &ps);
	///         LineTo(hdc, 60, 60);
	///         EndPaint(wnd.hwnd(), &ps);
	///     });
	/// }
	/// ```
	class WindowControl final : public IWindowParent, public IWindowChild {
	public:
		/// Constructs the custom control window, which will be created
		/// programmatically with [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		WindowControl(IWindowParent &parent, ControlOpts creationOpts);

		/// Constructs the custom control window, which will be loaded from a
		/// dialog resource with [`CreateDialogParam`].
		///
		/// The `dlgId` parameter must identify a dialog resource.
		///
		/// [`CreateDialogParam`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createdialogparamw
		WindowControl(wl::IWindowParent &parent, WORD dlgId,
			WORD ctrlId = 0, POINT pos = POINT{}, wl::Lay layout = wl::Lay::hold_hold);

		/** Returns the window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _rawOrDlg.base()._hWnd; };

		/** Returns the control ID. */
		[[nodiscard]] WORD ctrl_id() const override;

		/// Allows message events to be added.
		///
		/// The events must be added before the window is created on the screen.
		[[nodiscard]] constexpr events::WindowEvents& on() override {
			return _wl_internal::valid_event(hwnd(), _rawOrDlg.base()._userEvents);
		};

		/// Calls [`SendMessage`] to block the current thread, then runs `cb` in
		/// the UI thread. After `cb` finishes, returns back to the calling
		/// thread.
		///
		/// Useful if you are processing something in a parallel thread, then need
		/// to update the UI.
		///
		/// Example using [`std::thread`] to launch a thread:
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
		/// [`std::thread`]: https://en.cppreference.com/w/cpp/thread/thread.html
		void ui_thread(std::function<void()> &&cb) const override { _rawOrDlg.base().ui_thread(std::move(cb)); }

	private:
		[[nodiscard]] const _wl_internal::WndBase& base() const override { return _rawOrDlg.base(); }
		[[nodiscard]] _wl_internal::WndBase& base() override { return _rawOrDlg.base(); }
		void paint_custom_border(wm::NcPaint p) const;
		_wl_internal::RawOrDlg<_wl_internal::RawControl, _wl_internal::DlgControl> _rawOrDlg{};
	};

	/// @brief Implements [`IDropTarget`] COM interface, allowing file drag &
	/// drop on the window.
	///
	/// Calls [`RegisterDragDrop`] and [`RevokeDragDrop`], and extracts the
	/// dropped files automatically.
	///
	/// Example, .h and .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{DLG_MAIN, ICO_MAIN};
	///     wl::DropFiles dropFiles{wnd};
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
	///     // ...
	///
	///     dropFiles.on_drop([](const std::vector<std::wstring> &files) -> void {
	///         // ...
	///     });
	/// }
	/// ```
	///
	/// [`IDropTarget`]: https://learn.microsoft.com/en-us/windows/win32/api/oleidl/nn-oleidl-idroptarget
	/// [`RegisterDragDrop`]: https://learn.microsoft.com/en-us/windows/win32/api/ole2/nf-ole2-registerdragdrop
	/// [`RevokeDragDrop`]: https://learn.microsoft.com/en-us/windows/win32/api/ole2/nf-ole2-revokedragdrop
	class DropFiles final : public IDropTarget, private wl::NoCopyNoMove {
	public:
		explicit DropFiles(IWindowParent &owner);

		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject) override;
		ULONG STDMETHODCALLTYPE AddRef() override;
		ULONG STDMETHODCALLTYPE Release() override;

		HRESULT STDMETHODCALLTYPE DragEnter(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect) override;
		HRESULT STDMETHODCALLTYPE DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect) override;
		HRESULT STDMETHODCALLTYPE DragLeave() override;
		HRESULT STDMETHODCALLTYPE Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect) override;

		/// Defines a callback to be called when files are dropped on the window.
		///
		/// Receives a [`std::vector`] with the full path of each file being
		/// dropped.
		///
		/// [`std::vector`]: https://en.cppreference.com/w/cpp/container/vector.html
		void on_drop(std::function<void(const std::vector<std::wstring>&)> cb) { _cb = std::make_optional(cb); }

	private:
		std::vector<std::wstring> get_dropped(HANDLE hDrop) const;

		LONG _refCount = 1;
		std::optional<std::function<void(const std::vector<std::wstring>&)>> _cb{};
	};

}

/// @brief Displays various system dialog boxes with [`TaskDialogIndirect`].
///
/// Example:
///
/// ```cpp
/// wl::msg_box::info(wnd, L"Hello"); // simplest usage
///
/// bool ok = wl::msg_box::ask( // OK/Cancel prompt
///     wnd,
///     L"My title",
///     L"My caption",
///     L"My text",
///     L"&OK", // you can have a custom text for the OK button
/// );
/// if (ok) {
///     // user clicked OK
/// }
/// ```
///
/// [`TaskDialogIndirect`]: https://learn.microsoft.com/en-us/windows/win32/api/commctrl/nf-commctrl-taskdialogindirect
namespace wl::msg_box {

	/** @brief Displays an information dialog box. */
	void info(const IWindowParent &parent, WStrView body);

	/** @brief Displays an information dialog box. */
	void info(const IWindowParent &parent, WStrView title, WStrView body);

	/** @brief Displays an information dialog box. */
	void info(const IWindowParent &parent, WStrView title, WStrView caption, WStrView body);

	/** @brief Displays a warning dialog box. */
	void warn(const IWindowParent &parent, WStrView body);

	/** @brief Displays a warning dialog box. */
	void warn(const IWindowParent &parent, WStrView title, WStrView body);

	/** @brief Displays a warning dialog box. */
	void warn(const IWindowParent &parent, WStrView title, WStrView caption, WStrView body);

	/** @brief Displays an error dialog box. */
	void err(const IWindowParent &parent, WStrView body);

	/** @brief Displays an error dialog box. */
	void err(const IWindowParent &parent, WStrView title, WStrView body);

	/** @brief Displays an error dialog box. */
	void err(const IWindowParent &parent, WStrView title, WStrView caption, WStrView body);

	/** @brief Displays a warning dialog box asking for OK/Cancel user confirmation. */
	[[nodiscard]] bool ask(const IWindowParent &parent, WStrView body);

	/** @brief Displays a warning dialog box asking for OK/Cancel user confirmation. */
	[[nodiscard]] bool ask(const IWindowParent &parent, WStrView title, WStrView body);

	/** @brief Displays a warning dialog box asking for OK/Cancel user confirmation. */
	[[nodiscard]] bool ask(const IWindowParent &parent, WStrView title, WStrView caption, WStrView body,
		WStrView okText = L"&OK");

}
