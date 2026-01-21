#include <memory>
#include "events-ctl.h"
#include "wnd-controls.h"
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
				return _ctrlEvents._parentWndBase._preEvents._isDlg ? TRUE : 0; \
			}); \
	}
#define EVENT_NFY_RET(method, nm, intret) \
	void EVENT_CLS::method(std::function<intret()> &&cb) { \
		_ctrlEvents._parentWndBase._userEvents.wm_notify(_ctrlEvents._ctrlId, nm, \
			[this, cb = std::move(cb)](wm::Notify) -> LRESULT { \
				return cb(); \
			}); \
	}

#define EVENT_NFY_ARG(method, nm, argty) \
	void EVENT_CLS::method(std::function<void(argty&)> &&cb) { \
		_ctrlEvents._parentWndBase._userEvents.wm_notify(_ctrlEvents._ctrlId, nm, \
			[this, cb = std::move(cb)](wm::Notify p) -> LRESULT { \
				cb(p.hdr<argty>()); \
				return _ctrlEvents._parentWndBase._preEvents._isDlg ? TRUE : 0; \
			}); \
	}
#define EVENT_NFY_ARG_RET(method, nm, argty, intret) \
	void EVENT_CLS::method(std::function<intret(argty&)> &&cb) { \
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
EVENT_NFY_ARG_RET(nm_custom_draw, NM_CUSTOMDRAW, NMCUSTOMDRAW, DWORD)

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
EVENT_NFY_ARG_RET(lvn_begin_label_edit, LVN_BEGINLABELEDITW, NMLVDISPINFOW, bool)
EVENT_NFY_ARG(lvn_begin_r_drag, LVN_BEGINRDRAG, NMLISTVIEW)
EVENT_NFY_ARG(lvn_begin_scroll, LVN_BEGINSCROLL, NMLVSCROLL)
EVENT_NFY_ARG(lvn_column_click, LVN_COLUMNCLICK, NMLISTVIEW)
EVENT_NFY_ARG(lvn_column_drop_down, LVN_COLUMNDROPDOWN, NMLISTVIEW)
EVENT_NFY_ARG(lvn_column_overflow_click, LVN_COLUMNOVERFLOWCLICK, NMLISTVIEW)
EVENT_NFY_ARG_RET(lvn_delete_all_items, LVN_DELETEALLITEMS, NMLISTVIEW, bool)
EVENT_NFY_ARG(lvn_delete_item, LVN_DELETEITEM, NMLISTVIEW)
EVENT_NFY_ARG_RET(lvn_end_label_edit, LVN_ENDLABELEDITW, NMLVDISPINFOW, bool)
EVENT_NFY_ARG(lvn_end_scroll, LVN_ENDSCROLL, NMLVSCROLL)
EVENT_NFY_ARG(lvn_insert_item, LVN_INSERTITEM, NMLISTVIEW)
EVENT_NFY_ARG(lvn_item_activate, LVN_ITEMACTIVATE, NMITEMACTIVATE)
EVENT_NFY_ARG(lvn_item_changed, LVN_ITEMCHANGED, NMLISTVIEW)
EVENT_NFY_ARG_RET(lvn_item_changing, LVN_ITEMCHANGING, NMLISTVIEW, bool)
EVENT_NFY_ARG(lvn_key_down, LVN_KEYDOWN, NMLVKEYDOWN)
EVENT_NFY_ARG(nm_click, NM_CLICK, NMITEMACTIVATE)
EVENT_NFY_ARG_RET(nm_custom_draw, NM_CUSTOMDRAW, NMLVCUSTOMDRAW, DWORD)
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

#define RADIO_GROUP_CMD(method) \
	void RadioGroupEvents::method(std::function<void(int)> &&cb) { \
		auto callback = std::make_shared<std::function<void(int)>>(std::move(cb)); \
		for (int i = 0; i < _ownerGroup.radios.count(); ++i) { \
			_ownerGroup.radios[i].on().method([i, callback]() -> void { \
				(*callback)(i); \
			}); \
		} \
	}
RADIO_GROUP_CMD(bn_clicked)
RADIO_GROUP_CMD(bn_dbl_clk)
RADIO_GROUP_CMD(bn_kill_focus)
RADIO_GROUP_CMD(bn_set_focus)

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
EVENT_NFY_ARG_RET(nm_click, NM_CLICK, NMMOUSE, bool)
EVENT_NFY_ARG_RET(nm_dbl_clk, NM_DBLCLK, NMMOUSE, bool)
EVENT_NFY_ARG_RET(nm_r_click, NM_RCLICK, NMMOUSE, bool)
EVENT_NFY_ARG_RET(nm_r_dbl_clk, NM_RDBLCLK, NMMOUSE, bool)
EVENT_NFY_ARG(sbn_simple_mode_change, SBN_SIMPLEMODECHANGE, NMMOUSE)

////////////////////////////////////////////////////////////////////////////////

#undef EVENT_CLS
#define EVENT_CLS TreeViewEvents
EVENT_NFY_ARG(tvn_async_draw, TVN_ASYNCDRAW, NMTVASYNCDRAW)
EVENT_NFY_ARG(tvn_begin_drag, TVN_BEGINDRAGW, NMTREEVIEWW)
EVENT_NFY_ARG_RET(tvn_begin_label_edit, TVN_BEGINLABELEDITW, NMTVDISPINFOW, bool)
EVENT_NFY_ARG(tvn_begin_r_drag, TVN_BEGINRDRAGW, NMTREEVIEWW)
EVENT_NFY_ARG(tvn_delete_item, TVN_DELETEITEMW, NMTREEVIEWW)
EVENT_NFY_ARG_RET(tvn_end_label_edit, TVN_ENDLABELEDITW, NMTVDISPINFOW, bool)
EVENT_NFY_ARG(tvn_get_disp_info, TVN_GETDISPINFOW, NMTVDISPINFOW)
EVENT_NFY_ARG(tvn_get_info_tip, TVN_GETINFOTIPW, NMTVGETINFOTIPW)
EVENT_NFY_ARG(tvn_item_changed, TVN_ITEMCHANGEDW, NMTVITEMCHANGE)
EVENT_NFY_ARG_RET(tvn_item_changing, TVN_ITEMCHANGINGW, NMTVITEMCHANGE, bool)
EVENT_NFY_ARG(tvn_item_expanded, TVN_ITEMEXPANDEDW, NMTREEVIEWW)
EVENT_NFY_ARG_RET(tvn_item_expanding, TVN_ITEMEXPANDINGW, NMTREEVIEWW, bool)
EVENT_NFY_ARG_RET(tvn_key_down, TVN_KEYDOWN, NMTVKEYDOWN, int)
EVENT_NFY_ARG(tvn_sel_changed, TVN_SELCHANGEDW, NMTREEVIEWW)
EVENT_NFY_ARG_RET(tvn_sel_changing, TVN_SELCHANGINGW, NMTREEVIEWW, bool)
EVENT_NFY_ARG(tvn_set_disp_info, TVN_SETDISPINFOW, NMTVDISPINFOW)
EVENT_NFY_ARG_RET(tvn_single_expand, TVN_SINGLEEXPAND, NMTREEVIEWW, WORD)
EVENT_NFY_RET(nm_click, NM_CLICK, int)
EVENT_NFY_ARG_RET(nm_custom_draw, NM_CUSTOMDRAW, NMTVCUSTOMDRAW, DWORD)
EVENT_NFY_RET(nm_dbl_clk, NM_DBLCLK, int)
EVENT_NFY(nm_kill_focus, NM_KILLFOCUS)
EVENT_NFY_RET(nm_r_click, NM_RCLICK, int)
EVENT_NFY_RET(nm_r_dbl_clk, NM_RDBLCLK, int)
EVENT_NFY_RET(nm_return, NM_RETURN, int)
EVENT_NFY_ARG_RET(nm_set_cursor, NM_SETCURSOR, NMMOUSE, int)
EVENT_NFY(nm_set_focus, NM_SETFOCUS)

////////////////////////////////////////////////////////////////////////////////

#undef EVENT_CLS
#define EVENT_CLS TabEvents
EVENT_NFY(tcn_focus_change, TCN_FOCUSCHANGE)
EVENT_NFY_ARG(tcn_get_object, TCN_GETOBJECT, NMOBJECTNOTIFY)
EVENT_NFY_ARG(tcn_key_down, TCN_KEYDOWN, NMTCKEYDOWN)
EVENT_NFY(tcn_sel_change, TCN_SELCHANGE)
EVENT_NFY_RET(tcn_sel_changing, TCN_SELCHANGING, bool)
EVENT_NFY(nm_click, NM_CLICK)
EVENT_NFY(nm_dbl_clk, NM_DBLCLK)
EVENT_NFY(nm_r_click, NM_RCLICK)
EVENT_NFY(nm_r_dbl_clk, NM_RDBLCLK)
EVENT_NFY(nm_released_capture, NM_RELEASEDCAPTURE)
