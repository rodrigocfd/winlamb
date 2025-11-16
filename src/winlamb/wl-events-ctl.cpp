#include "events-ctl.h"
using namespace wl;
using namespace wl::events;
using namespace _wl_internal;

#define EVENT_CLS
#define EVENT_CMD(method, cmd) \
	void EVENT_CLS::method(std::function<void()> &&cb) { \
		_ctrlEvents._parentWndBase._userEvents.wm_command(_ctrlEvents._ctrlId, cmd, std::move(cb)); \
	}
#define EVENT_NFY(method, nm) \
	void EVENT_CLS::method(std::function<void()> &&cb) { \
		_ctrlEvents._parentWndBase._userEvents.wm_notify(_ctrlEvents._ctrlId, nm, \
			[this, cb = std::move(cb)](wm::Notify) -> LRESULT { \
				cb(); \
				return _ctrlEvents._parentWndBase._isDlg ? TRUE : 0; \
			}); \
	}
#define EVENT_NFY_ARG(method, nm, argty) \
	void EVENT_CLS::method(std::function<void(argty&)> &&cb) { \
		_ctrlEvents._parentWndBase._userEvents.wm_notify(_ctrlEvents._ctrlId, nm, \
			[this, cb = std::move(cb)](wm::Notify p) -> LRESULT { \
				cb(p.hdr<argty>()); \
				return _ctrlEvents._parentWndBase._isDlg ? TRUE : 0; \
			}); \
	}
#define EVENT_NFY_ARG_RET_BOOL(method, nm, argty) \
	void EVENT_CLS::method(std::function<bool(argty&)> &&cb) { \
		_ctrlEvents._parentWndBase._userEvents.wm_notify(_ctrlEvents._ctrlId, nm, \
			[cb = std::move(cb)](wm::Notify p) -> LRESULT { \
				return cb(p.hdr<argty>()); \
			}); \
	}

////////////////////////////////////////////////////////////////////////////////

#undef EVENT_CLS
#define EVENT_CLS ButtonEvents
EVENT_NFY_ARG(bcn_drop_down, BCN_DROPDOWN, NMBCDROPDOWN)
EVENT_NFY_ARG(bcn_hot_item_change, BCN_HOTITEMCHANGE, NMBCHOTITEM)
EVENT_CMD(bn_clicked, BN_CLICKED)
EVENT_CMD(bn_dbl_clk, BN_DBLCLK)
EVENT_CMD(bn_kill_focus, BN_KILLFOCUS)
EVENT_CMD(bn_set_focus, BN_SETFOCUS)
void ButtonEvents::nm_custom_draw(std::function<DWORD(NMCUSTOMDRAW&)> &&cb) {
	_ctrlEvents._parentWndBase._userEvents.wm_notify(_ctrlEvents._ctrlId, NM_CUSTOMDRAW,
		[cb = std::move(cb)](wm::Notify p) -> LRESULT {
			return cb(p.hdr<NMCUSTOMDRAW>());
		});
}

////////////////////////////////////////////////////////////////////////////////

#undef EVENT_CLS
#define EVENT_CLS ComboBoxEvents
EVENT_CMD(cbn_close_up, CBN_CLOSEUP)
EVENT_CMD(cbn_dbl_clk, CBN_DBLCLK)
EVENT_CMD(cbn_drop_down, CBN_DROPDOWN)
EVENT_CMD(cbn_edit_change, CBN_EDITCHANGE)
EVENT_CMD(cbn_edit_update, CBN_EDITUPDATE)
EVENT_CMD(cbn_kill_focus, CBN_KILLFOCUS)
EVENT_CMD(cbn_sel_change, CBN_SELCHANGE)
EVENT_CMD(cbn_sel_end_cancel, CBN_SELENDCANCEL)
EVENT_CMD(cbn_sel_end_ok, CBN_SELENDOK)
EVENT_CMD(cbn_set_focus, CBN_SETFOCUS)

////////////////////////////////////////////////////////////////////////////////

#undef EVENT_CLS
#define EVENT_CLS DateTimePickerEvents
EVENT_NFY(dtn_close_up, DTN_CLOSEUP)
EVENT_NFY_ARG(dtn_date_time_change, DTN_DATETIMECHANGE, NMDATETIMECHANGE)
EVENT_NFY(dtn_drop_down, DTN_DROPDOWN)
EVENT_NFY_ARG(dtn_format, DTN_FORMATW, NMDATETIMEFORMATW)
EVENT_NFY_ARG(dtn_format_query, DTN_FORMATQUERYW, NMDATETIMEFORMATQUERYW)
EVENT_NFY_ARG(dtn_user_string, DTN_USERSTRINGW, NMDATETIMESTRINGW)
EVENT_NFY_ARG(dtn_wm_key_down, DTN_WMKEYDOWNW, NMDATETIMEWMKEYDOWNW)
EVENT_NFY(nm_kill_focus, NM_KILLFOCUS)
EVENT_NFY(nm_set_focus, NM_SETFOCUS)

////////////////////////////////////////////////////////////////////////////////

#undef EVENT_CLS
#define EVENT_CLS EditEvents
EVENT_CMD(en_change, EN_CHANGE)
EVENT_CMD(en_h_scroll, EN_HSCROLL)
EVENT_CMD(en_kill_focus, EN_KILLFOCUS)
EVENT_CMD(en_max_text, EN_MAXTEXT)
EVENT_CMD(en_set_focus, EN_SETFOCUS)
EVENT_CMD(en_update, EN_UPDATE)
EVENT_CMD(en_v_scroll, EN_VSCROLL)

