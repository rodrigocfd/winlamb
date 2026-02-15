#pragma once
#include "ui-wnd.hpp"

namespace _wl_internal {

	/// Base to all native controls.
	/// Stores the subclass messages.
	class NativeCtrlBase final : private wl::NoCopyNoMove {
	public:
		explicit NativeCtrlBase(WndBase &parentWndBase) : _parent{parentWndBase} { }

		void create_wnd(WORD ctrlId, DWORD exStyle, const wchar_t *className,
			std::wstring &&title, DWORD style, POINT pos, SIZE size);
		void assign_dlg(WORD ctrlId);
		void install_subclass();

		static LRESULT CALLBACK subclass_proc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp,
			UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

		HWND _hWnd = nullptr; // _hWnd member is set during control creation
		WndBase &_parent;
		wl::events::WindowEvents _subclassEvents{false};
	};

	/** Base to all native control events. */
	class NativeCtrlEvents final : private wl::NoCopyNoMove {
	public:
		NativeCtrlEvents(WndBase &parentWndBase, WORD ctrlId)
			: _parent{parentWndBase}, _ctrlId{ctrlId} { }

		WndBase &_parent;
		WORD _ctrlId;
	};

	/** An icon to be loaded either by its resource ID or file extension. */
	struct IconToLoad final {
		WORD id = 0;
		std::wstring ext{};
	};

}

namespace wl::events {

	/** Native `Button` control events. */
	class ButtonEvents final : private wl::NoCopyNoMove {
	private:
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
	class ComboBoxEvents final : private wl::NoCopyNoMove {
	private:
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
	class DateTimePickerEvents final : private wl::NoCopyNoMove {
	private:
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
	class EditEvents final : private wl::NoCopyNoMove {
	private:
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
	class ListViewEvents final : private wl::NoCopyNoMove {
	private:
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
	class MonthCalendarEvents final : private wl::NoCopyNoMove {
	private:
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
	class RadioGroupEvents final : private wl::NoCopyNoMove {
	private:
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
	class StaticEvents final : private wl::NoCopyNoMove {
	private:
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
	class StatusBarEvents final : private wl::NoCopyNoMove {
	private:
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

	/** Native `SysLink` events. */
	class SysLinkEvents final : private wl::NoCopyNoMove {
	private:
		SysLinkEvents(_wl_internal::WndBase &parentWndBase, WORD ctrlId)
			: _ctrlEvents{parentWndBase, ctrlId} { }

	public:
		void nm_click(std::function<void(NMLINK&)> &&cb);

	private:
		_wl_internal::NativeCtrlEvents _ctrlEvents;
		friend wl::SysLink;
	};

	/** Native `Tab` events. */
	class TabEvents final : private wl::NoCopyNoMove {
	private:
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
	class TrackbarEvents final : private wl::NoCopyNoMove {
	private:
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
	class TreeViewEvents final : private wl::NoCopyNoMove {
	private:
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

