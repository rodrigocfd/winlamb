#pragma once
#include <type_traits>
#include "lib-include-win.h"
#include "str.h"
#include "wnd-interfaces.h"
#include "wnd-funcs.h"
#include "wnd-opts.h"
#include "events-ctl.h"
#include "icon-store.h"

namespace wl {

	/// @brief Native [button] control.
	///
	/// Example of creating a window with a button programmatically, .h and .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{};
	///     wl::Button btn{wnd};
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
	///     wnd.setup().title = L"My main window";
	///
	///     btn.setup().pos = wl::dpi::pt(10, 10);
	///     btn.setup().text = L"&Click me";
	///
	///     btn.on().bn_clicked([this]() -> void {
	///         MessageBoxW(wnd.hwnd(), L"Button clicked", L"Hello", MB_ICONINFORMATION);
	///     });
	/// }
	/// ```
	///
	/// Example of creating a window with a button from a dialog resource, .h and .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{DLG_MAIN, ICO_MAIN};
	///     wl::Button btn{wnd, BTN_HELLO, wl::Lay::hold_hold};
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
	///     btn.on().bn_clicked([this]() -> void {
	///         MessageBoxW(wnd.hwnd(), L"Button clicked", L"Hello", MB_ICONINFORMATION);
	///     });
	/// }
	/// ```
	///
	/// [button]: https://learn.microsoft.com/en-us/windows/win32/controls/button-types-and-styles#push-buttons
	class Button final : public WindowChild {
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

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

		/** For controls created programmatically, defines additional creation options. */
		[[nodiscard]] constexpr opts::ButtonOpts& setup() { return _wl_internal::valid_opts(hwnd(), _opts); }

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
		[[nodiscard]] constexpr events::ButtonEvents& on() { return _wl_internal::valid_event(hwnd(), _events); }

		/// [Subclasses] the control allowing message events to be added.
		///
		/// The events must be added before the control is created on the screen.
		///
		/// Note that subclassing is a potentially slow technique, prefer using ordinary events.
		///
		/// [Subclasses]: https://learn.microsoft.com/en-us/windows/win32/controls/subclassing-overview
		[[nodiscard]] constexpr events::WindowEvents& subclass_on() { return _wl_internal::valid_event(hwnd(), _ctrl._subclassEvents); }

		/// Calls [`GetWindowText`] to return the control text.
		///
		/// [`GetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowtextw
		[[nodiscard]] std::wstring text() const { return _wl_internal::wnd_text(hwnd()); }

		/// Calls [`SetWindowText`] to set the control text.
		///
		/// [`SetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowtextw
		const Button& set_text(WStrView text) const;

		/// Sends a [`BM_CLICK`] message to fire the button click.
		///
		/// [`BM_CLICK`]: https://learn.microsoft.com/en-us/windows/win32/controls/bm-click
		const Button& trigger_click() const;

	private:
		_wl_internal::NativeCtrlBase _ctrl;
		events::ButtonEvents _events;
		opts::ButtonOpts _opts{};
	};

	/// @brief Native [check box] control.
	///
	/// Example of creating a window with a check box programmatically, .h and .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{};
	///     wl::CheckBox chk{wnd};
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
	///     wnd.setup().title = L"My main window";
	///
	///     chk.setup().pos = wl::dpi::pt(10, 10);
	///     chk.setup().text = L"&Check me";
	///
	///     chk.on().bn_clicked([this]() -> void {
	///         std::wstring title = wl::str::fmt(L"Check box is %s", chk.is_checked() ? L"YES" : L"NO");
	///         wnd.set_title(title);
	///     });
	/// }
	/// ```
	///
	/// [check box]: https://learn.microsoft.com/en-us/windows/win32/controls/button-types-and-styles#check-boxes
	class CheckBox final : public WindowChild {
	public:
		/// Constructs the button programmatically with [`CreateWindowEx`].
		///
		/// The `ctrlId` parameter is optional. If not set, the control will receive an auto-generated ID.
		///
		/// Further options can be defined with the `setup` method.
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit CheckBox(WindowParent &owner, WORD ctrlId = 0);

		/// Constructs the button from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		CheckBox(WindowParent &owner, WORD ctrlId, Lay layout);

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

		/** For controls created programmatically, defines additional creation options. */
		[[nodiscard]] constexpr opts::CheckBoxOpts& setup() { return _wl_internal::valid_opts(hwnd(), _opts); }

		/// Allows message events to be added.
		///
		/// The events must be added before the control is created on the screen.
		///
		/// Example:
		///
		/// ```cpp
		/// chk.on().bn_clicked([]() -> void {
		///     // ...
		/// });
		/// ```
		[[nodiscard]] constexpr events::ButtonEvents& on() { return _wl_internal::valid_event(hwnd(), _events); }