////////////////////////////////////////////////////////////////////////////////

#undef EVENT_CLS
#define EVENT_CLS ListViewEvents
EVENT_NFY_ARG(lvn_begin_drag, LVN_BEGINDRAG, NMLISTVIEW)
EVENT_NFY_ARG_RET_BOOL(lvn_begin_label_edit, LVN_BEGINLABELEDITW, NMLVDISPINFOW)
EVENT_NFY_ARG(lvn_begin_r_drag, LVN_BEGINRDRAG, NMLISTVIEW)
EVENT_NFY_ARG(lvn_begin_scroll, LVN_BEGINSCROLL, NMLVSCROLL)
EVENT_NFY_ARG(lvn_column_click, LVN_COLUMNCLICK, NMLISTVIEW)
EVENT_NFY_ARG(lvn_column_drop_down, LVN_COLUMNDROPDOWN, NMLISTVIEW)
EVENT_NFY_ARG(lvn_column_overflow_click, LVN_COLUMNOVERFLOWCLICK, NMLISTVIEW)
EVENT_NFY_ARG_RET_BOOL(lvn_delete_all_items, LVN_DELETEALLITEMS, NMLISTVIEW)
EVENT_NFY_ARG(lvn_delete_item, LVN_DELETEITEM, NMLISTVIEW)
EVENT_NFY_ARG_RET_BOOL(lvn_end_label_edit, LVN_ENDLABELEDITW, NMLVDISPINFOW)
EVENT_NFY_ARG(lvn_end_scroll, LVN_ENDSCROLL, NMLVSCROLL)
EVENT_NFY_ARG(lvn_insert_item, LVN_INSERTITEM, NMLISTVIEW)
EVENT_NFY_ARG(lvn_item_activate, LVN_ITEMACTIVATE, NMITEMACTIVATE)
EVENT_NFY_ARG(lvn_item_changed, LVN_ITEMCHANGED, NMLISTVIEW)
EVENT_NFY_ARG_RET_BOOL(lvn_item_changing, LVN_ITEMCHANGING, NMLISTVIEW)
EVENT_NFY_ARG(lvn_key_down, LVN_KEYDOWN, NMLVKEYDOWN)
EVENT_NFY_ARG(nm_click, NM_CLICK, NMITEMACTIVATE)
void ListViewEvents::nm_custom_draw(std::function<DWORD(NMLVCUSTOMDRAW&)> &&cb) {
	_ctrlEvents._parentWndBase._userEvents.wm_notify(_ctrlEvents._ctrlId, NM_CUSTOMDRAW,
		[cb = std::move(cb)](wm::Notify p) -> LRESULT {
			return cb(p.hdr<NMLVCUSTOMDRAW>());
		});
}
EVENT_NFY_ARG(nm_dbl_clk, NM_DBLCLK, NMITEMACTIVATE)
EVENT_NFY_ARG(nm_kill_focus, NM_KILLFOCUS, NMHDR)
EVENT_NFY_ARG(nm_r_click, NM_RCLICK, NMITEMACTIVATE)
EVENT_NFY_ARG(nm_r_dbl_clk, NM_RDBLCLK, NMITEMACTIVATE)
EVENT_NFY_ARG(nm_released_capture, NM_RELEASEDCAPTURE, NMHDR)
EVENT_NFY_ARG(nm_set_focus, NM_SETFOCUS, NMHDR)

////////////////////////////////////////////////////////////////////////////////

#undef EVENT_CLS
#define EVENT_CLS MonthCalendarEvents
EVENT_NFY_ARG(mcn_get_day_state, MCN_GETDAYSTATE, NMDAYSTATE)
EVENT_NFY_ARG(mcn_sel_change, MCN_SELCHANGE, NMSELCHANGE)
EVENT_NFY_ARG(mcn_select, MCN_SELECT, NMSELCHANGE)
EVENT_NFY_ARG(mcn_view_change, MCN_VIEWCHANGE, NMVIEWCHANGE)
EVENT_NFY(nm_released_capture, NM_RELEASEDCAPTURE)

////////////////////////////////////////////////////////////////////////////////

#undef EVENT_CLS
#define EVENT_CLS StaticEvents
EVENT_CMD(stn_clicked, STN_CLICKED)
EVENT_CMD(stn_dbl_clk, STN_DBLCLK)
EVENT_CMD(stn_disable, STN_DISABLE)
EVENT_CMD(stn_enable, STN_ENABLE)

////////////////////////////////////////////////////////////////////////////////

#undef EVENT_CLS
#define EVENT_CLS StatusBarEvents
EVENT_NFY_ARG_RET_BOOL(nm_click, NM_CLICK, NMMOUSE)
EVENT_NFY_ARG_RET_BOOL(nm_dbl_clk, NM_DBLCLK, NMMOUSE)
EVENT_NFY_ARG_RET_BOOL(nm_r_click, NM_RCLICK, NMMOUSE)
EVENT_NFY_ARG_RET_BOOL(nm_r_dbl_clk, NM_RDBLCLK, NMMOUSE)
EVENT_NFY_ARG(sbn_simple_mode_change, SBN_SIMPLEMODECHANGE, NMMOUSE)
