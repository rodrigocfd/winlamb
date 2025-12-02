#pragma once
#include <functional>
#include "lib-include-win.h"
#include <CommCtrl.h>
#include "wnd-base.h"
#include "wnd-interfaces.h"

namespace _wl_internal {

	/** Base to all native control events. */
	class NativeCtrlEvents final {
	public:
		NativeCtrlEvents(NativeCtrlEvents&&) = delete; // non-copyable, non-movable

		NativeCtrlEvents(wl::WindowParent &owner, WORD ctrlId)
			: _parentWndBase{owner.wnd_base()}, _ctrlId{ctrlId} { }

		WndBase &_parentWndBase;
		WORD _ctrlId;
	};

}

namespace wl {
	class Button;
	class CheckBox;
	class ComboBox;
	class DateTimePicker;
	class Edit;
	class ListView;
	class MonthCalendar;
	class RadioButton;
	class Static;
	class StatusBar;
}

namespace wl::events {

	/** @brief Native `Button` control events. */
	class ButtonEvents final {
	private:
		ButtonEvents(ButtonEvents&&) = delete; // non-copyable, non-movable

		ButtonEvents(wl::WindowParent &owner, WORD ctrlId) : _ctrlEvents{owner, ctrlId} { }

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

	/** @brief Native `ComboBox` control events. */
	class ComboBoxEvents final {
	private:
		ComboBoxEvents(ComboBoxEvents&&) = delete; // non-copyable, non-movable

		ComboBoxEvents(wl::WindowParent &owner, WORD ctrlId) : _ctrlEvents{owner, ctrlId} { }

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

	/** @brief Native `DateTimePicker` control events. */
	class DateTimePickerEvents final {
	private:
		DateTimePickerEvents(DateTimePickerEvents&&) = delete; // non-copyable, non-movable

		DateTimePickerEvents(wl::WindowParent &owner, WORD ctrlId) : _ctrlEvents{owner, ctrlId} { }

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

	/** @brief Native `Edit` control events. */
	class EditEvents final {
	private:
		EditEvents(EditEvents&&) = delete; // non-copyable, non-movable

		EditEvents(wl::WindowParent &owner, WORD ctrlId) : _ctrlEvents{owner, ctrlId} { }

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

	/** @brief Native `ListView` events. */
	class ListViewEvents final {
	private:
		ListViewEvents(ListViewEvents&&) = delete; // non-copyable, non-movable

		ListViewEvents(wl::WindowParent &owner, WORD ctrlId) : _ctrlEvents{owner, ctrlId} { }

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

	/** @brief Native `MonthCalendar` events. */
	class MonthCalendarEvents final {
	private:
		MonthCalendarEvents(MonthCalendarEvents&&) = delete; // non-copyable, non-movable

		MonthCalendarEvents(wl::WindowParent &owner, WORD ctrlId) : _ctrlEvents{owner, ctrlId} { }

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

	/** @brief Native `Static` events. */
	class StaticEvents final {
	private:
		StaticEvents(StaticEvents&&) = delete; // non-copyable, non-movable

		StaticEvents(wl::WindowParent &owner, WORD ctrlId) : _ctrlEvents{owner, ctrlId} { }

	public:
		void stn_clicked(std::function<void()> &&cb);
		void stn_dbl_clk(std::function<void()> &&cb);
		void stn_disable(std::function<void()> &&cb);
		void stn_enable(std::function<void()> &&cb);

	private:
		_wl_internal::NativeCtrlEvents _ctrlEvents;
		friend wl::Static;
	};

	/** @brief Native `StatusBar` events. */
	class StatusBarEvents final {
	private:
		StatusBarEvents(StatusBarEvents&&) = delete; // non-copyable, non-movable

		StatusBarEvents(wl::WindowParent &owner, WORD ctrlId) : _ctrlEvents{owner, ctrlId} { }

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

}
