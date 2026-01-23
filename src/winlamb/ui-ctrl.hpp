#pragma once
#include "ui-wnd.hpp"

namespace _wl_internal {

	/** Base to all native control events. */
	class NativeCtrlEvents final {
	public:
		NativeCtrlEvents(NativeCtrlEvents&&) = delete; // non-copyable, non-movable

		NativeCtrlEvents(WndBase &parentWndBase, WORD ctrlId)
			: _parent{parentWndBase}, _ctrlId{ctrlId} { }

		WndBase &_parent;
		WORD _ctrlId;
	};

}

namespace wl::events {

	/** Native `Button` control events. */
	class ButtonEvents final {
	private:
		ButtonEvents(ButtonEvents&&) = delete; // non-copyable, non-movable

		ButtonEvents(_wl_internal::WndBase &parentWndBase, WORD ctrlId)
			: _ctrlEvents{parentWndBase, ctrlId} { }

	public:
		void bcn_drop_down(std::function<void(NMBCDROPDOWN&)> &&cb);
		void bcn_hot_item_change(std::function<void(NMBCHOTITEM&)> &&cb);
		void bn_clicked(std::function<void()> &&cb);
		void bn_dbl_clk(std::function<void()> &&cb);
		void bn_kill_focus(std::function<void()> &&cb);
		void bn_set_focus(std::function<void()> &&cb);
		void nm_custom_draw(std::function<DWORD(NMCUSTOMDRAW&)> &&cb);

	private:
		_wl_internal::NativeCtrlEvents _ctrlEvents;
		friend wl::Button;
		friend wl::CheckBox;
		friend wl::RadioButton;
	};

	/** Native `ComboBox` control events. */
	class ComboBoxEvents final {
	private:
		ComboBoxEvents(ComboBoxEvents&&) = delete; // non-copyable, non-movable

		ComboBoxEvents(_wl_internal::WndBase &parentWndBase, WORD ctrlId)
			: _ctrlEvents{parentWndBase, ctrlId} { }

	public:
		void cbn_close_up(std::function<void()> &&cb);
		void cbn_dbl_clk(std::function<void()> &&cb);
		void cbn_drop_down(std::function<void()> &&cb);
		void cbn_edit_change(std::function<void()> &&cb);
		void cbn_edit_update(std::function<void()> &&cb);
		void cbn_kill_focus(std::function<void()> &&cb);
		void cbn_sel_change(std::function<void()> &&cb);
		void cbn_sel_end_cancel(std::function<void()> &&cb);
		void cbn_sel_end_ok(std::function<void()> &&cb);
		void cbn_set_focus(std::function<void()> &&cb);

	private:
		_wl_internal::NativeCtrlEvents _ctrlEvents;
		friend wl::ComboBox;
	};

	/** Native `DateTimePicker` control events. */
	class DateTimePickerEvents final {
	private:
		DateTimePickerEvents(DateTimePickerEvents&&) = delete; // non-copyable, non-movable

		DateTimePickerEvents(_wl_internal::WndBase &parentWndBase, WORD ctrlId)
			: _ctrlEvents{parentWndBase, ctrlId} { }

	public:
		void dtn_close_up(std::function<void()> &&cb);
		void dtn_date_time_change(std::function<void(NMDATETIMECHANGE&)> &&cb);
		void dtn_drop_down(std::function<void()> &&cb);
		void dtn_format(std::function<void(NMDATETIMEFORMATW&)> &&cb);
		void dtn_format_query(std::function<void(NMDATETIMEFORMATQUERYW&)> &&cb);
		void dtn_user_string(std::function<void(NMDATETIMESTRINGW&)> &&cb);
		void dtn_wm_key_down(std::function<void(NMDATETIMEWMKEYDOWNW&)> &&cb);
		void nm_kill_focus(std::function<void()> &&cb);
		void nm_set_focus(std::function<void()> &&cb);

	private:
		_wl_internal::NativeCtrlEvents _ctrlEvents;
		friend wl::DateTimePicker;
	};

	/** Native `Edit` control events. */
	class EditEvents final {
	private:
		EditEvents(EditEvents&&) = delete; // non-copyable, non-movable

		EditEvents(_wl_internal::WndBase &parentWndBase, WORD ctrlId)
			: _ctrlEvents{parentWndBase, ctrlId} { }

	public:
		void en_change(std::function<void()> &&cb);
		void en_h_scroll(std::function<void()> &&cb);
		void en_kill_focus(std::function<void()> &&cb);
		void en_max_text(std::function<void()> &&cb);
		void en_set_focus(std::function<void()> &&cb);
		void en_update(std::function<void()> &&cb);
		void en_v_scroll(std::function<void()> &&cb);

	private:
		_wl_internal::NativeCtrlEvents _ctrlEvents;
		friend wl::Edit;
	};

	/** Native `ListView` events. */
	class ListViewEvents final {
	private:
		ListViewEvents(ListViewEvents&&) = delete; // non-copyable, non-movable