		/// [Subclasses] the control allowing message events to be added.
		///
		/// The events must be added before the control is created on the screen.
		///
		/// Note that subclassing is a potentially slow technique, prefer using ordinary events.
		///
		/// [Subclasses]: https://learn.microsoft.com/en-us/windows/win32/controls/subclassing-overview
		[[nodiscard]] constexpr events::WindowEvents& subclass_on() { return _wl_internal::valid_event(hwnd(), _ctrl._subclassEvents); }

		/// Sends [`BM_GETCHECK`] and returns true if current state flag is `BST_CHECKED`.
		///
		/// [`BM_GETCHECK`]: https://learn.microsoft.com/en-us/windows/win32/controls/bm-getcheck
		[[nodiscard]] bool is_checked() const { return state() == BST_CHECKED; }

		/// Sends [`BM_SETCHECK`] to set the current state flag to `BST_CHECKED` or `BST_UNCHECKED`.
		///
		/// [`BM_SETCHECK`]: https://learn.microsoft.com/en-us/windows/win32/controls/bm-setcheck
		const CheckBox& set_check(bool doCheck) const { set_state(doCheck ? BST_CHECKED : BST_UNCHECKED); }

		/// Sends [`BM_GETCHECK`] to retrieve the current `BST` state flag.
		///
		/// Prefer using `is_checked`, which is simpler.
		///
		/// [`BM_GETCHECK`]: https://learn.microsoft.com/en-us/windows/win32/controls/bm-getcheck
		[[nodiscard]] WORD state() const;

		/// Sends [`BM_SETCHECK`] to set the current `BST` flag.
		///
		/// Example:
		///
		/// ```cpp
		/// chk.set_state(BST_CHECKED);
		/// ```
		///
		/// Prefer using `set_check`, which is simpler.
		///
		/// [`BM_SETCHECK`]: https://learn.microsoft.com/en-us/windows/win32/controls/bm-setcheck
		const CheckBox& set_state(WORD bstFlag) const;

		/// Calls [`GetWindowText`] to return the control text.
		///
		/// [`GetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowtextw
		[[nodiscard]] std::wstring text() const { return _wl_internal::wnd_text(hwnd()); }

		/// Calls [`SetWindowText`] to set the control text.
		///
		/// [`SetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowtextw
		const CheckBox& set_text(WStrView text) const;

		/// Calls [`SetWindowText`] to set the button text, then resizes the check box to fit the text exactly.
		///
		/// [`SetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowtextw
		const CheckBox& set_text_resize(WStrView text) const;

	private:
		_wl_internal::NativeCtrlBase _ctrl;
		events::ButtonEvents _events;
		opts::CheckBoxOpts _opts{};
	};

	/// @brief Native [combo box] control (dropdown).
	///
	/// Example of creating a window with a combo box programmatically, .h and .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{};
	///     wl::ComboBox cmb{wnd};
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
	///     wnd.setup().title = L"My main window";
	///
	///     cmb.setup().pos = wl::dpi::pt(10, 10);
	///     cmb.setup().texts = {L"Hello", L"World"};
	///
	///     cmb.on().cbn_sel_change([this]() -> void {
	///         std::optional<std::wstring> selText = cmb.items.selected_text();
	///         if (selText.has_value()) {
	///             std::wstring title = wl::str::fmt(L"Selected: %s", selText.value().c_str());
	///             wnd.set_title(title);
	///         } else {
	///             wnd.set_title(L"No selection");
	///         }
	///     });
	/// }
	/// ```
	///
	/// [combo box]: https://learn.microsoft.com/en-us/windows/win32/controls/about-combo-boxes
	class ComboBox final : public WindowChild {
	public:
		/** @brief Operations over the items. */
		class ItemCollection final {
		private:
			ItemCollection(ItemCollection&&) = delete; // non-copyable, non-movable

			constexpr explicit ItemCollection(const ComboBox *pOwner) : _pOwner{pOwner} { }

		public:
			/** Returns the item at the given index. */
			[[nodiscard]] std::wstring operator[](int index) const;

			/** Adds a new item. */
			void add(WStrView text) const;

			/** Adds multiple new items. */
			void add(std::initializer_list<WStrView> texts) const;

			/** Returns the item count. */
			[[nodiscard]] size_t count() const;

			/** Removes all items. */
			void delete_all() const;

			/** Selects the item with the given index, or `-1` for none. */
			void select(int index) const;

			/** Returns the zero-based index of the selected item, or `-1` if none. */
			[[nodiscard]] int selected_index() const;

			/** Returns the text of the selected item, if any. */
			[[nodiscard]] std::optional<std::wstring> selected_text() const;

		private:
			const ComboBox *_pOwner;
			friend ComboBox;
		};