	/** Options to create a `Button` programmatically. */
	struct ButtonOpts final {
		/** Constructor. */
		constexpr explicit ButtonOpts(IWindowParent &owner) : _owner{owner} { }
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		ButtonOpts ctrl_id(WORD id) { _ctrlId = id; return std::move(*this); }
		/// Control text passed to [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		ButtonOpts text(WStrView t) { _text = t.c_str(); return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::Button{
		///     wl::ButtonOpts{wnd}
		///         .pos(wl::dpi::pt(10, 10))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		ButtonOpts pos(POINT p) { _pos = p; return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::Button{
		///     wl::ButtonOpts{wnd}
		///         .pos(wl::dpi::x(10) wl::dpi::y(10))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		ButtonOpts pos(int x, int y) { return pos({x, y}); }
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::Button{
		///     wl::ButtonOpts{wnd}
		///         .size(wl::dpi::sz(88, 26))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		ButtonOpts size(SIZE p) { _size = p; return std::move(*this); }
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::Button{
		///     wl::ButtonOpts{wnd}
		///         .size(wl::dpi::x(88) wl::dpi::y(26))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		ButtonOpts size(int cx, int cy) { return size({cx, cy}); }
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		ButtonOpts layout(Lay l) { _layout = l; return std::move(*this); }
		/// The [window] and [Button style] passed to [`CreateWindowEx`].
		///
		/// [window]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [Button style]: https://learn.microsoft.com/en-us/windows/win32/controls/button-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		ButtonOpts style(DWORD s) { _style = s; return std::move(*this); }
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		ButtonOpts style_ex(DWORD sx) { _styleEx = sx; return std::move(*this); }

	private:
		IWindowParent &_owner;
		WORD _ctrlId = 0;
		std::wstring _text{};
		POINT _pos{};
		SIZE _size{-1, -1};
		Lay _layout = Lay::hold_hold;
		DWORD _style = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE | BS_PUSHBUTTON;
		DWORD _styleEx = WS_EX_LEFT;
		friend Button;
	};

	/** Options to create a `CheckBox` programmatically. */
	struct CheckBoxOpts final {
		/** Constructor. */
		constexpr explicit CheckBoxOpts(IWindowParent &owner) : _owner{owner} { }
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		CheckBoxOpts ctrl_id(WORD id) { _ctrlId = id; return std::move(*this); }
		/// Control text passed to [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		CheckBoxOpts text(WStrView t) { _text = t.c_str(); return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::CheckBox{
		///     wl::CheckBoxOpts{wnd}
		///         .pos(wl::dpi::pt(10, 10))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		CheckBoxOpts pos(POINT p) { _pos = p; return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::CheckBox{
		///     wl::CheckBoxOpts{wnd}
		///         .pos(wl::dpi::x(10) wl::dpi::y(10))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		CheckBoxOpts pos(int x, int y) { return pos({x, y}); }
		/// Control size passed to [`CreateWindowEx`].
		///
		/// If not defined, the control will resize to automatically fit its
		/// initial text.
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::CheckBox{
		///     wl::CheckBoxOpts{wnd}
		///         .size(wl::dpi::sz(88, 26))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		CheckBoxOpts size(SIZE p) { _size = p; return std::move(*this); }
		/// Control size passed to [`CreateWindowEx`].
		///
		/// If not defined, the control will resize to automatically fit its
		/// initial text.
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::CheckBox{
		///     wl::CheckBoxOpts{wnd}
		///         .size(wl::dpi::x(88) wl::dpi::y(26))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		CheckBoxOpts size(int cx, int cy) { return size({cx, cy}); }
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		CheckBoxOpts layout(Lay l) { _layout = l; return std::move(*this); }
		/// The [window] and [CheckBox style] passed to [`CreateWindowEx`].
		///
		/// [window]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [CheckBox style]: https://learn.microsoft.com/en-us/windows/win32/controls/button-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		CheckBoxOpts style(DWORD s) { _style = s; return std::move(*this); }
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		CheckBoxOpts style_ex(DWORD sx) { _styleEx = sx; return std::move(*this); }
		/** Initial state. */
		CheckBoxOpts checked(bool c) { _checked = c; return std::move(*this); }

	private:
		IWindowParent &_owner;
		WORD _ctrlId = 0;
		std::wstring _text{};
		POINT _pos{};
		SIZE _size{-1, -1};
		Lay _layout = Lay::hold_hold;
		DWORD _style = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE | BS_AUTOCHECKBOX;
		DWORD _styleEx = WS_EX_LEFT;
		bool _checked = false;
		friend CheckBox;
	};

	/** Options to create a `ComboBox` programmatically. */
	struct ComboBoxOpts final {
		/** Constructor. */
		constexpr explicit ComboBoxOpts(IWindowParent &owner) : _owner{owner} { }
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		ComboBoxOpts ctrl_id(WORD id) { _ctrlId = id; return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::ComboBox{
		///     wl::ComboBoxOpts{wnd}
		///         .pos(wl::dpi::pt(10, 10))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		ComboBoxOpts pos(POINT p) { _pos = p; return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::ComboBox{
		///     wl::ComboBoxOpts{wnd}
		///         .pos(wl::dpi::x(10) wl::dpi::y(10))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		ComboBoxOpts pos(int x, int y) { return pos({x, y}); }
		/// Control width passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::ComboBox{
		///     wl::ComboBoxOpts{wnd}
		///         .width(wl::dpi::x(100))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		ComboBoxOpts width(int cx) { _width = cx; return std::move(*this); }
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		ComboBoxOpts layout(Lay l) { _layout = l; return std::move(*this); }
		/// The [window] and [ComboBox style] passed to [`CreateWindowEx`].
		///
		/// [window]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [ComboBox style]: https://learn.microsoft.com/en-us/windows/win32/controls/combo-box-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		ComboBoxOpts style(DWORD s) { _style = s; return std::move(*this); }
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		ComboBoxOpts style_ex(DWORD sx) { _styleEx = sx; return std::move(*this); }
		/** Texts to be added. */
		ComboBoxOpts texts(std::vector<std::wstring> t) { _texts = std::move(t); return std::move(*this); }

	private:
		IWindowParent &_owner;
		WORD _ctrlId = 0;
		POINT _pos{};
		int _width = -1;
		Lay _layout = Lay::hold_hold;
		DWORD _style = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE | CBS_DROPDOWNLIST;
		DWORD _styleEx = WS_EX_LEFT;
		std::vector<std::wstring> _texts{};
		friend ComboBox;
	};

	/** Options to create a `DateTimePicker` programmatically. */
	struct DateTimePickerOpts final {
		/** Constructor. */
		constexpr explicit DateTimePickerOpts(IWindowParent &owner) : _owner{owner} { }
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		DateTimePickerOpts ctrl_id(WORD id) { _ctrlId = id; return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::DateTimePicker{
		///     wl::DateTimePickerOpts{wnd}
		///         .pos(wl::dpi::pt(10, 10))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DateTimePickerOpts pos(POINT p) { _pos = p; return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::DateTimePicker{
		///     wl::DateTimePickerOpts{wnd}
		///         .pos(wl::dpi::x(10) wl::dpi::y(10))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DateTimePickerOpts pos(int x, int y) { return pos({x, y}); }
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::DateTimePicker{
		///     wl::DateTimePickerOpts{wnd}
		///         .size(wl::dpi::sz(230, 23))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DateTimePickerOpts size(SIZE p) { _size = p; return std::move(*this); }
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::DateTimePicker{
		///     wl::DateTimePickerOpts{wnd}
		///         .size(wl::dpi::x(230) wl::dpi::y(23))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DateTimePickerOpts size(int cx, int cy) { return size({cx, cy}); }
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		DateTimePickerOpts layout(Lay l) { _layout = l; return std::move(*this); }
		/// The [window] and [DateTimePicker style] passed to [`CreateWindowEx`].
		///
		/// [window]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [DateTimePicker style]: https://learn.microsoft.com/en-us/windows/win32/controls/date-and-time-picker-control-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DateTimePickerOpts style(DWORD s) { _style = s; return std::move(*this); }
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DateTimePickerOpts style_ex(DWORD sx) { _styleEx = sx; return std::move(*this); }
		/** Initial value. */
		DateTimePickerOpts value(Time t) { _value = t; return std::move(*this); }

	private:
		IWindowParent &_owner;
		WORD _ctrlId = 0;
		POINT _pos{};
		SIZE _size{-1, -1};
		Lay _layout = Lay::hold_hold;
		DWORD _style = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE | DTS_LONGDATEFORMAT;
		DWORD _styleEx = WS_EX_LEFT | WS_EX_CLIENTEDGE;
		Time _value{Time::Zero{}};
		friend DateTimePicker;
	};

	/** Options to create an `Edit` programmatically. */
	struct EditOpts final {
		/** Constructor. */
		constexpr explicit EditOpts(IWindowParent &owner) : _owner{owner} { }
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		EditOpts ctrl_id(WORD id) { _ctrlId = id; return std::move(*this); }
		/// Control text passed to [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		EditOpts text(WStrView t) { _text = t.c_str(); return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::Edit{
		///     wl::EditOpts{wnd}
		///         .pos(wl::dpi::pt(10, 10))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		EditOpts pos(POINT p) { _pos = p; return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::Edit{
		///     wl::EditOpts{wnd}
		///         .pos(wl::dpi::x(10) wl::dpi::y(10))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		EditOpts pos(int x, int y) { return pos({x, y}); }
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::Edit{
		///     wl::EditOpts{wnd}
		///         .size(wl::dpi::sz(100, 23))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		EditOpts size(SIZE p) { _size = p; return std::move(*this); }
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::Edit{
		///     wl::EditOpts{wnd}
		///         .size(wl::dpi::x(100) wl::dpi::y(23))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		EditOpts size(int cx, int cy) { return size({cx, cy}); }
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		EditOpts layout(Lay l) { _layout = l; return std::move(*this); }
		/// The [window] and [Edit style] passed to [`CreateWindowEx`].
		///
		/// [window]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [Edit style]: https://learn.microsoft.com/en-us/windows/win32/controls/edit-control-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		EditOpts style(DWORD s) { _style = s; return std::move(*this); }
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		EditOpts style_ex(DWORD sx) { _styleEx = sx; return std::move(*this); }

	private:
		IWindowParent &_owner;
		WORD _ctrlId = 0;
		std::wstring _text{};
		POINT _pos{};
		SIZE _size{-1, -1};
		Lay _layout = Lay::hold_hold;
		DWORD _style = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE | ES_AUTOHSCROLL | ES_NOHIDESEL;
		DWORD _styleEx = WS_EX_LEFT | WS_EX_CLIENTEDGE;
		friend Edit;
	};

	/** Options to create a `ListView` programmatically. */
	struct ListViewOpts final {
		/** Constructor. */
		constexpr explicit ListViewOpts(IWindowParent &owner) : _owner{owner} { }
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		ListViewOpts ctrl_id(WORD id) { _ctrlId = id; return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::ListView{
		///     wl::ListViewOpts{wnd}
		///         .pos(wl::dpi::pt(10, 10))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		ListViewOpts pos(POINT p) { _pos = p; return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::ListView{
		///     wl::ListViewOpts{wnd}
		///         .pos(wl::dpi::x(10) wl::dpi::y(10))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		ListViewOpts pos(int x, int y) { return pos({x, y}); }
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::ListView{
		///     wl::ListViewOpts{wnd}
		///         .size(wl::dpi::sz(120, 120))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		ListViewOpts size(SIZE p) { _size = p; return std::move(*this); }
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::ListView{
		///     wl::ListViewOpts{wnd}
		///         .size(wl::dpi::x(120) wl::dpi::y(120))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		ListViewOpts size(int cx, int cy) { return size({cx, cy}); }
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		ListViewOpts layout(Lay l) { _layout = l; return std::move(*this); }
		/// The [window] and [ListView style] passed to [`CreateWindowEx`].
		///
		/// Note that, for safety reasons, `LVS_SHAREIMAGELISTS` will always be
		/// set.
		///
		/// [window]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [ListView style]: https://learn.microsoft.com/en-us/windows/win32/controls/list-view-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		ListViewOpts style(DWORD s) { _style = s; return std::move(*this); }
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		ListViewOpts style_ex(DWORD sx) { _styleEx = sx; return std::move(*this); }
		/// The [ListView extended styles] applied right after the control is
		/// created.
		///
		/// [ListView extended styles]: https://learn.microsoft.com/en-us/windows/win32/controls/extended-list-view-styles
		ListViewOpts style_ex_lv(DWORD sxlv) { _styleExLv = sxlv; return std::move(*this); }
		/// Columns to be added, title and width.
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::ListView{
		///     wl::ListViewOpts{wnd}
		///         .col(L"First", wl::dpi::x(100))
		///         .col(L"Second", wl::dpi::x(120))
		/// };
		/// ```
		ListViewOpts col(WStrView text, UINT width);
		/// Loads the 16x16 icon of the given file extension into the `ListView`.
		///
		/// They can be later referenced by index, following the same order they
		/// are added.
		ListViewOpts icon16_ext(WStrView fileExt);
		/// Loads the 16x16 icon of the given resource ID into the `ListView`.
		///
		/// They can be later referenced by index, following the same order they
		/// are added.
		ListViewOpts icon16_id(WORD id);
		/// Loads the 32x32 icon of the given file extension into the `ListView`.
		///
		/// They can be later referenced by index, following the same order they
		/// are added.
		ListViewOpts icon32_ext(WStrView fileExt);
		/// Loads the 32x32 icon of the given resource ID into the `ListView`.
		///
		/// They can be later referenced by index, following the same order they
		/// are added.
		ListViewOpts icon32_id(WORD id);
		/// Context menu resource to be loaded as the context menu with [`LoadMenu`].
		///
		/// [`LoadMenu`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-loadmenuw
		ListViewOpts context_menu_id(WORD id) { _contextMenuId = id; return std::move(*this); }

	private:
		struct Col final {
			std::wstring text{};
			UINT width = 0;
		};

		IWindowParent &_owner;
		WORD _ctrlId = 0;
		POINT _pos{};
		SIZE _size{-1, -1};
		Lay _layout = Lay::hold_hold;
		DWORD _style = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE | LVS_REPORT | LVS_NOSORTHEADER | LVS_SHOWSELALWAYS;
		DWORD _styleEx = WS_EX_LEFT | WS_EX_CLIENTEDGE;
		DWORD _styleExLv = LVS_EX_FULLROWSELECT;
		std::vector<Col> _cols{};
		std::vector<_wl_internal::IconToLoad> _icons16{}, _icons32{};
		WORD _contextMenuId = 0;
		friend ListView;
	};

	/** Options to create a `MonthCalendar` programmatically. */
	struct MonthCalendarOpts final {
		/** Constructor. */
		constexpr explicit MonthCalendarOpts(IWindowParent &owner) : _owner{owner} { }
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		MonthCalendarOpts ctrl_id(WORD id) { _ctrlId = id; return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::MonthCalendar{
		///     wl::MonthCalendarOpts{wnd}
		///         .pos(wl::dpi::pt(10, 10))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		MonthCalendarOpts pos(POINT p) { _pos = p; return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::MonthCalendar{
		///     wl::MonthCalendarOpts{wnd}
		///         .pos(wl::dpi::x(10) wl::dpi::y(10))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		MonthCalendarOpts pos(int x, int y) { return pos({x, y}); }
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		MonthCalendarOpts layout(Lay l) { _layout = l; return std::move(*this); }
		/// The [window] and [MonthCalendar style] passed to [`CreateWindowEx`].
		///
		/// [window]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [MonthCalendar style]: https://learn.microsoft.com/en-us/windows/win32/controls/month-calendar-control-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		MonthCalendarOpts style(DWORD s) { _style = s; return std::move(*this); }
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		MonthCalendarOpts style_ex(DWORD sx) { _styleEx = sx; return std::move(*this); }
		/** Initial value. */
		MonthCalendarOpts value(Time t) { _value = t; return std::move(*this); }

	private:
		IWindowParent &_owner;
		WORD _ctrlId = 0;
		POINT _pos{};
		Lay _layout = Lay::hold_hold;
		DWORD _style = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE;
		DWORD _styleEx = WS_EX_LEFT;
		Time _value{Time::Zero{}};
		friend MonthCalendar;
	};

	struct RadioGroupOpts; // forward declaration

	/** Options to create a `RadioButton` programmatically. */
	struct RadioButtonOpts final {
		/** Constructor. */
		constexpr explicit RadioButtonOpts(IWindowParent &owner) : _owner{owner} { }
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		RadioButtonOpts ctrl_id(WORD id) { _ctrlId = id; return std::move(*this); }
		/// Control text passed to [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		RadioButtonOpts text(WStrView t) { _text = t.c_str(); return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::RadioButton{
		///     wl::RadioButtonOpts{wnd}
		///         .pos(wl::dpi::pt(88, 26))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		RadioButtonOpts pos(POINT p) { _pos = p; return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::RadioButton{
		///     wl::RadioButtonOpts{wnd}
		///         .pos(wl::dpi::x(88) wl::dpi::y(26))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		RadioButtonOpts pos(int x, int y) { return pos({x, y}); }
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::RadioButton{
		///     wl::RadioButtonOpts{wnd}
		///         .size(wl::dpi::sz(88, 26))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		RadioButtonOpts size(SIZE p) { _size = p; return std::move(*this); }
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::RadioButton{
		///     wl::RadioButtonOpts{wnd}
		///         .size(wl::dpi::x(88) wl::dpi::y(26))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		RadioButtonOpts size(int cx, int cy) { return size({cx, cy}); }
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		RadioButtonOpts layout(Lay l) { _layout = l; return std::move(*this); }
		/// The [window] and [RadioButton style] passed to [`CreateWindowEx`].
		///
		/// [window]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [RadioButton style]: https://learn.microsoft.com/en-us/windows/win32/controls/button-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		RadioButtonOpts style(DWORD s) { _style = s; return std::move(*this); }
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		RadioButtonOpts style_ex(DWORD sx) { _styleEx = sx; return std::move(*this); }
		/** `RadioButton` is initially selected. */
		RadioButtonOpts select() { _selected = true; return std::move(*this); }

	private:
		IWindowParent &_owner;
		WORD _ctrlId = 0;
		std::wstring _text{};
		POINT _pos{};
		SIZE _size{-1, -1};
		Lay _layout = Lay::hold_hold;
		DWORD _style = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE | BS_AUTORADIOBUTTON;
		DWORD _styleEx = WS_EX_LEFT;
		bool _selected = false;
		friend RadioButton;
		friend RadioGroup;
		friend RadioGroupOpts;
	};

	/** Options to create a `RadioGroup` programmatically. */
	struct RadioGroupOpts final {
		/** Constructor. */
		constexpr explicit RadioGroupOpts(IWindowParent &owner) : _owner{owner} { }
		/** Adds a new `RadioButton` to the group. */
		RadioGroupOpts radio() { _opts.emplace_back(_owner); return std::move(*this); }
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		RadioGroupOpts ctrl_id(WORD id) { _opts.back()._ctrlId = id; return std::move(*this); }
		/// Control text passed to [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		RadioGroupOpts text(WStrView t) { _opts.back()._text = t.c_str(); return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::RadioGroup{
		///     wl::RadioGroupOpts{wnd}
		///         .radio()
		///         .pos(wl::dpi::pt(88, 26))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		RadioGroupOpts pos(POINT p) { _opts.back()._pos = p; return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::RadioGroup{
		///     wl::RadioGroupOpts{wnd}
		///         .radio()
		///         .pos(wl::dpi::x(88) wl::dpi::y(26))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		RadioGroupOpts pos(int x, int y) { return pos({x, y}); }
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::RadioGroup{
		///     wl::RadioGroupOpts{wnd}
		///         .radio()
		///         .size(wl::dpi::sz(88, 26))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		RadioGroupOpts size(SIZE p) { _opts.back()._size = p; return std::move(*this); }
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::RadioGroup{
		///     wl::RadioGroupOpts{wnd}
		///         .radio()
		///         .size(wl::dpi::x(88) wl::dpi::y(26))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		RadioGroupOpts size(int cx, int cy) { return size({cx, cy}); }
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		RadioGroupOpts layout(Lay l) { _opts.back()._layout = l; return std::move(*this); }
		/// The [window] and [RadioButton style] passed to [`CreateWindowEx`].
		///
		/// [window]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [RadioButton style]: https://learn.microsoft.com/en-us/windows/win32/controls/button-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		RadioGroupOpts style(DWORD s) { _opts.back()._style = s; return std::move(*this); }
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		RadioGroupOpts style_ex(DWORD sx) { _opts.back()._styleEx = sx; return std::move(*this); }
		/** `RadioButton` is initially selected. */
		RadioGroupOpts select() { _opts.back()._selected = true; return std::move(*this); }

	private:
		IWindowParent &_owner;
		std::vector<RadioButtonOpts> _opts{};
		friend RadioGroup;
	};

	/** Options to create a `Static` programmatically. */
	struct StaticOpts final {
		/** Constructor. */
		constexpr explicit StaticOpts(IWindowParent &owner) : _owner{owner} { }
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		StaticOpts ctrl_id(WORD id) { _ctrlId = id; return std::move(*this); }
		/// Control text passed to [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		StaticOpts text(WStrView t) { _text = t.c_str(); return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::Static{
		///     wl::StaticOpts{wnd}
		///         .pos(wl::dpi::pt(10, 10))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		StaticOpts pos(POINT p) { _pos = p; return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::Static{
		///     wl::StaticOpts{wnd}
		///         .pos(wl::dpi::x(10) wl::dpi::y(10))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		StaticOpts pos(int x, int y) { return pos({x, y}); }
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::Static{
		///     wl::StaticOpts{wnd}
		///         .size(wl::dpi::sz(88, 26))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		StaticOpts size(SIZE p) { _size = p; return std::move(*this); }
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::Static{
		///     wl::StaticOpts{wnd}
		///         .size(wl::dpi::x(88) wl::dpi::y(26))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		StaticOpts size(int cx, int cy) { return size({cx, cy}); }
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		StaticOpts layout(Lay l) { _layout = l; return std::move(*this); }
		/// The [window] and [Button style] passed to [`CreateWindowEx`].
		///
		/// [window]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [Button style]: https://learn.microsoft.com/en-us/windows/win32/controls/button-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		StaticOpts style(DWORD s) { _style = s; return std::move(*this); }
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		StaticOpts style_ex(DWORD sx) { _styleEx = sx; return std::move(*this); }

	private:
		private:
		IWindowParent &_owner;
		WORD _ctrlId = 0;
		std::wstring _text{};
		POINT _pos{};
		SIZE _size{-1, -1};
		Lay _layout = Lay::hold_hold;
		DWORD _style = WS_CHILD | WS_VISIBLE | SS_LEFT | SS_NOTIFY;
		DWORD _styleEx = WS_EX_LEFT;
		friend Static;
	};

	/** Options to create a `StatusBar` programmatically. */
	struct StatusBarOpts final {
		/** Constructor. */
		constexpr explicit StatusBarOpts(IWindowParent &owner) : _owner{owner} { }
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		StatusBarOpts ctrl_id(WORD id) { _ctrlId = id; return std::move(*this); }
		/// Loads the icon of the given file extension into the `StatusBar`.
		///
		/// They can be later referenced by index, following the same order they
		/// are added.
		StatusBarOpts icon_ext(WStrView fileExt);
		/// Loads the icon of the given resource ID into the `StatusBar`.
		///
		/// They can be later referenced by index, following the same order they
		/// are added.
		StatusBarOpts icon_id(WORD id);
		/// Adds a fixed-width part.
		///
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::StatusBar{
		///     wl::StatusBarOpts{wnd}
		///         .fixed_part(wl::dpi::x(100), L"Hello")
		/// };
		/// ```
		StatusBarOpts part_fixed(int px, WStrView initialText = L"", int iconIndex = -1);
		/** Adds a variable-sized part, which will resize according to the remaining space. */
		StatusBarOpts part_flex(int weight, WStrView initialText = L"", int iconIndex = -1);

	private:
		struct Part final {
			int width = -1;
			int flex = -1;
			std::wstring text{};
			int iconIndex = -1;
		};

		IWindowParent &_owner;
		WORD _ctrlId = 0;
		std::vector<_wl_internal::IconToLoad> _icons{};
		std::vector<Part> _parts{};
		friend StatusBar;
	};

	/** Options to create a `SysLink` programmatically. */
	struct SysLinkOpts final {
		/** Constructor. */
		constexpr explicit SysLinkOpts(IWindowParent &owner) : _owner{owner} { }
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		SysLinkOpts ctrl_id(WORD id) { _ctrlId = id; return std::move(*this); }
		/// Control text passed to [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SysLinkOpts text(WStrView t) { _text = t.c_str(); return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::SysLink{
		///     wl::SysLinkOpts{wnd}
		///         .pos(wl::dpi::pt(10, 10))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SysLinkOpts pos(POINT p) { _pos = p; return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::SysLink{
		///     wl::SysLinkOpts{wnd}
		///         .pos(wl::dpi::x(10) wl::dpi::y(10))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SysLinkOpts pos(int x, int y) { return pos({x, y}); }
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::SysLink{
		///     wl::SysLinkOpts{wnd}
		///         .size(wl::dpi::sz(88, 26))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SysLinkOpts size(SIZE p) { _size = p; return std::move(*this); }
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::SysLink{
		///     wl::SysLinkOpts{wnd}
		///         .size(wl::dpi::x(88) wl::dpi::y(26))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SysLinkOpts size(int cx, int cy) { return size({cx, cy}); }
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		SysLinkOpts layout(Lay l) { _layout = l; return std::move(*this); }
		/// The [window] and [SysLink style] passed to [`CreateWindowEx`].
		///
		/// [window]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [SysLink style]: https://learn.microsoft.com/en-us/windows/win32/controls/syslink-control-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SysLinkOpts style(DWORD s) { _style = s; return std::move(*this); }
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SysLinkOpts style_ex(DWORD sx) { _styleEx = sx; return std::move(*this); }

	private:
		IWindowParent &_owner;
		WORD _ctrlId = 0;
		std::wstring _text{};
		POINT _pos{};
		SIZE _size{-1, -1};
		Lay _layout = Lay::hold_hold;
		DWORD _style = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE | LWS_TRANSPARENT;
		DWORD _styleEx = WS_EX_LEFT;
		friend SysLink;
	};

	/** Options to create a `Tab` programmatically. */
	struct TabOpts final {
		/** Constructor. */
		constexpr explicit TabOpts(IWindowParent &owner) : _owner{owner} { }
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		TabOpts ctrl_id(WORD id) { _ctrlId = id; return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::Tab{
		///     wl::TabOpts{wnd}
		///         .pos(wl::dpi::pt(10, 10))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		TabOpts pos(POINT p) { _pos = p; return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::Tab{
		///     wl::TabOpts{wnd}
		///         .pos(wl::dpi::x(10) wl::dpi::y(10))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		TabOpts pos(int x, int y) { return pos({x, y}); }
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::Tab{
		///     wl::TabOpts{wnd}
		///         .size(wl::dpi::sz(120, 120))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		TabOpts size(SIZE p) { _size = p; return std::move(*this); }
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::Tab{
		///     wl::TabOpts{wnd}
		///         .size(wl::dpi::x(120) wl::dpi::y(120))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		TabOpts size(int cx, int cy) { return size({cx, cy}); }
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		TabOpts layout(Lay l) { _layout = l; return std::move(*this); }
		/// The [window] and [Tab style] passed to [`CreateWindowEx`].
		///
		/// [window]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [Tab style]: https://learn.microsoft.com/en-us/windows/win32/controls/tree-view-control-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		TabOpts style(DWORD s) { _style = s; return std::move(*this); }
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		TabOpts style_ex(DWORD sx) { _styleEx = sx; return std::move(*this); }
		/// The [Tab extended styles] applied right after the control is created.
		///
		/// [Tab extended styles]: https://learn.microsoft.com/en-us/windows/win32/controls/tab-control-extended-styles
		TabOpts style_ex_tab(DWORD sxt) { _styleExTab = sxt; return std::move(*this); }
		/** Titles of each tab item to be created. */
		TabOpts titles(std::vector<std::wstring> t) { _titles = std::move(t); return std::move(*this); }
		/** Zero-based index of initially selected tab item. */
		TabOpts select(size_t i) { _sel = i; return std::move(*this); }

	private:
		IWindowParent &_owner;
		WORD _ctrlId = 0;
		POINT _pos{};
		SIZE _size{-1, -1};
		Lay _layout = Lay::hold_hold;
		DWORD _style = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE;
		DWORD _styleEx = WS_EX_LEFT;
		DWORD _styleExTab = 0;
		std::vector<std::wstring> _titles{};
		size_t _sel = 0;
		friend Tab;
	};

	/** Options to create a `Trackbar` programmatically. */
	struct TrackbarOpts final {
		/** Constructor. */
		constexpr explicit TrackbarOpts(IWindowParent &owner) : _owner{owner} { }
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		TrackbarOpts ctrl_id(WORD id) { _ctrlId = id; return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::Trackbar{
		///     wl::TrackbarOpts{wnd}
		///         .pos(wl::dpi::pt(10, 10))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		TrackbarOpts pos(POINT p) { _pos = p; return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::Trackbar{
		///     wl::Trackbar{wnd}
		///         .pos(wl::dpi::x(10) wl::dpi::y(10))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		TrackbarOpts pos(int x, int y) { return pos({x, y}); }
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::Trackbar{
		///     wl::TrackbarOpts{wnd}
		///         .size(wl::dpi::sz(175, 28))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		TrackbarOpts size(SIZE p) { _size = p; return std::move(*this); }
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::Trackbar{
		///     wl::TrackbarOpts{wnd}
		///         .size(wl::dpi::x(175) wl::dpi::y(28))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		TrackbarOpts size(int cx, int cy) { return size({cx, cy}); }
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		TrackbarOpts layout(Lay l) { _layout = l; return std::move(*this); }
		/// The [window] and [Trackbar style] passed to [`CreateWindowEx`].
		///
		/// [window]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [Trackbar style]: https://learn.microsoft.com/en-us/windows/win32/controls/trackbar-control-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		TrackbarOpts style(DWORD s) { _style = s; return std::move(*this); }
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		TrackbarOpts style_ex(DWORD sx) { _styleEx = sx; return std::move(*this); }
		/// Minimum and maximum values for the selectable range.
		///
		/// Defaults to `0, 100`.
		TrackbarOpts range(int min, int max) { _range = {min, max}; return std::move(*this); }
		/** Page size. */
		TrackbarOpts page_size(int p) { _page = p; return std::move(*this); }
		/** Current selected position. */
		TrackbarOpts value(int v) { _value = v; return std::move(*this); }

	private:
		IWindowParent &_owner;
		WORD _ctrlId = 0;
		POINT _pos{};
		SIZE _size{-1, -1};
		Lay _layout = Lay::hold_hold;
		DWORD _style = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE | TBS_AUTOTICKS | TBS_HORZ;
		DWORD _styleEx = WS_EX_LEFT;
		std::pair<int, int> _range{0, 100};
		int _page = 0;
		int _value = 0;
		friend Trackbar;
	};

	/** Options to create a `TreeView` programmatically. */
	struct TreeViewOpts final {
		/** Constructor. */
		constexpr explicit TreeViewOpts(IWindowParent &owner) : _owner{owner} { }
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		TreeViewOpts ctrl_id(WORD id) { _ctrlId = id; return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::TreeView{
		///     wl::TreeViewOpts{wnd}
		///         .pos(wl::dpi::pt(10, 10))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		TreeViewOpts pos(POINT p) { _pos = p; return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::TreeView{
		///     wl::TreeViewOpts{wnd}
		///         .pos(wl::dpi::x(10) wl::dpi::y(10))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		TreeViewOpts pos(int x, int y) { return pos({x, y}); }
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::TreeView{
		///     wl::TreeViewOpts{wnd}
		///         .size(wl::dpi::sz(120, 120))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		TreeViewOpts size(SIZE p) { _size = p; return std::move(*this); }
		/// Control size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::TreeView{
		///     wl::TreeViewOpts{wnd}
		///         .size(wl::dpi::x(120), wl::dpi::y(120))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		TreeViewOpts size(int cx, int cy) { return size({cx, cy}); }
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		TreeViewOpts layout(Lay l) { _layout = l; return std::move(*this); }
		/// The [window] and [TreeView style] passed to [`CreateWindowEx`].
		///
		/// [window]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [TreeView style]: https://learn.microsoft.com/en-us/windows/win32/controls/tree-view-control-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		TreeViewOpts style(DWORD s) { _style = s; return std::move(*this); }
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		TreeViewOpts style_ex(DWORD sx) { _styleEx = sx; return std::move(*this); }
		/// The [TreeView extended styles] applied right after the control is
		/// created.
		///
		/// [TreeView extended styles]: https://learn.microsoft.com/en-us/windows/win32/controls/tree-view-control-window-extended-styles
		TreeViewOpts style_ex_lv(DWORD sxlv) { _styleExTv = sxlv; return std::move(*this); }
		/// Loads the 16x16 icon of the given file extension into the `TreeView`.
		///
		/// They can be later referenced by index, following the same order they
		/// are added.
		TreeViewOpts icon_ext(WStrView fileExt);
		/// Loads the 16x16 icon of the given resource ID into the `TreeView`.
		///
		/// They can be later referenced by index, following the same order they
		/// are added.
		TreeViewOpts icon_id(WORD id);

	private:
		IWindowParent &_owner;
		WORD _ctrlId = 0;
		POINT _pos{};
		SIZE _size{-1, -1};
		Lay _layout = Lay::hold_hold;
		DWORD _style = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS | TVS_HASBUTTONS;
		DWORD _styleEx = WS_EX_LEFT | WS_EX_CLIENTEDGE;
		DWORD _styleExTv = 0;
		std::vector<_wl_internal::IconToLoad> _icons16{};
		friend TreeView;
	};

}

namespace wl {

	/// @brief Native [button] control.
	///
	/// Example of creating a window with a button programmatically, .h and .cpp
	/// files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{
	///         wl::WindowMainOpts{}
	///             .title(L"My main window")
	///     };
	///     wl::Button btn{
	///         wl::ButtonOpts{wnd}
	///             .text(L"&Click me")
	///             .pos(wl::dpi::pt(10, 10))
	///     };
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
	///     btn.on().bn_clicked([this]() -> void {
	///         wl::msg_box::info(wnd, L"Button clicked.");
	///     });
	/// }
	/// ```
	///
	/// Example of creating a window with a button from a dialog resource, .h and
	/// .cpp files:
	///
	/// ```cpp
	/// #define ICO_MAIN  101
	/// #define DLG_MAIN  1000
	/// #define BTN_HELLO 1001
	///
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
	///         wl::msg_box::info(wnd, L"Button clicked.");
	///     });
	/// }
	/// ```
	///
	/// [button]: https://learn.microsoft.com/en-us/windows/win32/controls/button-types-and-styles#push-buttons
	class Button final : public IWindowChild {
	public:
		/// Constructs the button, which will be created programmatically with
		/// [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit Button(ButtonOpts creationOpts);

		/// Constructs the button, which will be loaded from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog
		/// resource.
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
		/// Note that subclassing is a potentially slow technique, prefer using
		/// ordinary events.
		///
		/// [Subclasses]: https://learn.microsoft.com/en-us/windows/win32/controls/subclassing-overview
		[[nodiscard]] constexpr events::WindowEvents& subclass_on() { return _wl_internal::valid_event(hwnd(), _ctrl._subclassEvents); }

		/// Focus the control by posting a [`WM_NEXTDLGCTL`] message, which
		/// is better than calling [`SetFocus`].
		///
		/// [`WM_NEXTDLGCTL`]: https://learn.microsoft.com/en-us/windows/win32/dlgbox/wm-nextdlgctl
		/// [`SetFocus`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setfocus
		const Button& focus() const { _wl_internal::focus(hwnd()); return *this; }

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
	/// Example of creating a window with a check box programmatically, .h and
	/// .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{
	///         wl::WindowMainOpts{}
	///             .title(L"My main window")
	///     };
	///     wl::CheckBox chk{
	///         wl::CheckBoxOpts{wnd}
	///             .text(L"&Check me")
	///             .pos(wl::dpi::pt(10, 10))
	///             .checked(true)
	///     };
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
		/// Constructs the check box, which will be created programmatically with
		/// [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit CheckBox(CheckBoxOpts creationOpts);

		/// Constructs the check box, which will be loaded from the dialog
		/// resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog
		/// resource.
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
		/// Note that subclassing is a potentially slow technique, prefer using
		/// ordinary events.
		///
		/// [Subclasses]: https://learn.microsoft.com/en-us/windows/win32/controls/subclassing-overview
		[[nodiscard]] constexpr events::WindowEvents& subclass_on() { return _wl_internal::valid_event(hwnd(), _ctrl._subclassEvents); }

		/// Sends [`BM_GETCHECK`] and returns true if current state flag is
		/// `BST_CHECKED`.
		///
		/// [`BM_GETCHECK`]: https://learn.microsoft.com/en-us/windows/win32/controls/bm-getcheck
		[[nodiscard]] bool is_checked() const { return state() == BST_CHECKED; }

		/// Sends [`BM_SETCHECK`] to set the current state flag to `BST_CHECKED`
		/// or `BST_UNCHECKED`.
		///
		/// [`BM_SETCHECK`]: https://learn.microsoft.com/en-us/windows/win32/controls/bm-setcheck
		const CheckBox& set_check(bool doCheck) const { return set_state(doCheck ? BST_CHECKED : BST_UNCHECKED); }

		/// Focus the control by posting a [`WM_NEXTDLGCTL`] message, which
		/// is better than calling [`SetFocus`].
		///
		/// [`WM_NEXTDLGCTL`]: https://learn.microsoft.com/en-us/windows/win32/dlgbox/wm-nextdlgctl
		/// [`SetFocus`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setfocus
		const CheckBox& focus() const { _wl_internal::focus(hwnd()); return *this; }

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

		/// Calls [`SetWindowText`] to set the button text, then resizes the check
		/// box to fit the text exactly.
		///
		/// [`SetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowtextw
		const CheckBox& set_text_resize(WStrView newText) const;

	private:
		_wl_internal::NativeCtrlBase _ctrl;
		events::ButtonEvents _events;
	};

	/// @brief Native [combo box] control (dropdown).
	///
	/// Example of creating a window with a combo box programmatically, .h and
	/// .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{
	///         wl::WindowMainOpts{}
	///             .title(L"My main window")
	///     };
	///     wl::ComboBox cmb{
	///         wl::ComboBoxOpts{wnd}
	///             .texts({L"Hello", L"World"})
	///             .pos(wl::dpi::pt(10, 10))
	///     };
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
		class ItemCollection final : private wl::NoCopyNoMove {
		private:
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

		/// Constructs the combo box, which will be created programmatically with
		/// [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit ComboBox(ComboBoxOpts creationOpts);

		/// Constructs the combo box, which will be loaded from the dialog
		/// resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog
		/// resource.
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
		/// Note that subclassing is a potentially slow technique, prefer using
		/// ordinary events.
		///
		/// [Subclasses]: https://learn.microsoft.com/en-us/windows/win32/controls/subclassing-overview
		[[nodiscard]] constexpr events::WindowEvents& subclass_on() { return _wl_internal::valid_event(hwnd(), _ctrl._subclassEvents); }

		/// Focus the control by posting a [`WM_NEXTDLGCTL`] message, which
		/// is better than calling [`SetFocus`].
		///
		/// [`WM_NEXTDLGCTL`]: https://learn.microsoft.com/en-us/windows/win32/dlgbox/wm-nextdlgctl
		/// [`SetFocus`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setfocus
		const ComboBox& focus() const { _wl_internal::focus(hwnd()); return *this; }

	private:
		_wl_internal::NativeCtrlBase _ctrl;
		events::ComboBoxEvents _events;
	};

	/// @brief Native [date and time picker] control.
	///
	/// Example of creating a window with a date and time picker programmatically,
	/// .h and .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{
	///         wl::WindowMainOpts{}
	///             .title(L"My main window")
	///     };
	///     wl::DateTimePicker dtp{
	///         wl::DateTimePickerOpts{wnd}
	///             .pos(wl::dpi::pt(10, 10))
	///     };
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
	///     dtp.on().dtn_date_time_change([this](NMDATETIMECHANGE &p) -> void {
	///         wl::Time t{wl::Time::LocalSt{p.st}};
	///         wnd.set_title(t.to_str_local_ymd());
	///     });
	/// }
	/// ```
	///
	/// [date and time picker]: https://learn.microsoft.com/en-us/windows/win32/controls/date-and-time-picker-controls
	class DateTimePicker final : public IWindowChild {
	public:
		/// Constructs the date and time picker, which will be created
		/// programmatically with [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit DateTimePicker(DateTimePickerOpts creationOpts);

		/// Constructs the date and time picker, which will be loaded from the
		/// dialog resource.
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
		/// Note that subclassing is a potentially slow technique, prefer using
		/// ordinary events.
		///
		/// [Subclasses]: https://learn.microsoft.com/en-us/windows/win32/controls/subclassing-overview
		[[nodiscard]] constexpr events::WindowEvents& subclass_on() { return _wl_internal::valid_event(hwnd(), _ctrl._subclassEvents); }

		/// Focus the control by posting a [`WM_NEXTDLGCTL`] message, which
		/// is better than calling [`SetFocus`].
		///
		/// [`WM_NEXTDLGCTL`]: https://learn.microsoft.com/en-us/windows/win32/dlgbox/wm-nextdlgctl
		/// [`SetFocus`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setfocus
		const DateTimePicker& focus() const { _wl_internal::focus(hwnd()); return *this; }

		/** Returns the current value. */
		[[nodiscard]] Time value() const;

		/** Sets the current value. */
		const DateTimePicker& set_value(Time t) const;

	private:
		_wl_internal::NativeCtrlBase _ctrl;
		events::DateTimePickerEvents _events;
	};

	/// @brief Native [edit] control (textbox).
	///
	/// Example of creating a window with an edit programmatically, .h and .cpp
	/// files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{
	///         wl::WindowMainOpts{}
	///             .title(L"My main window")
	///     };
	///     wl::Edit txt{
	///         wl::EditOpts{wnd}
	///             .pos(wl::dpi::pt(10, 10))
	///     };
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
	/// Example of creating a window with an edit from a dialog resource, .h and
	/// .cpp files:
	///
	/// ```cpp
	/// #define ICO_MAIN  101
	/// #define DLG_MAIN  1000
	/// #define TXT_HELLO 1001
	///
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
		/// Constructs the edit, which will be created programmatically with
		/// [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit Edit(EditOpts creationOpts);

		/// Constructs the edit, which will be loaded from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog
		/// resource.
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
		/// Note that subclassing is a potentially slow technique, prefer using
		/// ordinary events.
		///
		/// [Subclasses]: https://learn.microsoft.com/en-us/windows/win32/controls/subclassing-overview
		[[nodiscard]] constexpr events::WindowEvents& subclass_on() { return _wl_internal::valid_event(hwnd(), _ctrl._subclassEvents); }

		/// Focus the control by posting a [`WM_NEXTDLGCTL`] message, which
		/// is better than calling [`SetFocus`].
		///
		/// [`WM_NEXTDLGCTL`]: https://learn.microsoft.com/en-us/windows/win32/dlgbox/wm-nextdlgctl
		/// [`SetFocus`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setfocus
		const Edit& focus() const { _wl_internal::focus(hwnd()); return *this; }

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
	/// Example of creating a window with a list view programmatically, .h and
	/// .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{
	///         wl::WindowMainOpts{}
	///             .title(L"My main window")
	///     };
	///     wl::ListView lv{
	///         wl::ListViewOpts{wnd}
	///             .pos(wl::dpi::pt(10, 80))
	///             .size(wl::dpi::sz(400, 100))
	///             .col(L"First", wl::dpi::x(200))
	///             .col(L"Second", 1)
	///             .icon16_ext(L"docx")
	///             .icon16_ext(L"zip")
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
	/// Example of creating a window with a list view from a dialog resource, .h
	/// and .cpp files:
	///
	/// ```cpp
	/// #define ICO_MAIN  101
	/// #define DLG_MAIN  1000
	/// #define LST_FILES 1001
	///
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
		class ColumnCollection final : private wl::NoCopyNoMove {
		private:
			constexpr explicit ColumnCollection(const ListView &owner) : _owner{owner} { }

		public:
			/// Returns the column at the given index.
			///
			/// Note that no bounds checking is performed. This method will simply
			/// return a `Column` holding the provided index.
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

		/// @brief A single item of the `ListView`.
		///
		/// This is a lightweight wrapper which performs no validation. The item
		/// may have the index `-1`, which represents an invalid item.
		class Item final {
		public:
			/** Constructs an item for the given `ListView` and zero-based index. */
			constexpr Item(const ListView &owner, int itemIndex) : _owner{owner}, _index{itemIndex} { }

			/// Returns the zero-based index of the item.
			///
			/// Note that, if the position of the item changes in the list, the
			/// index will also change. If you need to uniquely identify the item,
			/// regardless of its position, use `unique_id`.
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
			[[nodiscard]] std::wstring text_of(UINT columnIndex) const;

			/// Sets the text under a column for the item.
			///
			/// Example:
			///
			/// ```cpp
			/// lv.items[0].set_text_of(0, L"New text");
			/// ```
			const Item& set_text_of(UINT columnIndex, WStrView newText) const;

			/// Sets all the texts under all columns, at once.
			///
			/// Example:
			///
			/// ```cpp
			/// lv.items[0].set_texts({L"First col", L"Second col"});
			/// ```
			const Item& set_texts(std::initializer_list<WStrView> texts) const;

			/// Calls [`ListView_MapIndexToID`] to retrieve the unique ID of the
			/// item.
			///
			/// Differently from the item index, this ID is guaranteed to remain
			/// the same for the whole lifetime of the list view.
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
		class ItemCollection final : private wl::NoCopyNoMove {
		private:
			constexpr explicit ItemCollection(const ListView &owner) : _owner{owner} { }

		public:
			/// Returns the item at the given index.
			///
			/// Note that no bounds checking is performed. This method will simply
			/// return an `Item` holding the provided index.
			[[nodiscard]] constexpr Item operator[](int index) const { return Item{_owner, index}; }

			/// Adds a new item, defining the text for the first column.
			/// Optionally, you can provide texts for the subsequent columns.
			///
			/// The optional `iconIndex` refers to the zero-based index of an icon
			/// previusly added to one of the image lists.
			///
			/// Example:
			///
			/// ```cpp
			/// lv.items.add(L"My item", {L"Column 2", L"Column 3"});
			/// ```
			Item add(WStrView text, std::initializer_list<WStrView> otherColumnsTexts = {}, int iconIndex = -1) const;

			/// Returns the item count.
			///
			/// Example iterating over all items:
			///
			/// ```cpp
			/// for (int i = 0; i < lv.items.count(); ++i) {
			///     wl::ListView::Item curItem = lv.items[i];
			///     // ...
			/// }
			/// ```
			[[nodiscard]] size_t count() const;

			/** Deletes all items. */
			void delete_all() const;

			/** Deletes the selected items. */
			void delete_selected() const;

			/** Returns the focused item, if any. */
			[[nodiscard]] std::optional<Item> focused() const;

			/// Calls [`ListView_MapIDToIndex`] to return the item with the given
			/// unique ID.
			///
			/// Differently from the item index, this ID is guaranteed to remain
			/// the same for the whole lifetime of the list view.
			///
			/// [`ListView_MapIDToIndex`]: https://learn.microsoft.com/en-us/windows/win32/api/commctrl/nf-commctrl-listview_mapidtoindex
			[[nodiscard]] std::optional<Item> get_by_unique_id(UINT uid) const;

			/// Calls [`ListView_HitTestEx`] to return the item at the given
			/// position, relative to the list view's client area, if any.
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

			/// Calls [`ListView_SortItemsEx`] to sort the items according to the
			/// callback.
			///
			/// Example sorting the items by the text of the 3rd column:
			///
			/// ```cpp
			/// lv.items.sort([](wl::ListView::Item a, wl::ListView::Item b) -> int {
			///     return wl::str::cmp_i(a.text_of(2), b.text_of(2));
			/// });
			/// ```
			///
			/// [`ListView_SortItemsEx`]: https://learn.microsoft.com/en-us/windows/win32/api/commctrl/nf-commctrl-listview_sortitemsex
			void sort(std::function<int(Item, Item)> cb) const;

			/// Calls [`ListView_GetTopIndex`] to return the topmost visible item,
			/// if any.
			///
			/// [`ListView_GetTopIndex`]: https://learn.microsoft.com/en-us/windows/win32/api/commctrl/nf-commctrl-listview_gettopindex
			[[nodiscard]] std::optional<Item> topmost_visible() const;

		private:
			const ListView &_owner;
			friend ListView;
		};

		/// Constructs the list view, which will be created programmatically with
		/// [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit ListView(ListViewOpts creationOpts);

		/// Constructs the list view, which will be loaded from the dialog
		/// resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog
		/// resource.
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
		/// Note that subclassing is a potentially slow technique, prefer using
		/// ordinary events.
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

		/// Focus the control by posting a [`WM_NEXTDLGCTL`] message, which
		/// is better than calling [`SetFocus`].
		///
		/// [`WM_NEXTDLGCTL`]: https://learn.microsoft.com/en-us/windows/win32/dlgbox/wm-nextdlgctl
		/// [`SetFocus`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setfocus
		const ListView& focus() const { _wl_internal::focus(hwnd()); return *this; }

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
		/// Constructs the month calendar, which will be created programmatically
		/// with [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit MonthCalendar(MonthCalendarOpts creationOpts);

		/// Constructs the month calendar, which will be loaded from the dialog
		/// resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog
		/// resource.
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
		/// Note that subclassing is a potentially slow technique, prefer using
		/// ordinary events.
		///
		/// [Subclasses]: https://learn.microsoft.com/en-us/windows/win32/controls/subclassing-overview
		[[nodiscard]] constexpr events::WindowEvents& subclass_on() { return _wl_internal::valid_event(hwnd(), _ctrl._subclassEvents); }

		/// Focus the control by posting a [`WM_NEXTDLGCTL`] message, which
		/// is better than calling [`SetFocus`].
		///
		/// [`WM_NEXTDLGCTL`]: https://learn.microsoft.com/en-us/windows/win32/dlgbox/wm-nextdlgctl
		/// [`SetFocus`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setfocus
		const MonthCalendar& focus() const { _wl_internal::focus(hwnd()); return *this; }

		/** Returns the current value. */
		[[nodiscard]] Time value() const;

		/** Sets the current value. */
		const MonthCalendar& set_value(Time t) const;

	private:
		_wl_internal::NativeCtrlBase _ctrl;
		events::MonthCalendarEvents _events;
	};

	/// @brief Native [radio button] control.
	///
	/// Instead of creating the radio buttons directly, prefer using a
	/// `RadioGroup`.
	///
	/// [radio button]: https://learn.microsoft.com/en-us/windows/win32/controls/button-types-and-styles#radio-buttons
	class RadioButton final : public IWindowChild {
	public:
		/// Constructs the radio button, which will be created programmatically
		/// with [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit RadioButton(RadioButtonOpts creationOpts);

		/// Constructs the radio button, which will be loaded from the dialog
		/// resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog
		/// resource.
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
		/// Note that subclassing is a potentially slow technique, prefer using
		/// ordinary events.
		///
		/// [Subclasses]: https://learn.microsoft.com/en-us/windows/win32/controls/subclassing-overview
		[[nodiscard]] constexpr events::WindowEvents& subclass_on() { return _wl_internal::valid_event(hwnd(), _ctrl._subclassEvents); }

		/// Focus the control by posting a [`WM_NEXTDLGCTL`] message, which
		/// is better than calling [`SetFocus`].
		///
		/// [`WM_NEXTDLGCTL`]: https://learn.microsoft.com/en-us/windows/win32/dlgbox/wm-nextdlgctl
		/// [`SetFocus`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setfocus
		const RadioButton& focus() const { _wl_internal::focus(hwnd()); return *this; }

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

		/// Calls [`SetWindowText`] to set the button text, then resizes the check
		/// box to fit the text exactly.
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
	/// Example of creating a window with 3 radio buttons programmatically, .h
	/// and .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{
	///         wl::WindowMainOpts{}
	///             .title(L"My main window")
	///     };
	///     wl::RadioGroup rads{
	///         wl::RadioGroupOpts{wnd}
	///             .radio()
	///                 .text(L"First")
	///                 .pos(wl::dpi::pt(420, 10))
	///             .radio()
	///                 .text(L"Second")
	///                 .pos(wl::dpi::pt(420, 25))
	///                 .select()
	///             .radio()
	///                 .text(L"Third")
	///                 .pos(wl::dpi::pt(420, 40))
	///     };
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
	///     rads.on().bn_clicked([this](int i) -> void {
	///         wl::msg_box::info(wnd, L"Radio selected", rads.radios[i].text());
	///     });
	/// }
	/// ```
	class RadioGroup final {
	public:
		/** Accesses the managed `RadioButton` controls. */
		class RadioButtonCollection final : private wl::NoCopyNoMove {
		private:
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

		/// Constructs the grouped radio buttons, which will be created
		/// programmatically with [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit RadioGroup(RadioGroupOpts creationOpts);

		/// Constructs the radio group using each provided ID to programmatically
		/// create a radio button which will be loaded from the dialog resource.
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
		_wl_internal::NonMovableArray<RadioButton> _radios;
		events::RadioGroupEvents _events;
	};

	/// @brief Native [static] control (label).
	///
	/// Example of creating a window with a static programmatically, .h and .cpp
	/// files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{
	///         wl::WindowMainOpts{}
	///             .title(L"My main window")
	///     };
	///     wl::Static lbl{
	///         wl::StaticOpts{wnd}
	///             .text(L"Hello")
	///             .pos(wl::dpi::pt(10, 10))
	///     };
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
		/// Constructs the static, which will be created programmatically with
		/// [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit Static(StaticOpts creationOpts);

		/// Constructs the static, which will be loaded from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog
		/// resource.
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
		/// Note that subclassing is a potentially slow technique, prefer using
		/// ordinary events.
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

		/// Calls [`SetWindowText`] to set the text, then resizes the control to
		/// fit the text exactly.
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
	///     wl::WindowMain wnd{
	///         wl::WindowMainOpts{}
	///             .title(L"My main window")
	///     };
	///     wl::StatusBar sb{
	///         wl::StatusBarOpts{wnd}
	///             .icon_ext(L"xlsx")
	///             .icon_ext(L"zip")
	///             .part_flex(1, L"Here", 0)
	///             .part_fixed(wl::dpi::x(200), L"Hello", 1)
	///     };
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
	///     sb.on().nm_click([this](NMMOUSE &p) -> bool {
	///         wl::msg_box::info(wnd, L"Status bar clicked.");
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
		class PartCollection final : private wl::NoCopyNoMove {
		private:
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

		/// Constructs the status bar, which will be created programmatically with
		/// [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit StatusBar(StatusBarOpts creationOpts);

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
		/// Note that subclassing is a potentially slow technique, prefer using
		/// ordinary events.
		///
		/// [Subclasses]: https://learn.microsoft.com/en-us/windows/win32/controls/subclassing-overview
		[[nodiscard]] constexpr events::WindowEvents& subclass_on() { return _wl_internal::valid_event(hwnd(), _ctrl._subclassEvents); }

	private:
		void resize_to_fit_parent(wm::Size p);
		_wl_internal::NativeCtrlBase _ctrl;
		events::StatusBarEvents _events;
		std::vector<StatusBarOpts::Part> _parts;
		std::vector<int> _rightEdges{}; // buffer to speed up resize calls
		_wl_internal::HIconStore _iconStore16{{16, 16}};
	};

	/// @brief Native [syslink] control.
	///
	/// Example of creating a window with a static programmatically, .h and .cpp
	/// files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{
	///         wl::WindowMainOpts{}
	///             .title(L"My main window")
	///     };
	///     wl::SysLink lnk{
	///         wl::SysLinkOpts{wnd}
	///             .text(L"Link <a href=\"https://google.com\">here</a>")
	///             .pos(wl::dpi::pt(10, 10))
	///     };
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
	///     lnk.on().nm_click([this](NMLINK &p) -> void {
	///         wnd.set_title(L"Link clicked");
	///     });
	/// }
	/// ```
	///
	/// [syslink]: https://learn.microsoft.com/en-us/windows/win32/controls/syslink-control-entry
	class SysLink final : public IWindowChild {
	public:
		/// Constructs the static, which will be created programmatically with [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit SysLink(SysLinkOpts creationOpts);

		/// Constructs the syslink, which will be loaded from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog
		/// resource.
		SysLink(IWindowParent &owner, WORD ctrlId, Lay layout);

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
		/// lnk.on().nm_click([](NMLINK &p) -> void {
		///     // ...
		/// });
		/// ```
		[[nodiscard]] constexpr events::SysLinkEvents& on() { return _wl_internal::valid_event(hwnd(), _events); }

		/// [Subclasses] the control allowing message events to be added.
		///
		/// The events must be added before the control is created on the screen.
		///
		/// Note that subclassing is a potentially slow technique, prefer using
		/// ordinary events.
		///
		/// [Subclasses]: https://learn.microsoft.com/en-us/windows/win32/controls/subclassing-overview
		[[nodiscard]] constexpr events::WindowEvents& subclass_on() { return _wl_internal::valid_event(hwnd(), _ctrl._subclassEvents); }

		/// Focus the control by posting a [`WM_NEXTDLGCTL`] message, which
		/// is better than calling [`SetFocus`].
		///
		/// [`WM_NEXTDLGCTL`]: https://learn.microsoft.com/en-us/windows/win32/dlgbox/wm-nextdlgctl
		/// [`SetFocus`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setfocus
		const SysLink& focus() const { _wl_internal::focus(hwnd()); return *this; }

		/// Calls [`GetWindowText`] to return the control text.
		///
		/// [`GetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowtextw
		[[nodiscard]] std::wstring text() const { return _wl_internal::wnd_text(hwnd()); }

		/// Calls [`SetWindowText`] to set the control text.
		///
		/// [`SetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowtextw
		const SysLink& set_text(WStrView newText) const;

		/// Calls [`SetWindowText`] to set the text, then resizes the control to
		/// fit the text exactly.
		///
		/// [`SetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowtextw
		const SysLink& set_text_resize(WStrView newText) const;

	private:
		static std::wstring remove_html_anchor(WStrView text);
		_wl_internal::NativeCtrlBase _ctrl;
		events::SysLinkEvents _events;
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
	///     wl::WindowMain wnd{
	///         wl::WindowMainOpts{}
	///             .title(L"My main window")
	///     };
	///     wl::Tab tab{
	///         wl::TabOpts{
	///             .titles({L"First", L"Second"})
	///             .select(1)
	///             .pos(wl::dpi::pt(656, 10))
	///             .size(wl::dpi::sz(220, 200))
	///             .layout(wl::Lay::move_hold)
	///     };
	///     wl::Button btn{
	///         wl::ButtonOpts{tab.items[0].child()}
	///             .text(L"&Click")
	///             .pos(wl::dpi::pt(50, 40)) // button position inside the 1st tab
	///     };
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
	///     btn.on().bn_clicked([this]() -> void {
	///         wl::msg_box::info(wnd, L"Button clicked.");
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
		class ItemCollection final : private wl::NoCopyNoMove {
		private:
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

		/// Constructs the tab, which will be created programmatically with
		/// [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit Tab(TabOpts creationOpts);

		/// Constructs the tab, which will be loaded from the dialog resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog
		/// resource.
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
		/// Note that subclassing is a potentially slow technique, prefer using
		/// ordinary events.
		///
		/// [Subclasses]: https://learn.microsoft.com/en-us/windows/win32/controls/subclassing-overview
		[[nodiscard]] constexpr events::WindowEvents& subclass_on() { return _wl_internal::valid_event(hwnd(), _ctrl._subclassEvents); }

		/// Sets one or more [extended styles].
		///
		/// [extended styles]: https://learn.microsoft.com/en-us/windows/win32/controls/tree-view-control-window-extended-styles
		const Tab& set_extended_style(bool doSet, DWORD exStyle) const;

		/// Focus the control by posting a [`WM_NEXTDLGCTL`] message, which
		/// is better than calling [`SetFocus`].
		///
		/// [`WM_NEXTDLGCTL`]: https://learn.microsoft.com/en-us/windows/win32/dlgbox/wm-nextdlgctl
		/// [`SetFocus`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setfocus
		const Tab& focus() const { _wl_internal::focus(hwnd()); return *this; }

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
	/// Example of creating a window with a trackbar programmatically, .h and
	/// .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{
	///         wl::WindowMainOpts{}
	///             .title(L"My main window")
	///     };
	///     wl::Trackbar tb{
	///         wl::TrackbarOpts{wnd}
	///             .pos(wl::dpi::pt(10, 10))
	///             .range(0, 8)
	///             .value(6)
	///     };
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
		/// Constructs the tree view, which will be created programmatically with
		/// [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit Trackbar(TrackbarOpts creationOpts);

		/// Constructs the tree view, which will be loaded from the dialog
		/// resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog
		/// resource.
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
		/// Note that subclassing is a potentially slow technique, prefer using
		/// ordinary events.
		///
		/// [Subclasses]: https://learn.microsoft.com/en-us/windows/win32/controls/subclassing-overview
		[[nodiscard]] constexpr events::WindowEvents& subclass_on() { return _wl_internal::valid_event(hwnd(), _ctrl._subclassEvents); }

		/// Focus the control by posting a [`WM_NEXTDLGCTL`] message, which
		/// is better than calling [`SetFocus`].
		///
		/// [`WM_NEXTDLGCTL`]: https://learn.microsoft.com/en-us/windows/win32/dlgbox/wm-nextdlgctl
		/// [`SetFocus`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setfocus
		const Trackbar& focus() const { _wl_internal::focus(hwnd()); return *this; }

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
	/// Example of creating a window with a tree view programmatically, .h and
	/// .cpp files:
	///
	/// ```cpp
	/// class MyMain final {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{
	///         wl::WindowMainOpts{}
	///             .title(L"My main window")
	///     };
	///     wl::TreeView tv{
	///         wl::TreeViewOpts{wnd}
	///             .pos(wl::dpi::pt(10, 190))
	///             .size(wl::dpi::sz(250, 90))
	///             .icon_ext(L"docx")
	///     };
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
	///     wnd.on().wm_create([this](wl::wm::Create p) -> int {
	///         tv.roots.add(L"First root", 0);
	///         tv.roots.add(L"Second root", 0);
	///
	///         tv.roots[0].children.add(L"Child", 0);
	///         tv.roots[0].expand(true);
	///
	///         return 0;
	///     });
	///
	///     tv.on().tvn_sel_changed([this](NMTREEVIEW &p) -> void {
	///         wl::TreeView::Item sel = tv.items.selected();
	///         wnd.set_title(sel.hitem() ? sel.text() : L"No tree sel");
	///     });
	/// }
	/// ```
	///
	/// [tree view]: https://learn.microsoft.com/en-us/windows/win32/controls/tree-view-controls
	class TreeView final : public IWindowChild {
	public:
		class Item;

		/** @brief Operations over the child items. */
		class ChildCollection final : private wl::NoCopyNoMove {
		private:
			constexpr ChildCollection(const TreeView &owner, HTREEITEM hItem)
				: _pOwner{&owner}, _hItem{hItem} { }

		public:
			/// Retrieves the element at the given zero-based index.
			///
			/// This method calls [`TreeView_GetNextSibling`] up to the desired
			/// item, so if there are too many, it can be potentially slow.
			///
			/// If the index is out-of-bounds, the returned `Item` will have a
			/// `nullptr` handle.
			///
			/// [`TreeView_GetNextSibling`]: https://learn.microsoft.com/en-us/windows/win32/api/commctrl/nf-commctrl-treeview_getnextsibling
			[[nodiscard]] Item operator[](size_t index) const;

			/// Adds a new child item, defining its text.
			///
			/// The optional iconIndex refers to the zero-based index of an icon
			/// previusly added to one of the image lists.
			Item add(WStrView text, int iconIndex = -1) const;

			/// Counts the number child items, not recursively.
			///
			/// This method calls [`TreeView_GetNextSibling`] up to the last item,
			/// so if there are too many, it can be potentially slow.
			///
			/// [`TreeView_GetNextSibling`]: https://learn.microsoft.com/en-us/windows/win32/api/commctrl/nf-commctrl-treeview_getnextsibling
			[[nodiscard]] size_t count() const;

		private:
			const TreeView *_pOwner;
			HTREEITEM _hItem;
			friend TreeView;
		};

		/// @brief A single item of the `TreeView`.
		///
		/// This is a lightweight wrapper which performs no validation. The item
		/// may have the handle `nullptr`, which represents an invalid item.
		class Item final {
		public:
			/** Copy-constructor. */
			constexpr Item(const Item &other)
				: children{*other.children._pOwner, other.children._hItem} { }

			/** Copy-assignment operator. */
			constexpr Item& operator=(const Item &other) {
				children._pOwner = other.children._pOwner;
				children._hItem = other.children._hItem;
				return *this;
			}

			/** Constructs an item for the given `TreeView` and hItem. */
			constexpr Item(const TreeView &owner, HTREEITEM hItem) : children{owner, hItem} { }

			/** Child item methods. */
			ChildCollection children;

			/** Returns the item handle, which uniquely identifies the item. */
			[[nodiscard]] constexpr HTREEITEM hitem() const { return children._hItem; }

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

			/// Returns the zero-based index of the item within its siblings.
			///
			/// This method calls [`TreeView_GetPrevSibling`] up to the desired
			/// item, so if there are too many, it can be potentially slow.
			///
			/// [`TreeView_GetPrevSibling`]: https://learn.microsoft.com/en-us/windows/win32/api/commctrl/nf-commctrl-treeview_getprevsibling
			[[nodiscard]] size_t index() const;

			/// Retrieves the next sibling.
			///
			/// If none, the returned `Item` will have a `nullptr` handle.
			[[nodiscard]] Item next_sibling() const;

			/// Retrieves the parent item.
			///
			/// If a root, the returned `Item` will have a `nullptr` handle.
			[[nodiscard]] Item parent() const;

			/// Retrieves the previous sibling.
			///
			/// If none, the returned `Item` will have a `nullptr` handle.
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
		};

		/** @brief Operations over all items. */
		class ItemCollection final : private wl::NoCopyNoMove {
		private:
			constexpr explicit ItemCollection(const TreeView &owner) : _owner{owner} { }

		public:
			/// Returns the item with the given handle.
			///
			/// Note that no checking is performed. This method will simply return
			/// an `Item` holding the provided handle.
			[[nodiscard]] constexpr Item by_hitem(HTREEITEM hItem) const { return {_owner, hItem}; }

			/** Retrieves the total number of items. */
			[[nodiscard]] size_t count() const;

			/// Retrieves the first visible item.
			///
			/// If none, the returned `Item` will have a `nullptr` handle.
			[[nodiscard]] Item first_visible() const;

			/// Retrieves the last visible item.
			///
			/// If none, the returned `Item` will have a `nullptr` handle.
			[[nodiscard]] Item last_visible() const;

			/// Retrieves the currently selected item.
			///
			/// If none, the returned `Item` will have a `nullptr` handle.
			[[nodiscard]] Item selected() const;

		private:
			const TreeView &_owner;
			friend TreeView;
		};

		/// Constructs the tree view, which will be created programmatically with
		/// [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		explicit TreeView(TreeViewOpts creationOpts);

		/// Constructs the tree view, which will be loaded from the dialog
		/// resource.
		///
		/// The `ctrlId` parameter must identify the control in the dialog
		/// resource.
		TreeView(IWindowParent &owner, WORD ctrlId, Lay layout);

		/** Root item methods. */
		ChildCollection roots{*this, nullptr};

		/** All items methods. */
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
		/// Note that subclassing is a potentially slow technique, prefer using
		/// ordinary events.
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

		/// Focus the control by posting a [`WM_NEXTDLGCTL`] message, which
		/// is better than calling [`SetFocus`].
		///
		/// [`WM_NEXTDLGCTL`]: https://learn.microsoft.com/en-us/windows/win32/dlgbox/wm-nextdlgctl
		/// [`SetFocus`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setfocus
		const TreeView& focus() const { _wl_internal::focus(hwnd()); return *this; }

		/// Retrieves the 16x16 `IStoreIcon`.
		///
		/// Allows icons to be added to the control's image list.
		/// An `Item` can display an icon referring to its zero-based index.
		IStoreIcon& icons();

	private:
		_wl_internal::NativeCtrlBase _ctrl;
		events::TreeViewEvents _events;
		_wl_internal::ImageList _imgList16{{16, 16}};
	};

}