		ListViewEvents(_wl_internal::WndBase &parentWndBase, WORD ctrlId)
			: _ctrlEvents{parentWndBase, ctrlId} { }

	public:
		void lvn_begin_drag(std::function<void(NMLISTVIEW&)> &&cb);
		void lvn_begin_label_edit(std::function<bool(NMLVDISPINFOW&)> &&cb);
		void lvn_begin_r_drag(std::function<void(NMLISTVIEW&)> &&cb);
		void lvn_begin_scroll(std::function<void(NMLVSCROLL&)> &&cb);
		void lvn_column_click(std::function<void(NMLISTVIEW&)> &&cb);
		void lvn_column_drop_down(std::function<void(NMLISTVIEW&)> &&cb);
		void lvn_column_overflow_click(std::function<void(NMLISTVIEW&)> &&cb);
		void lvn_delete_all_items(std::function<bool(NMLISTVIEW&)> &&cb);
		void lvn_delete_item(std::function<void(NMLISTVIEW&)> &&cb);
		void lvn_end_label_edit(std::function<bool(NMLVDISPINFOW&)> &&cb);
		void lvn_end_scroll(std::function<void(NMLVSCROLL&)> &&cb);
		void lvn_insert_item(std::function<void(NMLISTVIEW&)> &&cb);
		void lvn_item_activate(std::function<void(NMITEMACTIVATE&)> &&cb);
		void lvn_item_changed(std::function<void(NMLISTVIEW&)> &&cb);
		void lvn_item_changing(std::function<bool(NMLISTVIEW&)> &&cb);
		void lvn_key_down(std::function<void(NMLVKEYDOWN&)> &&cb);
		void nm_click(std::function<void(NMITEMACTIVATE&)> &&cb);
		void nm_custom_draw(std::function<DWORD(NMLVCUSTOMDRAW&)> &&cb);
		void nm_dbl_clk(std::function<void(NMITEMACTIVATE&)> &&cb);
		void nm_kill_focus(std::function<void(NMHDR&)> &&cb);
		void nm_r_click(std::function<void(NMITEMACTIVATE&)> &&cb);
		void nm_r_dbl_clk(std::function<void(NMITEMACTIVATE&)> &&cb);
		void nm_released_capture(std::function<void(NMHDR&)> &&cb);
		void nm_set_focus(std::function<void(NMHDR&)> &&cb);

	private:
		_wl_internal::NativeCtrlEvents _ctrlEvents;
		friend wl::ListView;
	};

	/** Native `MonthCalendar` events. */
	class MonthCalendarEvents final {
	private:
		MonthCalendarEvents(MonthCalendarEvents&&) = delete; // non-copyable, non-movable

		MonthCalendarEvents(_wl_internal::WndBase &parentWndBase, WORD ctrlId)
			: _ctrlEvents{parentWndBase, ctrlId} { }

	public:
		void mcn_get_day_state(std::function<void(NMDAYSTATE&)> &&cb);
		void mcn_sel_change(std::function<void(NMSELCHANGE&)> &&cb);
		void mcn_select(std::function<void(NMSELCHANGE&)> &&cb);
		void mcn_view_change(std::function<void(NMVIEWCHANGE&)> &&cb);
		void nm_released_capture(std::function<void()> &&cb);

	private:
		_wl_internal::NativeCtrlEvents _ctrlEvents;
		friend wl::MonthCalendar;
	};

	/** Native `RadioGroup` events. */
	class RadioGroupEvents final {
	private:
		RadioGroupEvents(RadioGroupEvents&&) = delete; // non-copyable, non-movable

		RadioGroupEvents(wl::RadioGroup &owner) : _ownerGroup{owner} { }

	public:
		void bn_clicked(std::function<void(int)> &&cb);
		void bn_dbl_clk(std::function<void(int)> &&cb);
		void bn_kill_focus(std::function<void(int)> &&cb);
		void bn_set_focus(std::function<void(int)> &&cb);

	private:
		wl::RadioGroup &_ownerGroup;
		friend RadioGroup;
	};

	/** Native `Static` events. */
	class StaticEvents final {
	private:
		StaticEvents(StaticEvents&&) = delete; // non-copyable, non-movable

		StaticEvents(_wl_internal::WndBase &parentWndBase, WORD ctrlId)
			: _ctrlEvents{parentWndBase, ctrlId} { }

	public:
		void stn_clicked(std::function<void()> &&cb);
		void stn_dbl_clk(std::function<void()> &&cb);
		void stn_disable(std::function<void()> &&cb);
		void stn_enable(std::function<void()> &&cb);

	private:
		_wl_internal::NativeCtrlEvents _ctrlEvents;
		friend wl::Static;
	};

	/** Native `StatusBar` events. */
	class StatusBarEvents final {
	private:
		StatusBarEvents(StatusBarEvents&&) = delete; // non-copyable, non-movable

		StatusBarEvents(_wl_internal::WndBase &parentWndBase, WORD ctrlId)
			: _ctrlEvents{parentWndBase, ctrlId} { }