		/// Constructs the button programmatically with [`CreateWindowEx`].
		///
		/// The `ctrlId` parameter is optional. If not set, the control will receive an auto-generated ID.
		///
		/// Further options can be defined with the `setup` method.
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit ComboBox(WindowParent &owner, WORD ctrlId = 0);

		/// Constructs the button from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		ComboBox(WindowParent &owner, WORD ctrlId, Lay layout);

		/** Item methods. */
		ItemCollection items{this};

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

		/** For controls created programmatically, defines additional creation options. */
		[[nodiscard]] constexpr opts::ComboBoxOpts& setup() { return _wl_internal::valid_opts(hwnd(), _opts); }

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
		[[nodiscard]] constexpr events::ComboBoxEvents& on() { return _wl_internal::valid_event(hwnd(), _events); }

		/// [Subclasses] the control allowing message events to be added.
		///
		/// The events must be added before the control is created on the screen.
		///
		/// Note that subclassing is a potentially slow technique, prefer using ordinary events.
		///
		/// [Subclasses]: https://learn.microsoft.com/en-us/windows/win32/controls/subclassing-overview
		[[nodiscard]] constexpr events::WindowEvents& subclass_on() { return _wl_internal::valid_event(hwnd(), _ctrl._subclassEvents); }

	private:
		_wl_internal::NativeCtrlBase _ctrl;
		events::ComboBoxEvents _events;
		opts::ComboBoxOpts _opts{};
	};

	/// @brief Native [date and time picker] control.
	///
	/// Example of creating a window with a date and time picker programmatically, .h and .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{};
	///     wl::DateTimePicker dtp{wnd};
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
	///     wnd.setup().title = L"My main window";
	///
	///     dtp.setup().pos = wl::dpi::pt(10, 10);
	///
	///     dtp.on().dtn_date_time_change([this](NMDATETIMECHANGE &p) -> void {
	///         std::wstring title = wl::str::fmt(L"%d-%d-%d", p.st.wYear, p.st.wMonth, p.st.wDay);
	///         wnd.set_title(title);
	///     });
	/// }
	/// ```
	///
	/// [date and time picker]: https://learn.microsoft.com/en-us/windows/win32/controls/date-and-time-picker-controls
	class DateTimePicker final : public WindowChild {
	public:
		/// Constructs the date and time picker programmatically with [`CreateWindowEx`].
		///
		/// The `ctrlId` parameter is optional. If not set, the control will receive an auto-generated ID.
		///
		/// Further options can be defined with the `setup` method.
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit DateTimePicker(WindowParent &owner, WORD ctrlId = 0);

		/// Constructs the date and time picker from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		DateTimePicker(WindowParent &owner, WORD ctrlId, Lay layout);

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

		/** For controls created programmatically, defines additional creation options. */
		[[nodiscard]] constexpr opts::DateTimePickerOpts& setup() { return _wl_internal::valid_opts(hwnd(), _opts); }

		/// Allows message events to be added.
		///
		/// The events must be added before the control is created on the screen.
		///
		/// Example:
		///
		/// ```cpp
		/// dtp.on().dtn_date_time_change([](NMDATETIMECHANGE&) -> void {
		///     // ...
		/// });
		/// ```
		[[nodiscard]] constexpr events::DateTimePickerEvents& on() { return _wl_internal::valid_event(hwnd(), _events); }

		/// [Subclasses] the control allowing message events to be added.
		///
		/// The events must be added before the control is created on the screen.
		///
		/// Note that subclassing is a potentially slow technique, prefer using ordinary events.
		///
		/// [Subclasses]: https://learn.microsoft.com/en-us/windows/win32/controls/subclassing-overview
		[[nodiscard]] constexpr events::WindowEvents& subclass_on() { return _wl_internal::valid_event(hwnd(), _ctrl._subclassEvents); }

		/** Returns the current date and time value. */
		[[nodiscard]] SYSTEMTIME value() const;

		/** Sets the current date and time value. */
		const DateTimePicker& set_value(const SYSTEMTIME &st) const;

	private:
		_wl_internal::NativeCtrlBase _ctrl;
		events::DateTimePickerEvents _events;
		opts::DateTimePickerOpts _opts{};
	};

	/// @brief Native [edit] control (textbox).
	///
	/// Example of creating a window with an edit programmatically, .h and .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{};
	///     wl::Edit txt{wnd};
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
	///     wnd.setup().title = L"My main window";
	///
	///     txt.setup().pos = wl::dpi::pt(10, 10);
	///
	///     txt.on().en_change([this]() -> void {
	///         std::wstring s = txt.text();
	///         wnd.set_title(s);
	///     });
	/// }
	/// ```
	///
	/// Example of creating a window with an edit from a dialog resource, .h and .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{DLG_MAIN, ICO_MAIN};
	///     wl::Edit btn{wnd, TXT_HELLO, wl::Lay::hold_hold};
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
	///     txt.on().en_change([this]() -> void {
	///         std::wstring s = txt.text();
	///         wnd.set_title(s);
	///     });
	/// }
	/// ```
	///
	/// [edit]: https://learn.microsoft.com/en-us/windows/win32/controls/about-edit-controls
	class Edit final : public WindowChild {
	public:
		/// Constructs the edit programmatically with [`CreateWindowEx`].
		///
		/// The `ctrlId` parameter is optional. If not set, the control will receive an auto-generated ID.
		///
		/// Further options can be defined with the `setup` method.
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit Edit(WindowParent &owner, WORD ctrlId = 0);

