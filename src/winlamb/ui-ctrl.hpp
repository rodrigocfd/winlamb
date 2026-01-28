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

	/** Native `Trackbar` events. */
	class TrackbarEvents final {
	private:
		TrackbarEvents(TrackbarEvents&&) = delete; // non-copyable, non-movable

		TrackbarEvents(_wl_internal::WndBase &parentWndBase, WORD ctrlId)
			: _ctrlEvents{parentWndBase, ctrlId} { }

	public:
		void trbn_thumb_pos_changing(std::function<bool(NMTRBTHUMBPOSCHANGING&)> &&cb);
		void wm_h_scroll(std::function<void(wm::HScroll)> &&cb);
		void wm_v_scroll(std::function<void(wm::VScroll)> &&cb);
		void nm_custom_draw(std::function<DWORD(NMCUSTOMDRAW&)> &&cb);
		void nm_released_capture(std::function<void()> &&cb);

	private:
		_wl_internal::NativeCtrlEvents _ctrlEvents;
		friend wl::Trackbar;
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

	/// Options to load an icon, either from resource or from the system.
	///
	/// The fields are declared in alphabetical order to make it easy to work
	/// with [designated initializers], which require the fields to be set
	/// in the same order they appear in the struct.
	///
	/// [designated initializers]: https://en.cppreference.com/w/cpp/language/aggregate_initialization.html#Designated_initializers
	struct IconLoad final {
		/// Resource identifier of the icon.
		///
		/// If specified, the `ext` field is ignored.
		WORD id = 0;
		/// File extension of the icon to be loaded from the system, like "txt".
		///
		/// The icon displayed is the same used by Windows Explorer when listing
		/// the file with the given extension.
		///
		/// If `id` is specified, this field is ignored.
		std::wstring ext{};
	};

	/// Options to create a `Button` programmatically.
	///
	/// The fields are declared in alphabetical order to make it easy to work
	/// with [designated initializers], which require the fields to be set
	/// in the same order they appear in the struct.
	///
	/// [designated initializers]: https://en.cppreference.com/w/cpp/language/aggregate_initialization.html#Designated_initializers
	struct ButtonOpts final {
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		WORD ctrlId = 0;
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		Lay layout = Lay::hold_hold;
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::ButtonOpts myOpts{
		///     .pos = wl::dpi::pt(10, 10),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		POINT pos{};
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::ButtonOpts myOpts{
		///     .size = wl::dpi::sz(88, 26),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SIZE size = {.cx = 88, .cy = 26};
		/// The [window] and [Button style] passed to [`CreateWindowEx`].
		///
		/// [window]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [Button style]: https://learn.microsoft.com/en-us/windows/win32/controls/button-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD style = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE | BS_PUSHBUTTON;
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD styleEx = WS_EX_LEFT;
		/// Control text passed to [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		std::wstring text{};
	};

	/// Options to create a `CheckBox` programmatically.
	///
	/// The fields are declared in alphabetical order to make it easy to work
	/// with [designated initializers], which require the fields to be set
	/// the same order they appear in the struct.
	///
	/// [designated initializers]: https://en.cppreference.com/w/cpp/language/aggregate_initialization.html#Designated_initializers
	struct CheckBoxOpts final {
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		WORD ctrlId = 0;
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		Lay layout = Lay::hold_hold;
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::CheckBoxOpts myOpts{
		///     .pos = wl::dpi::pt(10, 10),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		POINT pos{};
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::CheckBoxOpts myOpts{
		///     .size = wl::dpi::sz(88, 26),
		/// };
		/// ```
		///
		/// If not defined, the control will resize to automatically fit its initial text.
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SIZE size{};
		/** Initial selection state. */
		WORD state = BST_UNCHECKED;
		/// The [window] and [CheckBox style] passed to [`CreateWindowEx`].
		///
		/// [window]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [CheckBox style]: https://learn.microsoft.com/en-us/windows/win32/controls/button-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD style = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE | BS_AUTOCHECKBOX;
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD styleEx = WS_EX_LEFT;
		/// Control text passed to [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		std::wstring text{};
	};

	/// Options to create a `ComboBox` programmatically.
	///
	/// The fields are declared in alphabetical order to make it easy to work
	/// with [designated initializers], which require the fields to be set
	/// the same order they appear in the struct.
	///
	/// [designated initializers]: https://en.cppreference.com/w/cpp/language/aggregate_initialization.html#Designated_initializers
	struct ComboBoxOpts final {
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		WORD ctrlId = 0;
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		Lay layout = Lay::hold_hold;
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::ComboBoxOpts myOpts{
		///     .pos = wl::dpi::pt(10, 10),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		POINT pos{};
		/// The [window] and [ComboBox style] passed to [`CreateWindowEx`].
		///
		/// [window]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [ComboBox style]: https://learn.microsoft.com/en-us/windows/win32/controls/combo-box-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD style = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE | CBS_DROPDOWNLIST;
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD styleEx = WS_EX_LEFT;
		/// Texts to be added.
		///
		/// Example:
		///
		/// ```cpp
		/// wl::ComboBoxOpts myOpts{
		///     .texts = {L"Hello", L"World"},
		/// };
		/// ```
		std::vector<std::wstring> texts{};
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::ComboBoxOpts myOpts{
		///     .width = wl::dpi::x(100),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		int width = 100;
	};

	/// Options to create a `DateTimePicker` programmatically.
	///
	/// The fields are declared in alphabetical order to make it easy to work
	/// with [designated initializers], which require the fields to be set
	/// the same order they appear in the struct.
	///
	/// [designated initializers]: https://en.cppreference.com/w/cpp/language/aggregate_initialization.html#Designated_initializers
	struct DateTimePickerOpts final {
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		WORD ctrlId = 0;
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		Lay layout = Lay::hold_hold;
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::DateTimePickerOpts myOpts{
		///     .pos = wl::dpi::pt(10, 10),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		POINT pos{};
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::DateTimePickerOpts myOpts{
		///     .size = wl::dpi::sz(230, 23),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SIZE size = {.cx = 230, .cy = 23};
		/// The [window] and [DateTimePicker style] passed to [`CreateWindowEx`].
		///
		/// [window]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [DateTimePicker style]: https://learn.microsoft.com/en-us/windows/win32/controls/date-and-time-picker-control-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD style = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE | DTS_LONGDATEFORMAT;
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD styleEx = WS_EX_LEFT | WS_EX_CLIENTEDGE;
		/// Initial [`SYSTEMTIME`].
		///
		/// [`SYSTEMTIME`]: https://learn.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-systemtime
		SYSTEMTIME value{};
	};

	/// Options to create an `Edit` programmatically.
	///
	/// The fields are declared in alphabetical order to make it easy to work
	/// with [designated initializers], which require the fields to be set
	/// the same order they appear in the struct.
	///
	/// [designated initializers]: https://en.cppreference.com/w/cpp/language/aggregate_initialization.html#Designated_initializers
	struct EditOpts final {
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		WORD ctrlId = 0;
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		Lay layout = Lay::hold_hold;
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::EditOpts myOpts{
		///     .pos = wl::dpi::pt(10, 10),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		POINT pos{};
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::EditOpts myOpts{
		///     .size = wl::dpi::sz(100, 23),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SIZE size = {.cx = 100, .cy = 23};
		/// The [window] and [Edit style] passed to [`CreateWindowEx`].
		///
		/// [window]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [Edit style]: https://learn.microsoft.com/en-us/windows/win32/controls/edit-control-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD style = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE | ES_AUTOHSCROLL | ES_NOHIDESEL;
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD styleEx = WS_EX_LEFT | WS_EX_CLIENTEDGE;
		/// Control text passed to [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		std::wstring text{};
	};

	/// Options to create a `ListView` programmatically.
	///
	/// The fields are declared in alphabetical order to make it easy to work
	/// with [designated initializers], which require the fields to be set
	/// the same order they appear in the struct.
	///
	/// [designated initializers]: https://en.cppreference.com/w/cpp/language/aggregate_initialization.html#Designated_initializers
	struct ListViewOpts final {
		/** A column to be added to the `ListView`. */
		struct Col final {
			/** Column header text. */
			std::wstring text{};
			/** Column width. */
			int width = 0;
		};
		/// Columns to be added, title and width.
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::ListViewOpts myOpts{
		///     .cols = {
		///         {L"First", wl::dpi::x(100)},
		///         {L"Second", wl::dpi::x(120)},
		///     },
		/// };
		/// ```
		std::vector<Col> cols{};
		/// Context menu resource to be loaded as the context menu with [`LoadMenu`].
		///
		/// [`LoadMenu`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-loadmenuw
		WORD contextMenuId = 0;
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		WORD ctrlId = 0;
		/// 16x16 icons to be loaded into the `ListView`.
		///
		/// They can be later referenced by index, following the same order they
		/// are added.
		std::vector<IconLoad> icons16{};
		/// 32x32 icons to be loaded into the `ListView`.
		///
		/// They can be later referenced by index, following the same order they
		/// are added.
		std::vector<IconLoad> icons32{};
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		Lay layout = Lay::hold_hold;
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::ListViewOpts myOpts{
		///     .pos = wl::dpi::pt(10, 10),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		POINT pos{};
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::ListViewOpts myOpts{
		///     .size = wl::dpi::sz(120, 120),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SIZE size = {.cx = 120, .cy = 120};
		/// The [window] and [ListView style] passed to [`CreateWindowEx`].
		///
		/// Note that, for safety reasons, `LVS_SHAREIMAGELISTS` will always be set.
		///
		/// [window]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [ListView style]: https://learn.microsoft.com/en-us/windows/win32/controls/list-view-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD style = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE | LVS_REPORT | LVS_NOSORTHEADER | LVS_SHOWSELALWAYS;
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD styleEx = WS_EX_LEFT | WS_EX_CLIENTEDGE;
		/// The [ListView extended styles] applied right after the control is created.
		///
		/// [ListView extended styles]: https://learn.microsoft.com/en-us/windows/win32/controls/extended-list-view-styles
		DWORD styleExListView = LVS_EX_FULLROWSELECT;
	};

	/// Options to create a `MonthCalendar` programmatically.
	///
	/// The fields are declared in alphabetical order to make it easy to work
	/// with [designated initializers], which require the fields to be set
	/// the same order they appear in the struct.
	///
	/// [designated initializers]: https://en.cppreference.com/w/cpp/language/aggregate_initialization.html#Designated_initializers
	struct MonthCalendarOpts final {
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		WORD ctrlId = 0;
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		Lay layout = Lay::hold_hold;
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::MonthCalendarOpts myOpts{
		///     .pos = wl::dpi::pt(10, 10),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		POINT pos{};
		/// The [window] and [MonthCalendar style] passed to [`CreateWindowEx`].
		///
		/// [window]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [MonthCalendar style]: https://learn.microsoft.com/en-us/windows/win32/controls/month-calendar-control-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD style = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE;
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD styleEx = WS_EX_LEFT;
		/// Initial [`SYSTEMTIME`].
		///
		/// [`SYSTEMTIME`]: https://learn.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-systemtime
		SYSTEMTIME value{};
	};

	/// Options to create a `RadioButton` programmatically.
	///
	/// The fields are declared in alphabetical order to make it easy to work
	/// with [designated initializers], which require the fields to be set
	/// the same order they appear in the struct.
	///
	/// [designated initializers]: https://en.cppreference.com/w/cpp/language/aggregate_initialization.html#Designated_initializers
	struct RadioButtonOpts final {
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		WORD ctrlId = 0;
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		Lay layout = Lay::hold_hold;
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::RadioButtonOpts myOpts{
		///     .pos = wl::dpi::pt(10, 10),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		POINT pos{};
		/// Initial state.
		///
		/// Only one radio button can be selected at once in its group.
		bool selected = false;
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::RadioButtonOpts myOpts{
		///     .size = wl::dpi::sz(88, 26),
		/// };
		/// ```
		///
		/// If not defined, the control will resize to automatically fit its initial text.
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SIZE size{};
		/// The [window] and [RadioButton style] passed to [`CreateWindowEx`].
		///
		/// [window]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [RadioButton style]: https://learn.microsoft.com/en-us/windows/win32/controls/button-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD style = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE | BS_AUTORADIOBUTTON;
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD styleEx = WS_EX_LEFT;
		/// Control text passed to [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		std::wstring text{};
	};

	/// Options to create a `Static` programmatically.
	///
	/// The fields are declared in alphabetical order to make it easy to work
	/// with [designated initializers], which require the fields to be set
	/// the same order they appear in the struct.
	///
	/// [designated initializers]: https://en.cppreference.com/w/cpp/language/aggregate_initialization.html#Designated_initializers
	struct StaticOpts final {
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		WORD ctrlId = 0;
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		Lay layout = Lay::hold_hold;
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::StaticOpts myOpts{
		///     .pos = wl::dpi::pt(10, 10),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		POINT pos{};
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::StaticOpts myOpts{
		///     .size = wl::dpi::sz(88, 26),
		/// };
		/// ```
		///
		/// If not defined, the control will resize to automatically fit its initial text.
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SIZE size{};
		/// The [window] and [Static style] passed to [`CreateWindowEx`].
		///
		/// [window]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [Static style]: https://learn.microsoft.com/en-us/windows/win32/controls/static-control-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD style = WS_CHILD | WS_VISIBLE | SS_LEFT | SS_NOTIFY;
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD styleEx = WS_EX_LEFT;
		/// Control text passed to [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		std::wstring text{};
	};

	/// A fixed-width or flexible part to be added to a `StatusBar`.
	///
	/// The fields are declared in alphabetical order to make it easy to work
	/// with [designated initializers], which require the fields to be set
	/// the same order they appear in the struct.
	///
	/// [designated initializers]: https://en.cppreference.com/w/cpp/language/aggregate_initialization.html#Designated_initializers
	struct SbPart final {
		/// Resizing weight for a flexible part, which expands to fill the remaining space.
		///
		/// If `width` is specified, this field is ignored.
		int flex = 0;
		/** Zero-based icon index. */
		int iconIndex = -1;
		/** Text to be rendered. */
		std::wstring text{};
		/// Width in pixels. If defined, this part will have a fixed width and `flex` field will be ignored.
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::SbPart myPart{
		///     .width = wl::dpi::x(200),
		/// };
		/// ```
		int width = 0;
	};

	/// Options to create a `StatusBar` programmatically.
	///
	/// The fields are declared in alphabetical order to make it easy to work
	/// with [designated initializers], which require the fields to be set
	/// the same order they appear in the struct.
	///
	/// [designated initializers]: https://en.cppreference.com/w/cpp/language/aggregate_initialization.html#Designated_initializers
	struct StatusBarOpts final {
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		WORD ctrlId = 0;
		/// Icons to be loaded into the `StatusBar`.
		///
		/// They can be later referenced by index, following the same order they
		/// are added.
		std::vector<IconLoad> icons{};
		/** Fixed-width and flexible parts to be added. */
		std::vector<SbPart> parts{};
	};

	/// Options to create a `Tab` programmatically.
	///
	/// The fields are declared in alphabetical order to make it easy to work
	/// with [designated initializers], which require the fields to be set
	/// the same order they appear in the struct.
	///
	/// [designated initializers]: https://en.cppreference.com/w/cpp/language/aggregate_initialization.html#Designated_initializers
	struct TabOpts final {
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		WORD ctrlId = 0;
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		Lay layout = Lay::hold_hold;
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::TabOpts myOpts{
		///     .pos = wl::dpi::pt(10, 10),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		POINT pos{};
		/** Zero-based index of initially selected tab item. */
		size_t selected = 0;
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::TabOpts myOpts{
		///     .size = wl::dpi::sz(120, 120),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SIZE size = {.cx = 120, .cy = 120};
		/// The [window] and [Tab style] passed to [`CreateWindowEx`].
		///
		/// Note that, for safety reasons, `LVS_SHAREIMAGELISTS` will always be set.
		///
		/// [window]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [Tab style]: https://learn.microsoft.com/en-us/windows/win32/controls/tree-view-control-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD style = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE;
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD styleEx = WS_EX_LEFT;
		/// The [Tab extended styles] applied right after the control is created.
		///
		/// [Tab extended styles]: https://learn.microsoft.com/en-us/windows/win32/controls/tab-control-extended-styles
		DWORD styleExTab = 0;
		/** Titles of each tab item to be created. */
		std::vector<std::wstring> titles{};
	};

	/// Options to create a `Trackbar` programmatically.
	///
	/// The fields are declared in alphabetical order to make it easy to work
	/// with [designated initializers], which require the fields to be set
	/// the same order they appear in the struct.
	///
	/// [designated initializers]: https://en.cppreference.com/w/cpp/language/aggregate_initialization.html#Designated_initializers
	struct TrackbarOpts final {
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		WORD ctrlId = 0;
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		Lay layout = Lay::hold_hold;
		/** Page size. */
		int pageSize = 0;
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::TrackbarOpts myOpts{
		///     .pos = wl::dpi::pt(10, 10),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		POINT pos{};
		/// Minimum and maximum values for the selectable range.
		///
		/// Defaults to `{0, 100}`.
		std::pair<int, int> range = {0, 100};
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::TrackbarOpts myOpts{
		///     .size = wl::dpi::sz(175, 28),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SIZE size = {.cx = 175, .cy = 28};
		/// The [window] and [Trackbar style] passed to [`CreateWindowEx`].
		///
		/// [window]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [Trackbar style]: https://learn.microsoft.com/en-us/windows/win32/controls/trackbar-control-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD style = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE | TBS_AUTOTICKS | TBS_HORZ;
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD styleEx = WS_EX_LEFT;
		/** Current selected position. */
		int value = 0;
	};

	/// Options to create a `TreeView` programmatically.
	///
	/// The fields are declared in alphabetical order to make it easy to work
	/// with [designated initializers], which require the fields to be set
	/// the same order they appear in the struct.
	///
	/// [designated initializers]: https://en.cppreference.com/w/cpp/language/aggregate_initialization.html#Designated_initializers
	struct TreeViewOpts final {
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		WORD ctrlId = 0;
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		Lay layout = Lay::hold_hold;
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::TreeViewOpts myOpts{
		///     .pos = wl::dpi::pt(10, 10),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		POINT pos{};
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::TreeViewOpts myOpts{
		///     .size = wl::dpi::sz(120, 120),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SIZE size = {.cx = 120, .cy = 120};
		/// The [window] and [TreeView style] passed to [`CreateWindowEx`].
		///
		/// Note that, for safety reasons, `LVS_SHAREIMAGELISTS` will always be set.
		///
		/// [window]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [TreeView style]: https://learn.microsoft.com/en-us/windows/win32/controls/tree-view-control-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD style = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS | TVS_HASBUTTONS;
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD styleEx = WS_EX_LEFT | WS_EX_CLIENTEDGE;
		/// The [TreeView extended styles] applied right after the control is created.
		///
		/// [TreeView extended styles]: https://learn.microsoft.com/en-us/windows/win32/controls/tree-view-control-window-extended-styles
		DWORD styleExTreeView = 0;
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
	///     wl::WindowMain wnd{wl::MainOpts{
	///         .title = L"My main window",
	///     }};
	///     wl::Button btn{wnd, wl::ButtonOpts{
	///         .pos = wl::dpi::pt(10, 10),
	///         .text = L"&Click me",
	///     }};
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
	class Button final : public IWindowChild {
	public:
		/// Constructs the button, which will be created programmatically with [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		Button(IWindowParent &owner, ButtonOpts creationOpts);

		/// Constructs the button, which will be loaded from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		Button(IWindowParent &owner, WORD ctrlId, Lay layout);

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

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
	};

	/// @brief Native [check box] control.
	///
	/// Example of creating a window with a check box programmatically, .h and .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{wl::MainOpts{
	///         .title = L"My main window",
	///     }};
	///     wl::CheckBox chk{wnd, wl::CheckBoxOpts{
	///         .pos = wl::dpi::pt(10, 10);
	///         .text = L"&Check me";
	///     }};
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
	///     chk.on().bn_clicked([this]() -> void {
	///         std::wstring title = wl::str::fmt(L"Check box is %s", chk.is_checked() ? L"YES" : L"NO");
	///         wnd.set_title(title);
	///     });
	/// }
	/// ```
	///
	/// [check box]: https://learn.microsoft.com/en-us/windows/win32/controls/button-types-and-styles#check-boxes
	class CheckBox final : public IWindowChild {
	public:
		/// Constructs the check box, which will be created programmatically with [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		CheckBox(IWindowParent &owner, CheckBoxOpts opts);

		/// Constructs the check box, which will be loaded from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		CheckBox(IWindowParent &owner, WORD ctrlId, Lay layout);

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

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
	};

	/// @brief Native [combo box] control (dropdown).
	///
	/// Example of creating a window with a combo box programmatically, .h and .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{wl::MainOpts{
	///         .title = L"My main window",
	///     }};
	///     wl::ComboBox cmb{wnd, wl::ComboBoxOpts{
	///         .pos = wl::dpi::pt(10, 10),
	///         .texts = {L"Hello", L"World"},
	///     }};
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
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
	class ComboBox final : public IWindowChild {
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
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		ComboBox(IWindowParent &owner, ComboBoxOpts creationOpts);

		/// Constructs the combo box, which will be loaded from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		ComboBox(IWindowParent &owner, WORD ctrlId, Lay layout);

		/** Item methods. */
		ItemCollection items{*this};

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

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
	};

	/// @brief Native [date and time picker] control.
	///
	/// Example of creating a window with a date and time picker programmatically, .h and .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{wl::MainOpts{
	///         .title = L"My main window",
	///     }};
	///     wl::DateTimePicker dtp{wnd, wl::DateTimePickerOpts{
	///         .pos = wl::dpi::pt(10, 10),
	///     }};
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
	///     dtp.on().dtn_date_time_change([this](NMDATETIMECHANGE &p) -> void {
	///         std::wstring title = wl::str::fmt(L"%d-%d-%d", p.st.wYear, p.st.wMonth, p.st.wDay);
	///         wnd.set_title(title);
	///     });
	/// }
	/// ```
	///
	/// [date and time picker]: https://learn.microsoft.com/en-us/windows/win32/controls/date-and-time-picker-controls
	class DateTimePicker final : public IWindowChild {
	public:
		/// Constructs the date and time picker, which will be created programmatically with [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DateTimePicker(IWindowParent &owner, DateTimePickerOpts creationOpts);

		/// Constructs the date and time picker, which will be loaded from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		DateTimePicker(IWindowParent &owner, WORD ctrlId, Lay layout);

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

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
	};

	/// @brief Native [edit] control (textbox).
	///
	/// Example of creating a window with an edit programmatically, .h and .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{wl::MainOpts{
	///         .title = L"My main window",
	///     }};
	///     wl::Edit txt{wnd, wl::EditOpts{
	///         .pos = wl::dpi::pt(10, 10),
	///     }};
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
	class Edit final : public IWindowChild {
	public:
		/// Constructs the edit, which will be created programmatically with [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		Edit(IWindowParent &owner, EditOpts creationOpts);

		/// Constructs the edit, which will be loaded from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		Edit(IWindowParent &owner, WORD ctrlId, Lay layout);

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

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
	};

	/// @brief Native [list view] control.
	///
	/// Example of creating a window with a list view programmatically, .h and .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{wl::MainOpts{
	///         .title = L"My main window",
	///     }};
	///     wl::ListView lv{wnd, wl::ListViewOpts{
	///         .cols = {
	///             {L"First", wl::dpi::x(200)},
	///             {L"Second", wl::dpi::x(100)},
	///         },
	///         .pos = wl::dpi::pt(10, 10),
	///         .size = wl::dpi::sz(400, 200),
	///     }};
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
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
	class ListView final : public IWindowChild {
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
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		ListView(IWindowParent &owner, ListViewOpts creationOpts);

		/// Constructs the list view, which will be loaded from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		ListView(IWindowParent &owner, WORD ctrlId, Lay layout, WORD contextMenuId = 0);

		/** Column methods. */
		ColumnCollection cols{*this};

		/** Item methods. */
		ItemCollection items{*this};

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

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

		/// Retrieves the 16x16 `IStoreIcon`.
		///
		/// Allows icons to be added to the control's image list.
		/// An `Item` can display an icon referring to its zero-based index.
		IStoreIcon& icons_16();

		/// Retrieves the 32x32 `IStoreIcon`.
		///
		/// Allows icons to be added to the control's image list.
		/// An `Item` can display an icon referring to its zero-based index.
		IStoreIcon& icons_32();

	private:
		void load_context_menu(WORD contextMenuId);
		void custom_events();
		void show_context_menu(bool followCursor, bool hasCtrl, bool hasShift);
		_wl_internal::NativeCtrlBase _ctrl;
		events::ListViewEvents _events;
		HMENU _hMenuContext = nullptr;
		_wl_internal::ImageList _imgList16{{16, 16}}, _imgList32{{32, 32}};
	};

	/// @brief Native [month calendar] control.
	///
	/// [month calendar]: https://learn.microsoft.com/en-us/windows/win32/controls/month-calendar-controls
	class MonthCalendar final : public IWindowChild {
	public:
		/// Constructs the month calendar, which will be created programmatically with [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		MonthCalendar(IWindowParent &owner, MonthCalendarOpts creationOpts);

		/// Constructs the month calendar, which will be loaded from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		MonthCalendar(IWindowParent &owner, WORD ctrlId, Lay layout);

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

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
	};

	/// @brief Native [radio button] control.
	///
	/// Instead of creating the radio buttons directly, prefer using a `RadioGroup`.
	///
	/// [radio button]: https://learn.microsoft.com/en-us/windows/win32/controls/button-types-and-styles#radio-buttons
	class RadioButton final : public IWindowChild {
	public:
		/// Constructs the radio button, which will be created programmatically with [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		RadioButton(IWindowParent &owner, RadioButtonOpts creationOpts);

		/// Constructs the radio button, which will be loaded from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		RadioButton(IWindowParent &owner, WORD ctrlId, Lay layout);

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

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
		friend RadioGroup;
	};

	/// @brief Manages a group of `RadioButton` controls.
	///
	/// Example of creating a window with 3 radio buttons programmatically, .h and .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{wl::MainOpts{
	///         .title = L"My main window",
	///     }};
	///     wl::RadioGroup rads{wnd, {
	///         wl::RadioButtonOpts{
	///             .pos = wl::dpi::pt(420, 10),
	///             .text = L"First",
	///         },
	///         wl::RadioButtonOpts{
	///             .pos = wl::dpi::pt(420, 25),
	///             .text = L"Second",
	///         },
	///         wl::RadioButtonOpts{
	///             .pos = wl::dpi::pt(420, 40),
	///             .selected = true,
	///             .text = L"Turd",
	///         },
	///     }};
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
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

		/// Constructs the grouped radio buttons, which will be created programmatically with [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		RadioGroup(IWindowParent &owner, std::initializer_list<RadioButtonOpts> creationOpts);

		/// Constructs the radio group using each provided ID to programmatically create
		/// a radio button which will be loaded from the dialog resource.
		///
		/// The layout will be applied to all radio buttons.
		RadioGroup(IWindowParent &owner, Lay layout, std::initializer_list<WORD> ctrlIds);

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
		IWindowParent &_owner;
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
	///     wl::WindowMain wnd{wl::MainOpts{
	///         .title = L"My main window",
	///     }};
	///     wl::Static lbl{wnd, wl::StaticOpts{
	///         .pos = wl::dpi::pt(10, 10),
	///         .text = L"Hello",
	///     }};
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
	///     lbl.on().stn_clicked([this]() -> void {
	///         wnd.set_title(L"Label clicked");
	///     });
	/// }
	/// ```
	///
	/// [static]: https://learn.microsoft.com/en-us/windows/win32/controls/about-static-controls
	class Static final : public IWindowChild {
	public:
		/// Constructs the static, which will be created programmatically with [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		Static(IWindowParent &owner, StaticOpts creationOpts);

		/// Constructs the static, which will be loaded from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		Static(IWindowParent &owner, WORD ctrlId, Lay layout);

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

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
	};

	/// @brief Native [status bar] control.
	///
	/// Example of creating a window with a status bar, .h and .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{wl::MainOpts{
	///         .title = L"My main window",
	///     }};
	///     wl::StatusBar sb{wnd, {
	///         .icons = {
	///             wl::IconLoad{.ext = L"xlsx"},
	///         },
	///         .parts = {
	///             wl::SbPart{
	///                 .flex = 1,
	///                 .text = L"Here",
	///             },
	///             wl::SbPart{
	///                 .iconIndex = 0,
	///                 .text = L"Hello",
	///                 .width = wl::dpi::x(200),
	///             },
	///         },
	///     }};
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
	///     sb.on().nm_click([this](NMMOUSE &p) -> bool {
	///         MessageBoxW(wnd.hwnd(), L"Status bar clicked", L"Click", MB_ICONINFORMATION);
	///         return true;
	///     });
	/// }
	/// ```
	///
	/// [status bar]: https://learn.microsoft.com/en-us/windows/win32/controls/status-bars
	class StatusBar final : public IWindowChild {
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

			/** Returns true is the part width is flexible. */
			[[nodiscard]] constexpr bool is_flex() const { return !is_fixed_width(); }

			/** Returns true is the part has fixed width. */
			[[nodiscard]] constexpr bool is_fixed_width() const { return _owner._parts[_index].width > 0; }

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
			[[nodiscard]] constexpr size_t count() const { return _owner._parts.size(); }

		private:
			const StatusBar &_owner;
			friend StatusBar;
		};

		/// Constructs the status bar, which will be created programmatically with [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		StatusBar(IWindowParent &owner, StatusBarOpts creationOpts);

		/** Part methods. */
		PartCollection parts{*this};

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

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

	private:
		void resize_to_fit_parent(wm::Size p);
		_wl_internal::NativeCtrlBase _ctrl;
		events::StatusBarEvents _events;
		std::vector<SbPart> _parts;
		std::vector<int> _rightEdges{}; // buffer to speed up resize calls
		_wl_internal::HIconStore _iconStore16{{16, 16}};
	};

	/// @brief Native [tab] control.
	///
	/// Example of creating a window with a tab programmatically, with a `Button`
	/// inside the tab, .h and .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{wl::MainOpts{
	///         .title = L"My main window",
	///     }};
	///     wl::Tab tab{wnd, wl::TabOpts{
	///         .pos = wl::dpi::pt(10, 10),
	///         .size = wl::dpi::sz(300, 200),
	///         .titles = {L"First tab", L"Second tab"},
	///     }};
	///     wl::Button btn{tab.items[0].child(), wl::ButtonOpts{
	///         .pos = wl::dpi::pt(50, 40), // button position inside the 1st tab
	///         .text = L"&Click",
	///     }};
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
	/// [tab]: https://learn.microsoft.com/en-us/windows/win32/controls/tab-controls
	class Tab final : public IWindowChild {
	public:
		class ItemCollection; // forward declaration

		/** @brief A single item of the `Tab`. */
		class Item final {
		private:
			constexpr Item(Tab &owner, int itemIndex) : _owner{owner}, _index{itemIndex} { }

		public:
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
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		Tab(IWindowParent &owner, TabOpts creationOpts);

		/// Constructs the tab, which will be loaded from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		///
		/// The `childrenDlgIds` are the IDs of the dialog resources of each
		/// children to be added, and it should have the same number of elements
		/// in `titles`.
		Tab(IWindowParent &owner, WORD ctrlId, Lay layout,
			std::initializer_list<WORD> childrenDlgIds, std::initializer_list<WStrView> titles);

		/** Item methods. */
		ItemCollection items{*this};

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

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
		void create_tab(WStrView title) const;
		void display_cur_tab() const;
		void custom_events();
		_wl_internal::NativeCtrlBase _ctrl;
		events::TabEvents _events;
		_wl_internal::NonMovableArray<WindowControl> _children;
	};

	/// @brief Native [trackbar] control.
	///
	/// Example of creating a window with a trackbar programmatically, .h and .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{wl::MainOpts{
	///         .title = L"My main window",
	///     }};
	///     wl::Trackbar tb{wnd, wl::TrackbarOpts{
	///         .pos = wl::dpi::pt(10, 10),
	///         .range = {0, 8},
	///         .value = 6,
	///     }};
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
	///     tb.on().wm_h_scroll([this](wl::wm::HScroll p) -> void {
	///         std::wstring s = wl::str::fmt(L"Track bar at %d", tb.pos());
	///         wnd.set_title(s);
	///     });
	/// }
	/// ```
	///
	/// [trackbar]: https://learn.microsoft.com/en-us/windows/win32/controls/trackbar-controls
	class Trackbar final : public IWindowChild {
	public:
		/// Constructs the tree view, which will be created programmatically with [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		Trackbar(IWindowParent &owner, TrackbarOpts creationOpts);

		/// Constructs the tree view, which will be loaded from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		Trackbar(IWindowParent &owner, WORD ctrlId, Lay layout);

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

		/// Allows message events to be added.
		///
		/// The events must be added before the control is created on the screen.
		///
		/// Example:
		///
		/// ```cpp
		/// tb.on().wm_h_scroll([](wm::HScroll p) -> void {
		///     // ...
		/// });
		/// ```
		[[nodiscard]] constexpr events::TrackbarEvents& on() { return _wl_internal::valid_event(hwnd(), _events); }

		/// [Subclasses] the control allowing message events to be added.
		///
		/// The events must be added before the control is created on the screen.
		///
		/// Note that subclassing is a potentially slow technique, prefer using ordinary events.
		///
		/// [Subclasses]: https://learn.microsoft.com/en-us/windows/win32/controls/subclassing-overview
		[[nodiscard]] constexpr events::WindowEvents& subclass_on() { return _wl_internal::valid_event(hwnd(), _ctrl._subclassEvents); }

		/** Retrieves the current page size. */
		[[nodiscard]] int page_size() const;

		/** Sets the current page size. */
		const Trackbar& set_page_size(int pageSize) const;

		/** Retrieves the current position. */
		[[nodiscard]] int pos() const;

		/** Sets the current position. */
		const Trackbar& set_pos(int value) const;

		/** Returns the current minimum and maximum range values. */
		[[nodiscard]] std::pair<int, int> range() const;

		/** Sets the current minimum and maximum range values. */
		const Trackbar& set_range(int rangeMin, int rangeMax) const;

		/** Sets the current minimum and maximum range values. */
		const Trackbar& set_range(std::pair<int, int> rangeMinMax) const;

	private:
		_wl_internal::NativeCtrlBase _ctrl;
		events::TrackbarEvents _events;
	};

	/// @brief Native [tree view] control.
	///
	/// [tree view]: https://learn.microsoft.com/en-us/windows/win32/controls/tree-view-controls
	class TreeView final : public IWindowChild {
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
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		TreeView(IWindowParent &owner, TreeViewOpts creationOpts);

		/// Constructs the tree view, which will be loaded from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog resource.
		TreeView(IWindowParent &owner, WORD ctrlId, Lay layout);

		/** Item methods. */
		ItemCollection items{*this};

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const override { return _ctrl._hWnd; }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const override { return _events._ctrlEvents._ctrlId; }

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

		/// Retrieves the 16x16 `IStoreIcon`.
		///
		/// Allows icons to be added to the control's image list.
		/// An `Item` can display an icon referring to its zero-based index.
		IStoreIcon& icons_16();

	private:
		_wl_internal::NativeCtrlBase _ctrl;
		events::TreeViewEvents _events;
		_wl_internal::ImageList _imgList16{{16, 16}};
	};

}