	public:
		void nm_click(std::function<bool(NMMOUSE&)> &&cb);
		void nm_dbl_clk(std::function<bool(NMMOUSE&)> &&cb);
		void nm_r_click(std::function<bool(NMMOUSE&)> &&cb);
		void nm_r_dbl_clk(std::function<bool(NMMOUSE&)> &&cb);
		void sbn_simple_mode_change(std::function<void(NMMOUSE&)> &&cb);

	private:
		_wl_internal::NativeCtrlEvents _ctrlEvents;
		friend wl::StatusBar;
	};

	/** Native `Tab` events. */
	class TabEvents final {
	private:
		TabEvents(TabEvents&&) = delete; // non-copyable, non-movable

		TabEvents(_wl_internal::WndBase &parentWndBase, WORD ctrlId)
			: _ctrlEvents{parentWndBase, ctrlId} { }

	public:
		void tcn_focus_change(std::function<void()> &&cb);
		void tcn_get_object(std::function<void(NMOBJECTNOTIFY&)> &&cb);
		void tcn_key_down(std::function<void(NMTCKEYDOWN&)> &&cb);
		void tcn_sel_change(std::function<void()> &&cb);
		void tcn_sel_changing(std::function<bool()> &&cb);
		void nm_click(std::function<void()> &&cb);
		void nm_dbl_clk(std::function<void()> &&cb);
		void nm_r_click(std::function<void()> &&cb);
		void nm_r_dbl_clk(std::function<void()> &&cb);
		void nm_released_capture(std::function<void()> &&cb);

	private:
		_wl_internal::NativeCtrlEvents _ctrlEvents;
		friend wl::Tab;
	};

	/** Native `TreeView` events. */
	class TreeViewEvents final {
	private:
		TreeViewEvents(TreeViewEvents&&) = delete; // non-copyable, non-movable

		TreeViewEvents(_wl_internal::WndBase &parentWndBase, WORD ctrlId)
			: _ctrlEvents{parentWndBase, ctrlId} { }

	public:
		void tvn_async_draw(std::function<void(NMTVASYNCDRAW&)> &&cb);
		void tvn_begin_drag(std::function<void(NMTREEVIEWW&)> &&cb);
		void tvn_begin_label_edit(std::function<bool(NMTVDISPINFOW&)> &&cb);
		void tvn_begin_r_drag(std::function<void(NMTREEVIEWW&)> &&cb);
		void tvn_delete_item(std::function<void(NMTREEVIEWW&)> &&cb);
		void tvn_end_label_edit(std::function<bool(NMTVDISPINFOW&)> &&cb);
		void tvn_get_disp_info(std::function<void(NMTVDISPINFOW&)> &&cb);
		void tvn_get_info_tip(std::function<void(NMTVGETINFOTIPW&)> &&cb);
		void tvn_item_changed(std::function<void(NMTVITEMCHANGE&)> &&cb);
		void tvn_item_changing(std::function<bool(NMTVITEMCHANGE&)> &&cb);
		void tvn_item_expanded(std::function<void(NMTREEVIEWW&)> &&cb);
		void tvn_item_expanding(std::function<bool(NMTREEVIEWW&)> &&cb);
		void tvn_key_down(std::function<int(NMTVKEYDOWN&)> &&cb);
		void tvn_sel_changed(std::function<void(NMTREEVIEWW&)> &&cb);
		void tvn_sel_changing(std::function<bool(NMTREEVIEWW&)> &&cb);
		void tvn_set_disp_info(std::function<void(NMTVDISPINFOW&)> &&cb);
		void tvn_single_expand(std::function<WORD(NMTREEVIEWW&)> &&cb);
		void nm_click(std::function<int()> &&cb);
		void nm_custom_draw(std::function<DWORD(NMTVCUSTOMDRAW&)> &&cb);
		void nm_dbl_clk(std::function<int()> &&cb);
		void nm_kill_focus(std::function<void()> &&cb);
		void nm_r_click(std::function<int()> &&cb);
		void nm_r_dbl_clk(std::function<int()> &&cb);
		void nm_return(std::function<int()> &&cb);
		void nm_set_cursor(std::function<int(NMMOUSE&)> &&cb);
		void nm_set_focus(std::function<void()> &&cb);