		/// Constructs the edit from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		Edit(WindowParent &owner, WORD ctrlId, Lay layout);

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

		/** For controls created programmatically, defines additional creation options. */
		[[nodiscard]] constexpr opts::EditOpts& setup() { return _wl_internal::valid_opts(hwnd(), _opts); }

		/// Allows message events to be added.
		///
		/// The events must be added before the control is created on the screen.
		///
		/// Example:
		///
		/// ```cpp
		/// txt.on().en_change([this]() -> void {
		///     // ...
		/// });
		/// ```
		[[nodiscard]] constexpr events::EditEvents& on() { return _wl_internal::valid_event(hwnd(), _events); }

		/// [Subclasses] the control allowing message events to be added.
		///
		/// The events must be added before the control is created on the screen.
		///
		/// Note that subclassing is a potentially slow technique, prefer using ordinary events.
		///
		/// [Subclasses]: https://learn.microsoft.com/en-us/windows/win32/controls/subclassing-overview
		[[nodiscard]] constexpr events::WindowEvents& subclass_on() { return _wl_internal::valid_event(hwnd(), _ctrl._subclassEvents); }

		/// Calls [`GetWindowText`] to return the control text.
		///
		/// [`GetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowtextw
		[[nodiscard]] std::wstring text() const { return _wl_internal::wnd_text(hwnd()); }

		/// Calls [`SetWindowText`] to set the control text.
		///
		/// [`SetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowtextw
		const Edit& set_text(WStrView text) const;

	private:
		_wl_internal::NativeCtrlBase _ctrl;
		events::EditEvents _events;
		opts::EditOpts _opts{};
	};

	/// @brief Native [list view] control.
	///
	/// Example of creating a window with a list view programmatically, .h and .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{};
	///     wl::ListView lv{wnd};
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
	///     wnd.setup().size = wl::dpi::sz(500, 300);
	///
	///     lv.setup().pos = wl::dpi::pt(10, 10);
	///     lv.setup().size = wl::dpi::sz(400, 200);
	///
	///     wnd.on().wm_create([this](wl::wm::Create p) -> int {
	///         lv.set_extended_style(true, LVS_EX_FULLROWSELECT);
	///         return 0;
	///     });
	///
	///     lv.on().lvn_item_changed([this](NMLISTVIEW &p) -> void {
	///         UINT numSel = lv.items.selected_count();
	///     });
	/// }
	/// ```
	///
	/// Example of creating a window with a list view from a dialog resource, .h and .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{DLG_MAIN, ICO_MAIN};
	///     wl::ListView lv{wnd, LST_FILES, wl::Lay::hold_hold};
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
	///     wnd.on().wm_init_dialog([this](wl::wm::InitDialog p) -> bool {
	///         lv.set_extended_style(true, LVS_EX_FULLROWSELECT);
	///         return true;
	///     });
	///
	///     lv.on().lvn_item_changed([this](NMLISTVIEW &p) -> void {
	///         UINT numSel = lv.items.selected_count();
	///     });
	/// }
	/// ```
	///
	/// [list view]: https://learn.microsoft.com/en-us/windows/win32/controls/list-view-controls-overview
	class ListView final : public WindowChild {
	public:
		/** @brief A single column of the `ListView`. */
		class Column final {
		public:
			constexpr Column(const ListView &owner, int index) : _pOwner{&owner}, _index{index} { }

			/** Returns the zero-based index of the column. */
			[[nodiscard]] constexpr int index() const { return _index; }

			/** Returns the texts of all items under this column. */
			[[nodiscard]] std::vector<std::wstring> item_texts() const;

			/** Returns the texts of the selected items under this column. */
			[[nodiscard]] std::vector<std::wstring> selected_item_texts() const;

			/** Returns `HDF_CENTER`, `HDF_LEFT` or `HDF_RIGHT` justification. */
			[[nodiscard]] int justif() const;

			/** Sets `HDF_CENTER`, `HDF_LEFT` or `HDF_RIGHT` justification. */
			const Column& set_justif(WORD hdf) const;

			/** Returns `HDF_SORTUP` (ascending) or `HDF_SORTDOWN` (descending). */
			[[nodiscard]] WORD sort_arrow() const;

			/** Sets `HDF_SORTUP` (ascending) or `HDF_SORTDOWN` (descending). */
			const Column& set_sort_arrow(WORD hdf) const;

			/** Returns the text of the column. */
			[[nodiscard]] std::wstring text() const;

			/** Sets the text of the column. */
			const Column& set_text(WStrView text) const;

			/** Returns the width of the column, in pixels. */
			[[nodiscard]] UINT width() const;

			/// Sets the width of the column, in pixels.
			///
			/// Prefer using DPI-aware values:
			///
			/// ```cpp
			/// lv.cols[0].set_width(wl::dpi::x(100));
			/// ```
			const Column& set_width(UINT width) const;

			/** Stretches the width so that it will fill the remaning space. */
			const Column& set_width_to_fill() const;

		private:
			const ListView *_pOwner;
			int _index;
		};

		/** @brief Operations over the columns. */
		class ColumnCollection final {
		private:
			ColumnCollection(ColumnCollection&&) = delete; // non-copyable, non-movable

			constexpr explicit ColumnCollection(const ListView *pOwner) : _pOwner{pOwner} { }

		public:
			/** Returns the column at the given index. */
			[[nodiscard]] constexpr Column operator[](int index) const { return Column{*_pOwner, index}; }

			/// Adds a new column with the given width.
			///
			/// Example:
			///
			/// ```cpp
			/// lv.cols.add(L"First column", wl::dpi::x(200));
			/// ```
			Column add(WStrView text, UINT width) const;

			/** Returns the column count. */
			[[nodiscard]] size_t count() const;

		private:
			const ListView *_pOwner;
			friend ListView;
		};

		/** @brief A single item of the `ListView`. */
		class Item final {
		public:
			constexpr Item(const ListView &owner, int index) : _pOwner{&owner}, _index{index} { }

			/** Returns the zero-based index of the item. */
			[[nodiscard]] constexpr int index() const { return _index; }

			/** Returns the data associated with the item. */
			template<typename T>
			[[nodiscard]] T data() const {
				if constexpr (std::is_pointer_v<T>) {
					return reinterpret_cast<T>(raw_data());
				} else {
					return static_cast<T>(raw_data());
				}
			}

			/** Sets the data associated with the item. */
			template<typename T>
			const Item& set_data(T value) const {
				if constexpr (std::is_pointer_v<T>) {
					return set_raw_data(reinterpret_cast<LPARAM>(value));
				} else {
					return set_raw_data(static_cast<LPARAM>(value));
				}
				return *this;
			}

			/** Returns true if the item is the currently focused one. Only one item can be focused at a time. */
			[[nodiscard]] bool is_focused() const;

			/** Sets this item as the focused one. Only one item can be focused at a time. */
			const Item& focus() const;

			/** Returns the zero-based index of the `ImageList` icon associated to the item. */
			[[nodiscard]] int icon_index() const;

			/** Sets the zero-based index of the `ImageList` icon associated to the item. */
			const Item& set_icon_index(int iconIndex) const;

			/** Deletes the item from the list view. */
			const Item& remove() const;

			/** Returns true if the item is currently selected. */
			[[nodiscard]] bool is_selected() const;

			/** Selects or deselects the item. */
			const Item& select(bool doSelect) const;

			/** Retrieves the text under a column for the item. */
			[[nodiscard]] std::wstring text(UINT columnIndex = 0) const;

			/** Sets the text under a column for the item. */
			const Item& set_text(WStrView text, UINT columnIndex = 0) const;

			/// Calls [`ListView_MapIndexToID`] to retrieve the unique ID of the item.
			///
			/// Differently from the item index, this ID is guaranteed to remain the same for the whole lifetime of the list view.
			///
			/// [`ListView_MapIndexToID`]: https://learn.microsoft.com/en-us/windows/win32/api/commctrl/nf-commctrl-listview_mapindextoid
			[[nodiscard]] UINT unique_id() const;

			/** Returns true if the item is currently visible in the list view. */
			[[nodiscard]] bool is_visible() const;

		private:
			[[nodiscard]] LPARAM raw_data() const;
			const Item& set_raw_data(LPARAM data) const;
			const ListView *_pOwner;
			int _index;
		};

		/** @brief Operations over the items. */
		class ItemCollection final {
		private:
			ItemCollection(ItemCollection&&) = delete; // non-copyable, non-movable

			constexpr explicit ItemCollection(const ListView *pOwner) : _pOwner{pOwner} { }

		public:
			/** Returns the item at the given index. */
			[[nodiscard]] constexpr Item operator[](int index) const { return Item{*_pOwner, index}; }