	private:
		_wl_internal::NativeCtrlEvents _ctrlEvents;
		friend wl::TreeView;
	};

}

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
		/// Constructs the button, which will be created programmatically with [`CreateWindowEx`].
		///
		/// The `ctrlId` parameter is optional. If not set, the control will receive an auto-generated ID.
		///
		/// Further options can be defined with the `setup` method.
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit Button(WindowParent &owner, WORD ctrlId = 0);

		/// Constructs the button, which will be loaded from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		Button(WindowParent &owner, WORD ctrlId, Lay layout);

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

		/** For controls created programmatically, defines additional creation options. */
		[[nodiscard]] constexpr opts::ButtonOpts& setup() { return _wl_internal::valid_setup(hwnd(), _opts); }

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
		const Button& set_text(WStrView newText) const;

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
		/// Constructs the check box, which will be created programmatically with [`CreateWindowEx`].
		///
		/// The `ctrlId` parameter is optional. If not set, the control will receive an auto-generated ID.
		///
		/// Further options can be defined with the `setup` method.
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit CheckBox(WindowParent &owner, WORD ctrlId = 0);

		/// Constructs the check box, which will be loaded from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		CheckBox(WindowParent &owner, WORD ctrlId, Lay layout);

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

		/** For controls created programmatically, defines additional creation options. */
		[[nodiscard]] constexpr opts::CheckBoxOpts& setup() { return _wl_internal::valid_setup(hwnd(), _opts); }

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
		const CheckBox& set_text(WStrView newText) const;

		/// Calls [`SetWindowText`] to set the button text, then resizes the check box to fit the text exactly.
		///
		/// [`SetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowtextw
		const CheckBox& set_text_resize(WStrView newText) const;

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
			constexpr explicit ItemCollection(const ComboBox &owner) : _owner{owner} { }

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
			const ComboBox &_owner;
			friend ComboBox;
		};

		/// Constructs the combo box, which will be created programmatically with [`CreateWindowEx`].
		///
		/// The `ctrlId` parameter is optional. If not set, the control will receive an auto-generated ID.
		///
		/// Further options can be defined with the `setup` method.
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit ComboBox(WindowParent &owner, WORD ctrlId = 0);

		/// Constructs the combo box, which will be loaded from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		ComboBox(WindowParent &owner, WORD ctrlId, Lay layout);

		/** Item methods. */
		ItemCollection items{*this};

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

		/** For controls created programmatically, defines additional creation options. */
		[[nodiscard]] constexpr opts::ComboBoxOpts& setup() { return _wl_internal::valid_setup(hwnd(), _opts); }

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
		/// Constructs the date and time picker, which will be created programmatically with [`CreateWindowEx`].
		///
		/// The `ctrlId` parameter is optional. If not set, the control will receive an auto-generated ID.
		///
		/// Further options can be defined with the `setup` method.
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit DateTimePicker(WindowParent &owner, WORD ctrlId = 0);

		/// Constructs the date and time picker, which will be loaded from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		DateTimePicker(WindowParent &owner, WORD ctrlId, Lay layout);

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

		/** For controls created programmatically, defines additional creation options. */
		[[nodiscard]] constexpr opts::DateTimePickerOpts& setup() { return _wl_internal::valid_setup(hwnd(), _opts); }

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

		/// Returns the current [`SYSTEMTIME`].
		///
		/// [`SYSTEMTIME`]: https://learn.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-systemtime
		[[nodiscard]] SYSTEMTIME value() const;

		/// Sets the current [`SYSTEMTIME`].
		///
		/// [`SYSTEMTIME`]: https://learn.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-systemtime
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
		/// Constructs the edit, which will be created programmatically with [`CreateWindowEx`].
		///
		/// The `ctrlId` parameter is optional. If not set, the control will receive an auto-generated ID.
		///
		/// Further options can be defined with the `setup` method.
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit Edit(WindowParent &owner, WORD ctrlId = 0);

		/// Constructs the edit, which will be loaded from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		Edit(WindowParent &owner, WORD ctrlId, Lay layout);

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

		/** For controls created programmatically, defines additional creation options. */
		[[nodiscard]] constexpr opts::EditOpts& setup() { return _wl_internal::valid_setup(hwnd(), _opts); }

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
		const Edit& set_text(WStrView newText) const;

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
			/** Constructs a column for the given `ListView` and zero-based index. */
			constexpr Column(const ListView &owner, int columnIndex) : _owner{owner}, _index{columnIndex} { }

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
			const Column& set_text(WStrView newText) const;

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
			const ListView &_owner;
			int _index;
		};

		/** @brief Operations over the columns. */
		class ColumnCollection final {
		private:
			ColumnCollection(ColumnCollection&&) = delete; // non-copyable, non-movable
			constexpr explicit ColumnCollection(const ListView &owner) : _owner{owner} { }

		public:
			/** Returns the column at the given index. */
			[[nodiscard]] constexpr Column operator[](int index) const { return Column{_owner, index}; }

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
			const ListView &_owner;
			friend ListView;
		};

		/** @brief A single item of the `ListView`. */
		class Item final {
		public:
			/** Constructs an item for the given `ListView` and zero-based index. */
			constexpr Item(const ListView &owner, int itemIndex) : _owner{owner}, _index{itemIndex} { }

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
			const Item& set_text(WStrView newText, UINT columnIndex = 0) const;

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
			const ListView &_owner;
			int _index;
		};

		/** @brief Operations over the items. */
		class ItemCollection final {
		private:
			ItemCollection(ItemCollection&&) = delete; // non-copyable, non-movable
			constexpr explicit ItemCollection(const ListView &owner) : _owner{owner} { }

		public:
			/** Returns the item at the given index. */
			[[nodiscard]] constexpr Item operator[](int index) const { return Item{_owner, index}; }

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

			/// Returns a [`std::vector`] with the the selected items.
			///
			/// [`std::vector`]: https://en.cppreference.com/w/cpp/container/vector.html
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
			const ListView &_owner;
			friend ListView;
		};

		/// Constructs the list view, which will be created programmatically with [`CreateWindowEx`].
		///
		/// The `ctrlId` parameter is optional. If not set, the control will receive an auto-generated ID.
		///
		/// Further options can be defined with the `setup` method.
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit ListView(WindowParent &owner, WORD ctrlId = 0);

		/// Constructs the list view, which will be loaded from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		ListView(WindowParent &owner, WORD ctrlId, Lay layout, WORD contextMenuId = 0);

		/** Column methods. */
		ColumnCollection cols{*this};

		/** Item methods. */
		ItemCollection items{*this};

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

		/** For controls created programmatically, defines additional creation options. */
		[[nodiscard]] constexpr opts::ListViewOpts& setup() { return _wl_internal::valid_setup(hwnd(), _opts); }

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

		/// Retrieves the 16x16 `IconStore`.
		///
		/// Allows icons to be added to the control's image list.
		/// An `Item` can display an icon referring to its zero-based index.
		IconStore& icons_16();

		/// Retrieves the 32x32 `IconStore`.
		///
		/// Allows icons to be added to the control's image list.
		/// An `Item` can display an icon referring to its zero-based index.
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
		/// Constructs the month calendar, which will be created programmatically with [`CreateWindowEx`].
		///
		/// The `ctrlId` parameter is optional. If not set, the control will receive an auto-generated ID.
		///
		/// Further options can be defined with the `setup` method.
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit MonthCalendar(WindowParent &owner, WORD ctrlId = 0);

		/// Constructs the month calendar, which will be loaded from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		MonthCalendar(WindowParent &owner, WORD ctrlId, Lay layout);

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

		/** For controls created programmatically, defines additional creation options. */
		[[nodiscard]] constexpr opts::MonthCalendarOpts& setup() { return _wl_internal::valid_setup(hwnd(), _opts); }

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

		/// Returns the current [`SYSTEMTIME`].
		///
		/// [`SYSTEMTIME`]: https://learn.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-systemtime
		[[nodiscard]] SYSTEMTIME value() const;

		/// Sets the current [`SYSTEMTIME`].
		///
		/// [`SYSTEMTIME`]: https://learn.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-systemtime
		const MonthCalendar& set_value(const SYSTEMTIME &st) const;

	private:
		_wl_internal::NativeCtrlBase _ctrl;
		events::MonthCalendarEvents _events;
		opts::MonthCalendarOpts _opts{};
	};

	/// @brief Native [radio button] control.
	///
	/// Instead of creating the radio buttons directly, prefer using a `RadioGroup`.
	///
	/// [radio button]: https://learn.microsoft.com/en-us/windows/win32/controls/button-types-and-styles#radio-buttons
	class RadioButton final : public WindowChild {
	public:
		/// Constructs the radio button, which will be created programmatically with [`CreateWindowEx`].
		///
		/// The `ctrlId` parameter is optional. If not set, the control will receive an auto-generated ID.
		///
		/// Further options can be defined with the `setup` method.
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit RadioButton(WindowParent &owner, WORD ctrlId = 0);

		/// Constructs the radio button, which will be loaded from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		RadioButton(WindowParent &owner, WORD ctrlId, Lay layout);

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

		/** For controls created programmatically, defines additional creation options. */
		[[nodiscard]] constexpr opts::RadioButtonOpts& setup() { return _wl_internal::valid_setup(hwnd(), _opts); }

		/// Allows message events to be added.
		///
		/// The events must be added before the control is created on the screen.
		///
		/// Example:
		///
		/// ```cpp
		/// rad.on().bn_clicked([]() -> void {
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

		/// Returns true if the radio button is currently selected.
		///
		/// Only one radio button can be selected at once in its group.
		[[nodiscard]] bool is_selected() const;

		/// Selects the radio button.
		///
		/// Only one radio button can be selected at once in its group.
		const RadioButton& select() const;

		/// Calls [`GetWindowText`] to return the control text.
		///
		/// [`GetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowtextw
		[[nodiscard]] std::wstring text() const { return _wl_internal::wnd_text(hwnd()); }

		/// Calls [`SetWindowText`] to set the control text.
		///
		/// [`SetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowtextw
		const RadioButton& set_text(WStrView newText) const;

		/// Calls [`SetWindowText`] to set the button text, then resizes the check box to fit the text exactly.
		///
		/// [`SetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowtextw
		const RadioButton& set_text_resize(WStrView newText) const;

	private:
		_wl_internal::NativeCtrlBase _ctrl;
		events::ButtonEvents _events;
		opts::RadioButtonOpts _opts{};
	};

	/// @brief Manages a group of `RadioButton` controls.
	///
	/// Example of creating a window with 3 radio buttons programmatically, .h and .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{};
	///     wl::RadioGroup rads{wnd, 3}; // create 3 radio buttons
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
	///     wnd.setup().title = L"My main window";
	///
	///     rads.radios[0].setup().pos = wl::dpi::pt(10, 10);
	///     rads.radios[0].setup().text = L"First";
	///     rads.radios[0].setup().selected = true;
	///
	///     rads.radios[1].setup().pos = wl::dpi::pt(10, 30);
	///     rads.radios[1].setup().text = L"Second";
	///
	///     rads.radios[2].setup().pos = wl::dpi::pt(10, 50);
	///     rads.radios[2].setup().text = L"Third";
	///
	///     rads.on().bn_clicked([this](int i) -> void {
	///         MessageBoxW(wnd.hwnd(), L"Radio selected", rads.radios[i].text().c_str(), MB_ICONINFORMATION);
	///     });
	/// }
	/// ```
	class RadioGroup final {
	public:
		/** Accesses the managed `RadioButton` controls. */
		class RadioButtonCollection final {
		private:
			RadioButtonCollection(RadioButtonCollection&&) = delete; // non-copyable, non-movable
			constexpr explicit RadioButtonCollection(RadioGroup &owner) : _owner{owner} { }

		public:
			/** Returns a const reference to the `RadioButton` at the given index. */
			[[nodiscard]] constexpr const RadioButton& operator[](int index) const { return _owner._radios[index]; }

			/** Returns a reference to the `RadioButton` at the given index. */
			[[nodiscard]] constexpr RadioButton& operator[](int index) { return _owner._radios[index]; }

			/** Returns the number of `RadioButton` controls in the group. */
			[[nodiscard]] constexpr size_t count() const { return _owner._radios.size(); }

		private:
			RadioGroup &_owner;
			friend RadioGroup;
		};

		/// Constructs the radio group with the given number of radio buttons, each one created
		/// programmatically with [`CreateWindowEx`], and an auto-generated ID.
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		RadioGroup(WindowParent &owner, size_t numRadios);

		/// Constructs the radio group using each provided ID to programmatically create
		/// a radio button with [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		RadioGroup(WindowParent &owner, std::initializer_list<WORD> ctrlIds);

		/// Constructs the radio group using each provided ID to programmatically create
		/// a radio button which will be loaded from the dialog resource.
		///
		/// The layout will be applied to all radio buttons.
		RadioGroup(WindowParent &owner, Lay layout, std::initializer_list<WORD> ctrlIds);

		/** Radio button methods. */
		RadioButtonCollection radios{*this};

		/// Allows message events to be added.
		///
		/// The events must be added before the control is created on the screen.
		///
		/// Example:
		///
		/// ```cpp
		/// rads.on().bn_clicked([this](int radioIndex) -> void {
		///     // ...
		/// });
		/// ```
		[[nodiscard]] constexpr events::RadioGroupEvents& on() { return _wl_internal::valid_event(_radios[0].hwnd(), _events); }

	private:
		void style_radios();
		WindowParent &_owner;
		_wl_internal::NonMovableArray<RadioButton> _radios;
		events::RadioGroupEvents _events;
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
		/// Constructs the static, which will be created programmatically with [`CreateWindowEx`].
		///
		/// The `ctrlId` parameter is optional. If not set, the control will receive an auto-generated ID.
		///
		/// Further options can be defined with the `setup` method.
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit Static(WindowParent &owner, WORD ctrlId = 0);

		/// Constructs the static, which will be loaded from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		Static(WindowParent &owner, WORD ctrlId, Lay layout);

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

		/** For controls created programmatically, defines additional creation options. */
		[[nodiscard]] constexpr opts::StaticOpts& setup() { return _wl_internal::valid_setup(hwnd(), _opts); }

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
		const Static& set_text(WStrView newText) const;

		/// Calls [`SetWindowText`] to set the text, then resizes the control to fit the text exactly.
		///
		/// [`SetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowtextw
		const Static& set_text_resize(WStrView newText) const;

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
	///     sb.setup().add_resizable_part(1, L"First");
	///     sb.setup().add_fixed_part(wl::dpi::x(200), L"Second");
	///
	///     sb.on().nm_click([this](NMMOUSE &p) -> bool {
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
			/** Constructs a part for the given `StatusBar` and zero-based index. */
			constexpr Part(const StatusBar &owner, int partIndex) : _owner{owner}, _index{partIndex} { }

			/** Returns the index of the part. */
			[[nodiscard]] constexpr int index() const { return _index; }

			/** Returns the text of the part. */
			[[nodiscard]] std::wstring text() const;

			/** Sets the text of the part. */
			const Part& set_text(WStrView newText) const;

			/** Returns true is the part has fixed width. */
			[[nodiscard]] constexpr bool is_fixed_width() const { return _owner._partsData[_index].is_fixed_width(); }

			/** Sets the zero-based index of the `ImageList` icon associated to the item. */
			const Part& set_icon_index(int iconIndex) const;

		private:
			const StatusBar &_owner;
			int _index;
		};

		/** @brief Operations over the parts of a `StatusBar`. */
		class PartCollection final {
		private:
			PartCollection(PartCollection&&) = delete; // no-copyable, non-movable
			constexpr explicit PartCollection(const StatusBar &owner) : _owner{owner} { }

		public:
			/** Returns the part at the given index. */
			[[nodiscard]] constexpr Part operator[](int index) const { return Part{_owner, index}; }

			/** Returns the number of parts. */
			[[nodiscard]] constexpr size_t count() const { return _owner._partsData.size(); }

		private:
			const StatusBar &_owner;
			friend StatusBar;
		};

		/// Constructs the status bar, which will be created programmatically with [`CreateWindowEx`].
		///
		/// The `ctrlId` parameter is optional. If not set, the control will receive an auto-generated ID.
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit StatusBar(WindowParent &owner, WORD ctrlId = 0);

		/** Part methods. */
		PartCollection parts{*this};

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

		/** Defines additional creation options. */
		[[nodiscard]] constexpr opts::StatusBarOpts& setup() { return _wl_internal::valid_setup(hwnd(), _opts); }

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

		/// Retrieves the 16x16 `IconStore`.
		///
		/// Allows icons to be added to the control's image list.
		/// A `Part` can display an icon referring to its zero-based index.
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

	/// @brief Native [tab] control.
	///
	/// [tab]: https://learn.microsoft.com/en-us/windows/win32/controls/tab-controls
	class Tab final : public WindowChild {
	public:
		class ItemCollection; // forward declaration

		/** @brief A single item of the `Tab`. */
		class Item final {
		private:
			constexpr Item(Tab &owner, int itemIndex) : _owner{owner}, _index{itemIndex} { }

		public:
			/** For controls created programmatically, defines additional creation options. */
			[[nodiscard]] constexpr opts::TabItemOpts setup() const {
				return {
					.title = _owner._titles[_index],
					.className = _owner._children[_index].setup().className,
					.hbrBackground = _owner._children[_index].setup().hbrBackground,
					.hCursor = _owner._children[_index].setup().hCursor,
				};
			}

			/** Returns the child `WindowControl` rendered within this tab item. */
			[[nodiscard]] constexpr WindowControl& child() const { return _owner._children[_index]; }

			/** Returns the zero-based index of the item. */
			[[nodiscard]] constexpr int index() const { return _index; }

			/** Selects this item. */
			const Item& select() const;

			/** Returns the text of the item. */
			[[nodiscard]] std::wstring text() const;

			/** Sets the text of the item. */
			const Item& set_text(WStrView newText) const;

		private:
			Tab &_owner;
			int _index;
			friend ItemCollection;
		};

		/** @brief Operations of the items. */
		class ItemCollection final {
		private:
			ItemCollection(ItemCollection&&) = delete; // non-copyable, non-movable
			constexpr explicit ItemCollection(Tab &owner) : _owner{owner} { }

		public:
			/** Returns the column at the given index. */
			[[nodiscard]] constexpr Item operator[](int index) const { return Item{_owner, index}; }

			/** Returns the item count. */
			[[nodiscard]] size_t count() const;

			/** Returns the focused item, if any. */
			[[nodiscard]] std::optional<Item> focused() const;

			/** Returns the selected item, if any. */
			[[nodiscard]] std::optional<Item> selected() const;

		private:
			Tab &_owner;
			friend Tab;
		};

		/// Constructs the tab, which will be created programmatically with [`CreateWindowEx`].
		///
		/// The control will receive an auto-generated ID.
		///
		/// Further options can be defined with the `setup` method.
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		Tab(WindowParent &owner, size_t numItems)
			: Tab{owner, 0, numItems} { }

		/// Constructs the tab, which will be created programmatically with [`CreateWindowEx`].
		///
		/// Further options can be defined with the `setup` method.
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		Tab(WindowParent &owner, WORD ctrlId, size_t numItems);

		/// Constructs the tab, which will be loaded from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		///
		/// The `childrenDlgIds` are the IDs of the dialog resources of each children to be added.
		Tab(WindowParent &owner, WORD ctrlId, Lay layout, std::initializer_list<WORD> childrenDlgIds);

		/** Item methods. */
		ItemCollection items{*this};

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

		/** For controls created programmatically, defines additional creation options. */
		[[nodiscard]] constexpr opts::TabOpts& setup() { return _wl_internal::valid_setup(hwnd(), _opts); }

		/// Allows message events to be added.
		///
		/// The events must be added before the control is created on the screen.
		[[nodiscard]] constexpr events::TabEvents& on() { return _wl_internal::valid_event(hwnd(), _events); }

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
		/// [extended styles]: https://learn.microsoft.com/en-us/windows/win32/controls/tree-view-control-window-extended-styles
		const Tab& set_extended_style(bool doSet, DWORD exStyle) const;

	private:
		void create_tabs() const;
		void display_tab(size_t index) const;
		void custom_events();
		_wl_internal::NativeCtrlBase _ctrl;
		events::TabEvents _events;
		opts::TabOpts _opts{};
		_wl_internal::NonMovableArray<WindowControl> _children;
		std::vector<std::wstring> _titles{};
	};

	/// @brief Native [tree view] control.
	///
	/// [tree view]: https://learn.microsoft.com/en-us/windows/win32/controls/tree-view-controls
	class TreeView final : public WindowChild {
	public:
		/** @brief A single item of the `TreeView`. */
		class Item final {
		public:
			/** Constructs an item for the given `TreeView` and hItem. */
			constexpr Item(const TreeView &owner, HTREEITEM hItem) : _owner{owner}, _hItem{hItem} { }

			/** Returns the item handle. */
			[[nodiscard]] constexpr HTREEITEM hitem() const { return _hItem; }

			/// Adds a new child item, defining its text.
			///
			/// The optional `iconIndex` refers to the zero-based index of an icon previusly added to one of the image lists.
			Item add_child(WStrView itemText, int iconIndex = -1) const;

			/** Returns the child items. */
			[[nodiscard]] std::vector<Item> children() const;

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

			/** Makes sure the item is visible. */
			const Item& ensure_visible() const;

			/** Returns true if the item is currently expanded. */
			[[nodiscard]] bool is_expanded() const;

			/** Expands or collapses the item. */
			const Item& expand(bool doExpand) const;

			/** Returns the zero-based index of the `ImageList` icon associated to the item. */
			[[nodiscard]] int icon_index() const;

			/** Sets the zero-based index of the `ImageList` icon associated to the item. */
			const Item& set_icon_index(int iconIndex) const;

			/** Retrieves the next sibling, if any. */
			[[nodiscard]] Item next_sibling() const;

			/** Retrieves the parent item, if any. */
			[[nodiscard]] Item parent() const;

			/** Retrieves the previous sibling, if any. */
			[[nodiscard]] Item prev_sibling() const;

			/** Deletes the item from the tree view. */
			const Item& remove() const;

			/** Returns the text of the column. */
			[[nodiscard]] std::wstring text() const;

			/** Sets the text of the column. */
			const Item& set_text(WStrView newText) const;

		private:
			[[nodiscard]] LPARAM raw_data() const;
			const Item& set_raw_data(LPARAM data) const;
			const TreeView &_owner;
			HTREEITEM _hItem;
		};

		/** @brief Operations over the items. */
		class ItemCollection final {
		private:
			ItemCollection(ItemCollection&&) = delete; // non-copyable, non-movable
			constexpr explicit ItemCollection(const TreeView &owner) : _owner{owner} { }

		public:
			/** Returns the item with the given handle. */
			[[nodiscard]] constexpr Item operator[](HTREEITEM hItem) const { return Item{_owner, hItem}; }

			/// Adds a new root item, defining its text.
			///
			/// The optional `iconIndex` refers to the zero-based index of an icon previusly added to one of the image lists.
			Item add_root(WStrView text, int iconIndex = -1) const;

			/** Returns the item count. */
			[[nodiscard]] size_t count() const;

			/** Deletes all items. */
			void delete_all() const;

			/** Retrieves the first visible item, if any. */
			[[nodiscard]] Item first_visible() const;

			/** Returns the root items. */
			[[nodiscard]] std::vector<Item> roots() const;

			/** Retrieves the selected item, if any. */
			[[nodiscard]] Item selected() const;

		private:
			const TreeView &_owner;
			friend TreeView;
		};

		/// Constructs the tree view, which will be created programmatically with [`CreateWindowEx`].
		///
		/// The `ctrlId` parameter is optional. If not set, the control will receive an auto-generated ID.
		///
		/// Further options can be defined with the `setup` method.
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit TreeView(WindowParent &owner, WORD ctrlId = 0);

		/// Constructs the tree view, which will be loaded from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		TreeView(WindowParent &owner, WORD ctrlId, Lay layout);

		/** Item methods. */
		ItemCollection items{*this};

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

		/** For controls created programmatically, defines additional creation options. */
		[[nodiscard]] constexpr opts::TreeViewOpts& setup() { return _wl_internal::valid_setup(hwnd(), _opts); }

		/// Allows message events to be added.
		///
		/// The events must be added before the control is created on the screen.
		///
		/// Example:
		///
		/// ```cpp
		/// tv.on().tvn_sel_changed([](NMTREEVIEWW &p) -> void {
		///     // ...
		/// });
		/// ```
		[[nodiscard]] constexpr events::TreeViewEvents& on() { return _wl_internal::valid_event(hwnd(), _events); }

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
		/// tv.set_extended_style(true, TVS_EX_AUTOHSCROLL);
		/// ```
		///
		/// [extended styles]: https://learn.microsoft.com/en-us/windows/win32/controls/tree-view-control-window-extended-styles
		const TreeView& set_extended_style(bool doSet, DWORD exStyle) const;

		/// Retrieves the 16x16 `IconStore`.
		///
		/// Allows icons to be added to the control's image list.
		/// An `Item` can display an icon referring to its zero-based index.
		IconStore& icons_16();

	private:
		_wl_internal::NativeCtrlBase _ctrl;
		events::TreeViewEvents _events;
		opts::TreeViewOpts _opts{};
		_wl_internal::ImageList _imgList16{{16, 16}};
	};

}