			/// Adds a new item, defining the text for the first column.
			/// Optionally, you can provide texts for the subsequent columns.
			///
			/// The optional `iconIndex` refers to the zero-based index of an icon previusly added to one of the image lists.
			///
			/// Example:
			///
			/// ```cpp
			/// lv.items.add(L"My item", {L"Column 2", L"Column 3"});
			/// ```
			Item add(WStrView text, std::initializer_list<WStrView> otherColumnsTexts = {}, int iconIndex = -1) const;

			/** Returns the item count. */
			[[nodiscard]] size_t count() const;

			/** Deletes all items. */
			void delete_all() const;

			/** Deletes the selected items. */
			void delete_selected() const;

			/** Returns the focused item, if any. */
			[[nodiscard]] std::optional<Item> focused() const;

			/// Calls [`ListView_MapIDToIndex`] to return the item with the given unique ID.
			///
			/// Differently from the item index, this ID is guaranteed to remain the same for the whole lifetime of the list view.
			///
			/// [`ListView_MapIDToIndex`]: https://learn.microsoft.com/en-us/windows/win32/api/commctrl/nf-commctrl-listview_mapidtoindex
			[[nodiscard]] std::optional<Item> get_by_unique_id(UINT uid) const;

			/// Calls [`ListView_HitTestEx`] to return the item at the given position, relative to the list view's client area, if any.
			///
			/// [`ListView_HitTestEx`]: https://learn.microsoft.com/en-us/windows/win32/api/commctrl/nf-commctrl-listview_hittestex
			[[nodiscard]] std::optional<Item> hit_test(POINT pt) const;

			/** Selects or deselects all items. */
			void select_all(bool doSelect) const;

			/** Returns the selected items. */
			[[nodiscard]] std::vector<Item> selected() const;

			/** Returns the selected item count. */
			[[nodiscard]] size_t selected_count() const;

			/// Calls [`ListView_SortItemsEx`] to sort the items according to the callback.
			///
			/// Example sorting the items by the text of the 3rd column:
			///
			/// ```cpp
			/// lv.items.sort([](wl::ListView::Item a, wl::ListView::Item b) -> int {
			///     return wl::str::cmp_i(a.text(2), b.text(2));
			/// });
			/// ```
			///
			/// [`ListView_SortItemsEx`]: https://learn.microsoft.com/en-us/windows/win32/api/commctrl/nf-commctrl-listview_sortitemsex
			void sort(std::function<int(Item, Item)> cb) const;

			/// Calls [`ListView_GetTopIndex`] to return the topmost visible item, if any.
			///
			/// [`ListView_GetTopIndex`]: https://learn.microsoft.com/en-us/windows/win32/api/commctrl/nf-commctrl-listview_gettopindex
			[[nodiscard]] std::optional<Item> topmost_visible() const;

		private:
			const ListView *_pOwner;
			friend ListView;
		};

		/// Constructs the list view programmatically with [`CreateWindowEx`].
		///
		/// The `ctrlId` parameter is optional. If not set, the control will receive an auto-generated ID.
		///
		/// Further options can be defined with the `setup` method.
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit ListView(WindowParent &owner, WORD ctrlId = 0);

		/// Constructs the list view from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		ListView(WindowParent &owner, WORD ctrlId, Lay layout, WORD contextMenuId = 0);

		/** Column methods. */
		ColumnCollection cols{this};

		/** Item methods. */
		ItemCollection items{this};

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

		/** For controls created programmatically, defines additional creation options. */
		[[nodiscard]] constexpr opts::ListViewOpts& setup() { return _wl_internal::valid_opts(hwnd(), _opts); }

		/// Allows message events to be added.
		///
		/// The events must be added before the control is created on the screen.
		///
		/// Example:
		///
		/// ```cpp
		/// lv.on().lvn_item_changed([](NMLISTVIEW &p) -> void {
		///     // ...
		/// });
		/// ```
		[[nodiscard]] constexpr events::ListViewEvents& on() { return _wl_internal::valid_event(hwnd(), _events); }

		/// [Subclasses] the control allowing message events to be added.
		///
		/// The events must be added before the control is created on the screen.
		///
		/// Note that subclassing is a potentially slow technique, prefer using ordinary events.
		///
		/// [Subclasses]: https://learn.microsoft.com/en-us/windows/win32/controls/subclassing-overview
		[[nodiscard]] constexpr events::WindowEvents& subclass_on() { return _wl_internal::valid_event(hwnd(), _ctrl._subclassEvents); }

		/// Sets one or more [extended styles].
		///
		/// Example:
		///
		/// ```cpp
		/// lv.set_extended_style(true, LVS_EX_FULLROWSELECT);
		/// ```
		///
		/// [extended styles]: https://learn.microsoft.com/en-us/windows/win32/controls/extended-list-view-styles
		const ListView& set_extended_style(bool doSet, DWORD exStyle) const;

		/** Retrieves the 16x16 `IconStore`. */
		IconStore& icons_16();

		/** Retrieves the 32x32 `IconStore`. */
		IconStore& icons_32();

	private:
		void custom_events();
		void show_context_menu(bool followCursor, bool hasCtrl, bool hasShift);

		_wl_internal::NativeCtrlBase _ctrl;
		events::ListViewEvents _events;
		opts::ListViewOpts _opts{};
		_wl_internal::ImageList _imgList16{{16, 16}}, _imgList32{{32, 32}};
	};

	/// @brief Native [month calendar] control.
	///
	/// [month calendar]: https://learn.microsoft.com/en-us/windows/win32/controls/month-calendar-controls
	class MonthCalendar final : public WindowChild {
	public:
		/// Constructs the month calendar programmatically with [`CreateWindowEx`].
		///
		/// The `ctrlId` parameter is optional. If not set, the control will receive an auto-generated ID.
		///
		/// Further options can be defined with the `setup` method.
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit MonthCalendar(WindowParent &owner, WORD ctrlId = 0);

		/// Constructs the month calendar from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		MonthCalendar(WindowParent &owner, WORD ctrlId, Lay layout);

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

		/** For controls created programmatically, defines additional creation options. */
		[[nodiscard]] constexpr opts::MonthCalendarOpts& setup() { return _wl_internal::valid_opts(hwnd(), _opts); }

		/// Allows message events to be added.
		///
		/// The events must be added before the control is created on the screen.
		///
		/// Example:
		///
		/// ```cpp
		/// mcal.on().mcn_sel_change([](NMSELCHANGE &p) -> void {
		///     // ...
		/// });
		/// ```
		[[nodiscard]] constexpr events::MonthCalendarEvents& on() { return _wl_internal::valid_event(hwnd(), _events); }

		/// [Subclasses] the control allowing message events to be added.
		///
		/// The events must be added before the control is created on the screen.
		///
		/// Note that subclassing is a potentially slow technique, prefer using ordinary events.
		///
		/// [Subclasses]: https://learn.microsoft.com/en-us/windows/win32/controls/subclassing-overview
		[[nodiscard]] constexpr events::WindowEvents& subclass_on() { return _wl_internal::valid_event(hwnd(), _ctrl._subclassEvents); }

		/** Returns the current date and time value. */
		[[nodiscard]] SYSTEMTIME value() const;

		/** Sets the current date and time value. */
		const MonthCalendar& set_value(const SYSTEMTIME &st) const;

	private:
		_wl_internal::NativeCtrlBase _ctrl;
		events::MonthCalendarEvents _events;
		opts::MonthCalendarOpts _opts{};
	};

	/// @brief Native [static] control (label).
	///
	/// Example of creating a window with a static programmatically, .h and .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{};
	///     wl::Static lbl{wnd};
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
	///     wnd.setup().title = L"My main window";
	///
	///     lbl.setup().pos = wl::dpi::pt(10, 10);
	///     lbl.setup().text = L"Hello";
	///
	///     lbl.on().stn_clicked([this]() -> void {
	///         wnd.set_title(L"Label clicked");
	///     });
	/// }
	/// ```
	///
	/// [static]: https://learn.microsoft.com/en-us/windows/win32/controls/about-static-controls
	class Static final : public WindowChild {
	public:
		/// Constructs the static programmatically with [`CreateWindowEx`].
		///
		/// The `ctrlId` parameter is optional. If not set, the control will receive an auto-generated ID.
		///
		/// Further options can be defined with the `setup` method.
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit Static(WindowParent &owner, WORD ctrlId = 0);

		/// Constructs the static from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		Static(WindowParent &owner, WORD ctrlId, Lay layout);

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

		/** For controls created programmatically, defines additional creation options. */
		[[nodiscard]] constexpr opts::StaticOpts& setup() { return _wl_internal::valid_opts(hwnd(), _opts); }

		/// Allows message events to be added.
		///
		/// The events must be added before the control is created on the screen.
		///
		/// Example:
		///
		/// ```cpp
		/// lbl.on().stn_clicked([]() -> void {
		///     // ...
		/// });
		/// ```
		[[nodiscard]] constexpr events::StaticEvents& on() { return _wl_internal::valid_event(hwnd(), _events); }

		/// [Subclasses] the control allowing message events to be added.
		///
		/// The events must be added before the control is created on the screen.
		///
		/// Note that subclassing is a potentially slow technique, prefer using ordinary events.
		///
		/// [Subclasses]: https://learn.microsoft.com/en-us/windows/win32/controls/subclassing-overview
		[[nodiscard]] constexpr events::WindowEvents& subclass_on() { return _wl_internal::valid_event(hwnd(), _ctrl._subclassEvents); }

		/// Calls [`GetWindowText`] to return the control text.
		///
		/// [`GetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowtextw
		[[nodiscard]] std::wstring text() const { return _wl_internal::wnd_text(hwnd()); }

		/// Calls [`SetWindowText`] to set the control text.
		///
		/// [`SetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowtextw
		const Static& set_text(WStrView text) const;

		/// Calls [`SetWindowText`] to set the text, then resizes the control to fit the text exactly.
		///
		/// [`SetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowtextw
		const Static& set_text_resize(WStrView text) const;

	private:
		_wl_internal::NativeCtrlBase _ctrl;
		events::StaticEvents _events;
		opts::StaticOpts _opts{};
	};

	/// @brief Native [status bar] control.
	///
	/// Example of creating a window with a status bar, .h and .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{};
	///     wl::StatusBar sb{wnd};
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
	///     wnd.setup().title = L"My main window";
	///
	///     sb.setup().part_resizable(1, L"First");
	///     sb.setup().part_fixed(wl::dpi::x(200), L"Second");
	///
	///     sb.on().nm_click([this](NMMOUSE& p) -> bool {
	///         MessageBoxW(wnd.hwnd(), L"Status bar clicked", L"Click", MB_ICONINFORMATION);
	///         return true;
	///     });
	/// }
	/// ```
	///
	/// [status bar]: https://learn.microsoft.com/en-us/windows/win32/controls/status-bars
	class StatusBar final : public WindowChild {
	public:
		/** @brief A single part of the `StatusBar`. */
		class Part final {
		public:
			constexpr Part(const StatusBar &owner, int index) : _pOwner{&owner}, _index{index} { }

			/** Returns the index of the part. */
			[[nodiscard]] constexpr int index() const { return _index; }

			/** Returns the text of the part. */
			[[nodiscard]] std::wstring text() const;

			/** Sets the text of the part. */
			const Part& set_text(WStrView text) const;

			/** Returns true is the part has fixed width. */
			[[nodiscard]] constexpr bool is_fixed_width() const { return _pOwner->_partsData[_index].is_fixed_width(); }

			/** Sets the zero-based index of the `ImageList` icon associated to the item. */
			const Part& set_icon_index(int iconIndex) const;

		private:
			const StatusBar *_pOwner;
			int _index;
		};

		/** @brief Operations over the parts. */
		class PartCollection final {
		private:
			PartCollection(PartCollection&&) = delete; // no-copyable, non-movable

			constexpr explicit PartCollection(const StatusBar *pOwner) : _pOwner{pOwner} { }

		public:
			/** Returns the part at the given index. */
			[[nodiscard]] constexpr Part operator[](int index) const { return Part{*_pOwner, index}; }

			/** Returns the number of parts. */
			[[nodiscard]] constexpr size_t count() const { return _pOwner->_partsData.size(); }

		private:
			const StatusBar *_pOwner;
			friend StatusBar;
		};

		/// Constructs the status bar programmatically with [`CreateWindowEx`].
		///
		/// The `ctrlId` parameter is optional. If not set, the control will receive an auto-generated ID.
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit StatusBar(WindowParent &owner, WORD ctrlId = 0);

		/** Part methods. */
		PartCollection parts{this};

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

		/** Defines additional creation options. */
		[[nodiscard]] constexpr opts::StatusBarOpts& setup() { return _wl_internal::valid_opts(hwnd(), _opts); }

		/// Allows message events to be added.
		///
		/// The events must be added before the control is created on the screen.
		///
		/// Example:
		///
		/// ```cpp
		/// sb.on().nm_click([](NMMOUSE &p) -> booo {
		///     // ...
		///     return true;
		/// });
		/// ```
		[[nodiscard]] constexpr events::StatusBarEvents& on() { return _wl_internal::valid_event(hwnd(), _events); }

		/// [Subclasses] the control allowing message events to be added.
		///
		/// The events must be added before the control is created on the screen.
		///
		/// Note that subclassing is a potentially slow technique, prefer using ordinary events.
		///
		/// [Subclasses]: https://learn.microsoft.com/en-us/windows/win32/controls/subclassing-overview
		[[nodiscard]] constexpr events::WindowEvents& subclass_on() { return _wl_internal::valid_event(hwnd(), _ctrl._subclassEvents); }

		/** Retrieves the 16x16 `IconStore`. */
		constexpr IconStore& icons() { return _iconStore16; }

	private:
		void resize_to_fit_parent(wm::Size p);

		_wl_internal::NativeCtrlBase _ctrl;
		events::StatusBarEvents _events;
		opts::StatusBarOpts _opts{};
		_wl_internal::HIconStore _iconStore16{{16, 16}};

		struct PartData final {
			int sizePixels = 0; // one used, the other zero
			int resizeWeight = 0;
			[[nodiscard]] constexpr bool is_fixed_width() const { return resizeWeight == 0; }
		};
		std::vector<PartData> _partsData{};
		std::vector<int> _rightEdges{}; // buffer to speed up resize calls
	};
}
