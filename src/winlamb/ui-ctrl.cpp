#include <memory>
#include <system_error>
#include "ui-ctrl.hpp"
#include "ui-app.hpp"
using namespace _wl_internal;
using namespace wl;
using namespace wl::events;

constexpr bool operator==(const SIZE a, const SIZE b) { return a.cx == b.cx && a.cy == b.cy; }

#define EVENT_CMD(eventcls, method, cmd) \
	void eventcls::method(std::function<void()> &&cb) { \
		_ctrlEvents._parent._userEvents.wm_command(_ctrlEvents._ctrlId, cmd, std::move(cb)); \
	}

#define EVENT_NFY(eventcls, method, nm) \
	void eventcls::method(std::function<void()> &&cb) { \
		_ctrlEvents._parent._userEvents.wm_notify(_ctrlEvents._ctrlId, nm, \
			[this, cb = std::move(cb)](wm::Notify) -> LRESULT { \
				cb(); \
				return _ctrlEvents._parent._preEvents._isDlg ? TRUE : 0; \
			}); \
	}
#define EVENT_NFY_RET(eventcls, method, nm, intret) \
	void eventcls::method(std::function<intret()> &&cb) { \
		_ctrlEvents._parent._userEvents.wm_notify(_ctrlEvents._ctrlId, nm, \
			[this, cb = std::move(cb)](wm::Notify) -> LRESULT { \
				return cb(); \
			}); \
	}

#define EVENT_NFY_ARG(eventcls, method, nm, argty) \
	void eventcls::method(std::function<void(argty&)> &&cb) { \
		_ctrlEvents._parent._userEvents.wm_notify(_ctrlEvents._ctrlId, nm, \
			[this, cb = std::move(cb)](wm::Notify p) -> LRESULT { \
				cb(p.hdr<argty>()); \
				return _ctrlEvents._parent._preEvents._isDlg ? TRUE : 0; \
			}); \
	}
#define EVENT_NFY_ARG_RET(eventcls, method, nm, argty, intret) \
	void eventcls::method(std::function<intret(argty&)> &&cb) { \
		_ctrlEvents._parent._userEvents.wm_notify(_ctrlEvents._ctrlId, nm, \
			[cb = std::move(cb)](wm::Notify p) -> LRESULT { \
				return cb(p.hdr<argty>()); \
			}); \
		}

EVENT_NFY_ARG(ButtonEvents, bcn_drop_down, BCN_DROPDOWN, NMBCDROPDOWN)
EVENT_NFY_ARG(ButtonEvents, bcn_hot_item_change, BCN_HOTITEMCHANGE, NMBCHOTITEM)
EVENT_CMD(ButtonEvents, bn_clicked, BN_CLICKED)
EVENT_CMD(ButtonEvents, bn_dbl_clk, BN_DBLCLK)
EVENT_CMD(ButtonEvents, bn_kill_focus, BN_KILLFOCUS)
EVENT_CMD(ButtonEvents, bn_set_focus, BN_SETFOCUS)
EVENT_NFY_ARG_RET(ButtonEvents, nm_custom_draw, NM_CUSTOMDRAW, NMCUSTOMDRAW, DWORD)

EVENT_CMD(ComboBoxEvents, cbn_close_up, CBN_CLOSEUP)
EVENT_CMD(ComboBoxEvents, cbn_dbl_clk, CBN_DBLCLK)
EVENT_CMD(ComboBoxEvents, cbn_drop_down, CBN_DROPDOWN)
EVENT_CMD(ComboBoxEvents, cbn_edit_change, CBN_EDITCHANGE)
EVENT_CMD(ComboBoxEvents, cbn_edit_update, CBN_EDITUPDATE)
EVENT_CMD(ComboBoxEvents, cbn_kill_focus, CBN_KILLFOCUS)
EVENT_CMD(ComboBoxEvents, cbn_sel_change, CBN_SELCHANGE)
EVENT_CMD(ComboBoxEvents, cbn_sel_end_cancel, CBN_SELENDCANCEL)
EVENT_CMD(ComboBoxEvents, cbn_sel_end_ok, CBN_SELENDOK)
EVENT_CMD(ComboBoxEvents, cbn_set_focus, CBN_SETFOCUS)

EVENT_NFY(DateTimePickerEvents, dtn_close_up, DTN_CLOSEUP)
EVENT_NFY_ARG(DateTimePickerEvents, dtn_date_time_change, DTN_DATETIMECHANGE, NMDATETIMECHANGE)
EVENT_NFY(DateTimePickerEvents, dtn_drop_down, DTN_DROPDOWN)
EVENT_NFY_ARG(DateTimePickerEvents, dtn_format, DTN_FORMATW, NMDATETIMEFORMATW)
EVENT_NFY_ARG(DateTimePickerEvents, dtn_format_query, DTN_FORMATQUERYW, NMDATETIMEFORMATQUERYW)
EVENT_NFY_ARG(DateTimePickerEvents, dtn_user_string, DTN_USERSTRINGW, NMDATETIMESTRINGW)
EVENT_NFY_ARG(DateTimePickerEvents, dtn_wm_key_down, DTN_WMKEYDOWNW, NMDATETIMEWMKEYDOWNW)
EVENT_NFY(DateTimePickerEvents, nm_kill_focus, NM_KILLFOCUS)
EVENT_NFY(DateTimePickerEvents, nm_set_focus, NM_SETFOCUS)

EVENT_CMD(EditEvents, en_change, EN_CHANGE)
EVENT_CMD(EditEvents, en_h_scroll, EN_HSCROLL)
EVENT_CMD(EditEvents, en_kill_focus, EN_KILLFOCUS)
EVENT_CMD(EditEvents, en_max_text, EN_MAXTEXT)
EVENT_CMD(EditEvents, en_set_focus, EN_SETFOCUS)
EVENT_CMD(EditEvents, en_update, EN_UPDATE)
EVENT_CMD(EditEvents, en_v_scroll, EN_VSCROLL)

EVENT_NFY_ARG(ListViewEvents, lvn_begin_drag, LVN_BEGINDRAG, NMLISTVIEW)
EVENT_NFY_ARG_RET(ListViewEvents, lvn_begin_label_edit, LVN_BEGINLABELEDITW, NMLVDISPINFOW, bool)
EVENT_NFY_ARG(ListViewEvents, lvn_begin_r_drag, LVN_BEGINRDRAG, NMLISTVIEW)
EVENT_NFY_ARG(ListViewEvents, lvn_begin_scroll, LVN_BEGINSCROLL, NMLVSCROLL)
EVENT_NFY_ARG(ListViewEvents, lvn_column_click, LVN_COLUMNCLICK, NMLISTVIEW)
EVENT_NFY_ARG(ListViewEvents, lvn_column_drop_down, LVN_COLUMNDROPDOWN, NMLISTVIEW)
EVENT_NFY_ARG(ListViewEvents, lvn_column_overflow_click, LVN_COLUMNOVERFLOWCLICK, NMLISTVIEW)
EVENT_NFY_ARG_RET(ListViewEvents, lvn_delete_all_items, LVN_DELETEALLITEMS, NMLISTVIEW, bool)
EVENT_NFY_ARG(ListViewEvents, lvn_delete_item, LVN_DELETEITEM, NMLISTVIEW)
EVENT_NFY_ARG_RET(ListViewEvents, lvn_end_label_edit, LVN_ENDLABELEDITW, NMLVDISPINFOW, bool)
EVENT_NFY_ARG(ListViewEvents, lvn_end_scroll, LVN_ENDSCROLL, NMLVSCROLL)
EVENT_NFY_ARG(ListViewEvents, lvn_insert_item, LVN_INSERTITEM, NMLISTVIEW)
EVENT_NFY_ARG(ListViewEvents, lvn_item_activate, LVN_ITEMACTIVATE, NMITEMACTIVATE)
EVENT_NFY_ARG(ListViewEvents, lvn_item_changed, LVN_ITEMCHANGED, NMLISTVIEW)
EVENT_NFY_ARG_RET(ListViewEvents, lvn_item_changing, LVN_ITEMCHANGING, NMLISTVIEW, bool)
EVENT_NFY_ARG(ListViewEvents, lvn_key_down, LVN_KEYDOWN, NMLVKEYDOWN)
EVENT_NFY_ARG(ListViewEvents, nm_click, NM_CLICK, NMITEMACTIVATE)
EVENT_NFY_ARG_RET(ListViewEvents, nm_custom_draw, NM_CUSTOMDRAW, NMLVCUSTOMDRAW, DWORD)
EVENT_NFY_ARG(ListViewEvents, nm_dbl_clk, NM_DBLCLK, NMITEMACTIVATE)
EVENT_NFY_ARG(ListViewEvents, nm_kill_focus, NM_KILLFOCUS, NMHDR)
EVENT_NFY_ARG(ListViewEvents, nm_r_click, NM_RCLICK, NMITEMACTIVATE)
EVENT_NFY_ARG(ListViewEvents, nm_r_dbl_clk, NM_RDBLCLK, NMITEMACTIVATE)
EVENT_NFY_ARG(ListViewEvents, nm_released_capture, NM_RELEASEDCAPTURE, NMHDR)
EVENT_NFY_ARG(ListViewEvents, nm_set_focus, NM_SETFOCUS, NMHDR)

EVENT_NFY_ARG(MonthCalendarEvents, mcn_get_day_state, MCN_GETDAYSTATE, NMDAYSTATE)
EVENT_NFY_ARG(MonthCalendarEvents, mcn_sel_change, MCN_SELCHANGE, NMSELCHANGE)
EVENT_NFY_ARG(MonthCalendarEvents, mcn_select, MCN_SELECT, NMSELCHANGE)
EVENT_NFY_ARG(MonthCalendarEvents, mcn_view_change, MCN_VIEWCHANGE, NMVIEWCHANGE)
EVENT_NFY(MonthCalendarEvents, nm_released_capture, NM_RELEASEDCAPTURE)

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

EVENT_CMD(StaticEvents, stn_clicked, STN_CLICKED)
EVENT_CMD(StaticEvents, stn_dbl_clk, STN_DBLCLK)
EVENT_CMD(StaticEvents, stn_disable, STN_DISABLE)
EVENT_CMD(StaticEvents, stn_enable, STN_ENABLE)

EVENT_NFY_ARG_RET(StatusBarEvents, nm_click, NM_CLICK, NMMOUSE, bool)
EVENT_NFY_ARG_RET(StatusBarEvents, nm_dbl_clk, NM_DBLCLK, NMMOUSE, bool)
EVENT_NFY_ARG_RET(StatusBarEvents, nm_r_click, NM_RCLICK, NMMOUSE, bool)
EVENT_NFY_ARG_RET(StatusBarEvents, nm_r_dbl_clk, NM_RDBLCLK, NMMOUSE, bool)
EVENT_NFY_ARG(StatusBarEvents, sbn_simple_mode_change, SBN_SIMPLEMODECHANGE, NMMOUSE)

EVENT_NFY_ARG_RET(TrackbarEvents, trbn_thumb_pos_changing, TRBN_THUMBPOSCHANGING, NMTRBTHUMBPOSCHANGING, bool)
void TrackbarEvents::wm_h_scroll(std::function<void(wm::HScroll)> &&cb) {
	_ctrlEvents._parent._userEvents.wm(WM_HSCROLL, [this, cb = std::move(cb)](wl::wm::Msg p) -> LRESULT {
		wm::HScroll hs{p};
		if (_ctrlEvents._ctrlId == GetDlgCtrlID(hs.hwnd_scrollbar()))
			cb(hs);
		return _ctrlEvents._parent._preEvents._isDlg ? TRUE : 0;
	});
}
void TrackbarEvents::wm_v_scroll(std::function<void(wm::VScroll)> &&cb) {
	_ctrlEvents._parent._userEvents.wm(WM_VSCROLL, [this, cb = std::move(cb)](wl::wm::Msg p) -> LRESULT {
		wm::VScroll vs{p};
		if (_ctrlEvents._ctrlId == GetDlgCtrlID(vs.hwnd_scrollbar()))
			cb(vs);
		return _ctrlEvents._parent._preEvents._isDlg ? TRUE : 0;
	});
}
EVENT_NFY_ARG_RET(TrackbarEvents, nm_custom_draw, NM_CUSTOMDRAW, NMCUSTOMDRAW, DWORD)
EVENT_NFY(TrackbarEvents, nm_released_capture, NM_RELEASEDCAPTURE)

EVENT_NFY_ARG(TreeViewEvents, tvn_async_draw, TVN_ASYNCDRAW, NMTVASYNCDRAW)
EVENT_NFY_ARG(TreeViewEvents, tvn_begin_drag, TVN_BEGINDRAGW, NMTREEVIEWW)
EVENT_NFY_ARG_RET(TreeViewEvents, tvn_begin_label_edit, TVN_BEGINLABELEDITW, NMTVDISPINFOW, bool)
EVENT_NFY_ARG(TreeViewEvents, tvn_begin_r_drag, TVN_BEGINRDRAGW, NMTREEVIEWW)
EVENT_NFY_ARG(TreeViewEvents, tvn_delete_item, TVN_DELETEITEMW, NMTREEVIEWW)
EVENT_NFY_ARG_RET(TreeViewEvents, tvn_end_label_edit, TVN_ENDLABELEDITW, NMTVDISPINFOW, bool)
EVENT_NFY_ARG(TreeViewEvents, tvn_get_disp_info, TVN_GETDISPINFOW, NMTVDISPINFOW)
EVENT_NFY_ARG(TreeViewEvents, tvn_get_info_tip, TVN_GETINFOTIPW, NMTVGETINFOTIPW)
EVENT_NFY_ARG(TreeViewEvents, tvn_item_changed, TVN_ITEMCHANGEDW, NMTVITEMCHANGE)
EVENT_NFY_ARG_RET(TreeViewEvents, tvn_item_changing, TVN_ITEMCHANGINGW, NMTVITEMCHANGE, bool)
EVENT_NFY_ARG(TreeViewEvents, tvn_item_expanded, TVN_ITEMEXPANDEDW, NMTREEVIEWW)
EVENT_NFY_ARG_RET(TreeViewEvents, tvn_item_expanding, TVN_ITEMEXPANDINGW, NMTREEVIEWW, bool)
EVENT_NFY_ARG_RET(TreeViewEvents, tvn_key_down, TVN_KEYDOWN, NMTVKEYDOWN, int)
EVENT_NFY_ARG(TreeViewEvents, tvn_sel_changed, TVN_SELCHANGEDW, NMTREEVIEWW)
EVENT_NFY_ARG_RET(TreeViewEvents, tvn_sel_changing, TVN_SELCHANGINGW, NMTREEVIEWW, bool)
EVENT_NFY_ARG(TreeViewEvents, tvn_set_disp_info, TVN_SETDISPINFOW, NMTVDISPINFOW)
EVENT_NFY_ARG_RET(TreeViewEvents, tvn_single_expand, TVN_SINGLEEXPAND, NMTREEVIEWW, WORD)
EVENT_NFY_RET(TreeViewEvents, nm_click, NM_CLICK, int)
EVENT_NFY_ARG_RET(TreeViewEvents, nm_custom_draw, NM_CUSTOMDRAW, NMTVCUSTOMDRAW, DWORD)
EVENT_NFY_RET(TreeViewEvents, nm_dbl_clk, NM_DBLCLK, int)
EVENT_NFY(TreeViewEvents, nm_kill_focus, NM_KILLFOCUS)
EVENT_NFY_RET(TreeViewEvents, nm_r_click, NM_RCLICK, int)
EVENT_NFY_RET(TreeViewEvents, nm_r_dbl_clk, NM_RDBLCLK, int)
EVENT_NFY_RET(TreeViewEvents, nm_return, NM_RETURN, int)
EVENT_NFY_ARG_RET(TreeViewEvents, nm_set_cursor, NM_SETCURSOR, NMMOUSE, int)
EVENT_NFY(TreeViewEvents, nm_set_focus, NM_SETFOCUS)

EVENT_NFY(TabEvents, tcn_focus_change, TCN_FOCUSCHANGE)
EVENT_NFY_ARG(TabEvents, tcn_get_object, TCN_GETOBJECT, NMOBJECTNOTIFY)
EVENT_NFY_ARG(TabEvents, tcn_key_down, TCN_KEYDOWN, NMTCKEYDOWN)
EVENT_NFY(TabEvents, tcn_sel_change, TCN_SELCHANGE)
EVENT_NFY_RET(TabEvents, tcn_sel_changing, TCN_SELCHANGING, bool)
EVENT_NFY(TabEvents, nm_click, NM_CLICK)
EVENT_NFY(TabEvents, nm_dbl_clk, NM_DBLCLK)
EVENT_NFY(TabEvents, nm_r_click, NM_RCLICK)
EVENT_NFY(TabEvents, nm_r_dbl_clk, NM_RDBLCLK)
EVENT_NFY(TabEvents, nm_released_capture, NM_RELEASEDCAPTURE)

////////////////////////////////////////////////////////////////////////////////

Button::Button(IWindowParent &owner, ButtonOpts creationOpts)
	: _ctrl{owner.base()}, _events{owner.base(), valid_ctrl_id(creationOpts.ctrlId)}
{
	_ctrl._parent._preEvents.wm_create_or_init_dialog(
		[this, pOwner = &owner, opts = std::move(creationOpts)]() mutable -> void {
			if (opts.size == ButtonOpts{}.size)
				opts.size = dpi::sz(opts.size); // special case: default size

			_ctrl.create_wnd(ctrl_id(), opts.styleEx, L"BUTTON", std::move(opts.text),
				opts.style, opts.pos, opts.size);
			apply_ui_font(hwnd());
			_ctrl._parent._layout.add(hwnd(), opts.layout);
		});
}

Button::Button(IWindowParent &owner, WORD ctrlId, Lay layout)
	: _ctrl{owner.base()}, _events{owner.base(), ctrlId}
{
	_ctrl._parent._preEvents.wm_create_or_init_dialog([this, layout]() -> void {
		_ctrl.assign_dlg(ctrl_id());
		_ctrl._parent._layout.add(hwnd(), layout);
	});
}

const Button& Button::set_text(WStrView newText) const {
	set_wnd_text(hwnd(), newText);
	return *this;
}

const Button& Button::trigger_click() const {
	SendMessageW(hwnd(), BM_CLICK, 0, 0);
	return *this;
}

////////////////////////////////////////////////////////////////////////////////

CheckBox::CheckBox(IWindowParent &owner, CheckBoxOpts creationOpts)
	: _ctrl{owner.base()}, _events{owner.base(), valid_ctrl_id(creationOpts.ctrlId)}
{
	_ctrl._parent._preEvents.wm_create_or_init_dialog(
		[this, pOwner = &owner, opts = std::move(creationOpts)]() mutable -> void {
			if (!opts.size.cx && !opts.size.cy)
				opts.size = calc_text_bound_box_with_check(str::remove_accel_ampersands(opts.text));

			_ctrl.create_wnd(ctrl_id(), opts.styleEx, L"BUTTON", std::move(opts.text),
				opts.style, opts.pos, opts.size);
			apply_ui_font(hwnd());
			_ctrl._parent._layout.add(hwnd(), opts.layout);
			set_state(opts.state);
		});
}

CheckBox::CheckBox(IWindowParent &owner, WORD ctrlId, Lay layout)
	: _ctrl{owner.base()}, _events{owner.base(), ctrlId}
{
	_ctrl._parent._preEvents.wm_create_or_init_dialog([this, layout]() -> void {
		_ctrl.assign_dlg(ctrl_id());
		_ctrl._parent._layout.add(hwnd(), layout);
	});
}

WORD CheckBox::state() const {
	return static_cast<WORD>(SendMessageW(hwnd(), BM_GETCHECK, 0, 0));
}

const CheckBox& CheckBox::set_state(WORD bstFlag) const {
	bstFlag &= (BST_CHECKED | BST_INDETERMINATE | BST_UNCHECKED); // sanitize
	SendMessageW(hwnd(), BM_SETCHECK, bstFlag, 0);
	return *this;
}

const CheckBox& CheckBox::set_text(WStrView newText) const {
	set_wnd_text(hwnd(), newText);
	return *this;
}

const CheckBox& CheckBox::set_text_resize(WStrView newText) const {
	set_text(newText);
	SIZE bounds = calc_text_bound_box_with_check(str::remove_accel_ampersands(newText));
	SetWindowPos(hwnd(), nullptr, 0, 0, bounds.cx, bounds.cy, SWP_NOZORDER | SWP_NOMOVE);
	return *this;
}

////////////////////////////////////////////////////////////////////////////////

std::wstring ComboBox::ItemCollection::operator[](int index) const {
	size_t nChars = SendMessageW(_owner.hwnd(), CB_GETLBTEXTLEN, index, 0);
	std::wstring s(nChars + 1, L'\0');
	SendMessageW(_owner.hwnd(), CB_GETLBTEXT, index, reinterpret_cast<LPARAM>(s.data()));
	s.resize(nChars);
	return s;
}

void ComboBox::ItemCollection::add(WStrView text) const {
	SendMessageW(_owner.hwnd(), CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(text.c_str()));
}

void ComboBox::ItemCollection::add(std::initializer_list<WStrView> texts) const {
	for (auto &&s : texts)
		add(s);
}

size_t ComboBox::ItemCollection::count() const {
	return SendMessageW(_owner.hwnd(), CB_GETCOUNT, 0, 0);
}

void ComboBox::ItemCollection::delete_all() const {
	SendMessageW(_owner.hwnd(), CB_RESETCONTENT, 0, 0);
}

void ComboBox::ItemCollection::select(int index) const {
	SendMessageW(_owner.hwnd(), CB_SETCURSEL, index, 0);
}

int ComboBox::ItemCollection::selected_index() const {
	return static_cast<int>(SendMessageW(_owner.hwnd(), CB_GETCURSEL, 0, 0));
}

std::optional<std::wstring> ComboBox::ItemCollection::selected_text() const {
	int selIdx = selected_index();
	return selIdx == -1 ? std::nullopt : std::make_optional(operator[](selIdx));
}

//------------------------------------------------------------------------------

ComboBox::ComboBox(IWindowParent &owner, ComboBoxOpts creationOpts)
	: _ctrl{owner.base()}, _events{owner.base(), valid_ctrl_id(creationOpts.ctrlId)}
{
	_ctrl._parent._preEvents.wm_create_or_init_dialog(
		[this, pOwner = &owner, opts = std::move(creationOpts)]() mutable -> void {
			if (opts.width == ComboBoxOpts{}.width)
				opts.width = dpi::x(opts.width); // special case: default width

			_ctrl.create_wnd(ctrl_id(), opts.styleEx, L"COMBOBOX", {},
				opts.style, opts.pos, {.cx = opts.width});
			apply_ui_font(hwnd());
			_ctrl._parent._layout.add(hwnd(), opts.layout);
			for (auto &&s : opts.texts)
				items.add(s);
		});
}

ComboBox::ComboBox(IWindowParent &owner, WORD ctrlId, Lay layout)
	: _ctrl{owner.base()}, _events{owner.base(), ctrlId}
{
	_ctrl._parent._preEvents.wm_create_or_init_dialog([this, layout]() -> void {
		_ctrl.assign_dlg(ctrl_id());
		_ctrl._parent._layout.add(hwnd(), layout);
	});
}

////////////////////////////////////////////////////////////////////////////////

DateTimePicker::DateTimePicker(IWindowParent &owner, DateTimePickerOpts creationOpts)
	: _ctrl{owner.base()}, _events{owner.base(), valid_ctrl_id(creationOpts.ctrlId)}
{
	_ctrl._parent._preEvents.wm_create_or_init_dialog(
		[this, pOwner = &owner, opts = std::move(creationOpts)]() mutable -> void {
			if (opts.size == DateTimePickerOpts{}.size)
				opts.size = dpi::sz(opts.size); // special case: default size

			_ctrl.create_wnd(ctrl_id(), opts.styleEx, DATETIMEPICK_CLASSW, {},
				opts.style, opts.pos, opts.size);
			apply_ui_font(hwnd());
			_ctrl._parent._layout.add(hwnd(), opts.layout);
			if (opts.value.wYear)
				set_value(opts.value);
		});
}

DateTimePicker::DateTimePicker(IWindowParent &owner, WORD ctrlId, Lay layout)
	: _ctrl{owner.base()}, _events{owner.base(), ctrlId}
{
	_ctrl._parent._preEvents.wm_create_or_init_dialog([this, layout]() -> void {
		_ctrl.assign_dlg(ctrl_id());
		_ctrl._parent._layout.add(hwnd(), layout);
	});
}

SYSTEMTIME DateTimePicker::value() const {
	SYSTEMTIME st{};
	DateTime_GetSystemtime(hwnd(), &st);
	return st;
}

const DateTimePicker& DateTimePicker::set_value(const SYSTEMTIME &st) const {
	DateTime_SetSystemtime(hwnd(), GDT_VALID, &st);
	return *this;
}

////////////////////////////////////////////////////////////////////////////////

Edit::Edit(IWindowParent &owner, EditOpts creationOpts)
	: _ctrl{owner.base()}, _events{owner.base(), valid_ctrl_id(creationOpts.ctrlId)}
{
	_ctrl._parent._preEvents.wm_create_or_init_dialog(
		[this, pOwner = &owner, opts = std::move(creationOpts)]() mutable -> void {
			if (opts.size == EditOpts{}.size)
				opts.size = dpi::sz(opts.size); // special case: default size

			_ctrl.create_wnd(ctrl_id(), opts.styleEx, L"EDIT", std::move(opts.text),
				opts.style, opts.pos, opts.size);
			apply_ui_font(hwnd());
			_ctrl._parent._layout.add(hwnd(), opts.layout);
		});
}

Edit::Edit(IWindowParent &owner, WORD ctrlId, Lay layout)
	: _ctrl{owner.base()}, _events{owner.base(), ctrlId}
{
	_ctrl._parent._preEvents.wm_create_or_init_dialog([this, layout]() -> void {
		_ctrl.assign_dlg(ctrl_id());
		_ctrl._parent._layout.add(hwnd(), layout);
	});
}

const Edit& Edit::set_text(WStrView newText) const {
	set_wnd_text(hwnd(), newText);
	return *this;
}

////////////////////////////////////////////////////////////////////////////////

std::vector<std::wstring> ListView::Column::item_texts() const {
	size_t count = _owner.items.count();
	std::vector<std::wstring> texts;
	texts.reserve(count);
	for (UINT i = 0; i < count; ++i)
		texts.emplace_back(_owner.items[i].text(_index));
	return texts;
}

std::vector<std::wstring> ListView::Column::selected_item_texts() const {
	std::vector<std::wstring> texts;
	texts.reserve(_owner.items.selected_count());

	int idx = -1;
	for (;;) {
		idx = ListView_GetNextItem(_owner.hwnd(), idx, LVNI_SELECTED);
		if (idx == -1) break;
		texts.emplace_back(_owner.items[idx].text(_index));
	}
	return texts;
}

int ListView::Column::justif() const {
	HWND hHeader = ListView_GetHeader(_owner.hwnd());

	HDITEMW hdi{.mask = HDI_FORMAT};
	Header_GetItem(hHeader, _index, &hdi);

	return hdi.fmt & (HDF_LEFT | HDF_CENTER | HDF_RIGHT); // filter out
}

const ListView::Column& ListView::Column::set_justif(WORD hdf) const {
	HWND hHeader = ListView_GetHeader(_owner.hwnd());

	HDITEMW hdi{.mask = HDI_FORMAT};
	Header_GetItem(hHeader, _index, &hdi); // first, retrieve current

	hdi.fmt &= ~(HDF_CENTER | HDF_LEFT | HDF_RIGHT); // clear all three
	hdi.fmt |= (hdf & (HDF_LEFT | HDF_CENTER | HDF_RIGHT)); // sanitize
	Header_SetItem(hHeader, _index, &hdi);

	return *this;
}

WORD ListView::Column::sort_arrow() const {
	HWND hHeader = ListView_GetHeader(_owner.hwnd());

	HDITEMW hdi{.mask = HDI_FORMAT};
	Header_GetItem(hHeader, _index, &hdi);

	return hdi.fmt & (HDF_SORTDOWN | HDF_SORTUP); // filter out
}

const ListView::Column& ListView::Column::set_sort_arrow(WORD hdf) const {
	HWND hHeader = ListView_GetHeader(_owner.hwnd());
	UINT numCols = Header_GetItemCount(hHeader);

	for (UINT i = 0; i < numCols; ++i) {
		HDITEMW hdi{.mask = HDI_FORMAT};
		Header_GetItem(hHeader, i, &hdi); // first, retrieve current

		hdi.fmt &= ~(HDF_SORTDOWN | HDF_SORTUP); // clear all two
		if (i == _index) // only the targeted column will have the flag set
			hdi.fmt |= (hdf & (HDF_SORTDOWN | HDF_SORTUP)); // sanitize

		Header_SetItem(hHeader, i, &hdi);
	}

	return *this;
}

std::wstring ListView::Column::text() const {
	std::wstring buf(64, L'\0'); // arbitrary

	LVCOLUMNW lvc{
		.mask = LVCF_TEXT,
		.pszText = buf.data(),
		.cchTextMax = static_cast<int>(buf.size()),
	};

	ListView_GetColumn(_owner.hwnd(), _index, &lvc);
	str::trim_nulls(buf);
	return buf;
}

const ListView::Column& ListView::Column::set_text(WStrView newText) const {
	LVCOLUMNW lvc{
		.mask = LVCF_TEXT,
		.pszText = const_cast<LPWSTR>(newText.c_str()),
	};
	ListView_SetColumn(_owner.hwnd(), _index, &lvc);
	return *this;
}

UINT ListView::Column::width() const {
	return ListView_GetColumnWidth(_owner.hwnd(), _index);
}

const ListView::Column& ListView::Column::set_width(UINT width) const {
	ListView_SetColumnWidth(_owner.hwnd(), _index, width);
	return *this;
}

const ListView::Column& ListView::Column::set_width_to_fill() const {
	size_t numCols = _owner.cols.count();
	if (numCols == 0) return *this;

	UINT cxUsed = 0;
	for (UINT i = 0; i < numCols; ++i) {
		if (i != _index)
			cxUsed += _owner.cols[i].width(); // retrieve cx of each column, but us
	}

	RECT rc{};
	GetClientRect(_owner.hwnd(), &rc); // list view client area
	return set_width(rc.right - cxUsed);
}

//------------------------------------------------------------------------------

ListView::Column ListView::ColumnCollection::add(WStrView text, UINT width) const {
	LVCOLUMNW lvc{
		.mask = LVCF_TEXT | LVCF_WIDTH,
		.cx = static_cast<int>(width),
		.pszText = const_cast<LPWSTR>(text.c_str()),
	};
	int index = ListView_InsertColumn(_owner.hwnd(), 0xffff, &lvc); // insert as the last column
	return {_owner, index}; // return newly added column
}

size_t ListView::ColumnCollection::count() const {
	HWND hHeader = ListView_GetHeader(_owner.hwnd());
	return Header_GetItemCount(hHeader);
}

//------------------------------------------------------------------------------

bool ListView::Item::is_focused() const {
	return ListView_GetItemState(_owner.hwnd(), _index, LVIS_FOCUSED) & LVIS_FOCUSED;
}

const ListView::Item& ListView::Item::focus() const {
	ListView_SetItemState(_owner.hwnd(), _index, LVIS_FOCUSED, LVIS_FOCUSED);
	return *this;
}

int ListView::Item::icon_index() const {
	#ifdef _DEBUG
	if (!_owner._imgList16.himagelist()
			|| !_owner._imgList16.count()
			|| !_owner._imgList32.himagelist()
			|| !_owner._imgList32.count())
		throw std::logic_error{"No icons have been added to any image list."};
	#endif

	LVITEMW lvi{
		.mask = LVIF_IMAGE,
		.iItem = _index,
	};
	ListView_GetItem(_owner.hwnd(), &lvi);
	return lvi.iImage;
}

const ListView::Item& ListView::Item::set_icon_index(int iconIndex) const {
	#ifdef _DEBUG
	if (!_owner._imgList16.himagelist()
			|| !_owner._imgList16.count()
			|| !_owner._imgList32.himagelist()
			|| !_owner._imgList32.count())
		throw std::logic_error{"No icons have been added to any image list."};
	#endif

	LVITEMW lvi{
		.mask = LVIF_IMAGE,
		.iItem = _index,
		.iImage = iconIndex,
	};
	ListView_SetItem(_owner.hwnd(), &lvi);
	return *this;
}

const ListView::Item& ListView::Item::remove() const {
	ListView_DeleteItem(_owner.hwnd(), _index);
	return *this;
}

bool ListView::Item::is_selected() const {
	return ListView_GetItemState(_owner.hwnd(), _index, LVIS_SELECTED) & LVIS_SELECTED;
}

const ListView::Item& ListView::Item::select(bool doSelect) const {
	ListView_SetItemState(_owner.hwnd(), _index, doSelect ? LVIS_SELECTED : 0, LVIS_SELECTED);
	return *this;
}

std::wstring ListView::Item::text(UINT columnIndex) const {
	UINT curBufSz = str::SSO_LEN;
	std::wstring buf(curBufSz, L'\0');

	for (;;) {
		LVITEMW lvi{
			.mask = LVIF_TEXT,
			.iSubItem = static_cast<int>(columnIndex),
			.pszText = buf.data(),
			.cchTextMax = static_cast<int>(curBufSz),
		};

		UINT recvChars = static_cast<UINT>(
			SendMessageW(_owner.hwnd(), LVM_GETITEMTEXTW, _index, reinterpret_cast<LPARAM>(&lvi)));
		recvChars += 1; // plus terminating null count

		if (recvChars < curBufSz) { // to break, must have at least 1 char gap
			str::trim_nulls(buf);
			return buf;
		}

		curBufSz *= 2; // double the buffer size to try again
		buf.resize(curBufSz, L'\0');
	}
}

const ListView::Item& ListView::Item::set_text(WStrView newText, UINT columnIndex) const {
	ListView_SetItemText(_owner.hwnd(), _index, columnIndex, const_cast<LPWSTR>(newText.c_str()));
	return *this;
}

UINT ListView::Item::unique_id() const {
	return ListView_MapIndexToID(_owner.hwnd(), _index);
}

bool ListView::Item::is_visible() const {
	return ListView_IsItemVisible(_owner.hwnd(), _index);
}

LPARAM ListView::Item::raw_data() const {
	LVITEMW lvi{
		.mask = LVIF_PARAM,
		.iItem = _index,
	};
	ListView_GetItem(_owner.hwnd(), &lvi);
	return lvi.lParam;
}

const ListView::Item& ListView::Item::set_raw_data(LPARAM data) const {
	LVITEMW lvi{
		.mask = LVIF_PARAM,
		.iItem = _index,
		.lParam = data,
	};
	ListView_SetItem(_owner.hwnd(), &lvi);
	return *this;
}

//------------------------------------------------------------------------------

ListView::Item ListView::ItemCollection::add(WStrView text,
	std::initializer_list<WStrView> otherColumnsTexts, int iconIndex) const
{
	LVITEMW lvi{
		.mask = LVIF_TEXT | static_cast<UINT>(iconIndex != -1 ? LVIF_IMAGE : 0),
		.iItem = 0x0fff'ffff, // insert as the last item
		.pszText = const_cast<LPWSTR>(text.c_str()),
		.iImage = iconIndex,
	};
	int index = ListView_InsertItem(_owner.hwnd(), &lvi);
	Item newItem{_owner, index};

	for (auto colText = otherColumnsTexts.begin(); colText != otherColumnsTexts.end(); ++colText) {
		size_t idx = std::distance(otherColumnsTexts.begin(), colText);
		newItem.set_text(*colText, static_cast<UINT>(idx) + 1);
	}

	return newItem; // return newly added item
}

size_t ListView::ItemCollection::count() const {
	return ListView_GetItemCount(_owner.hwnd());
}

void ListView::ItemCollection::delete_all() const {
	ListView_DeleteAllItems(_owner.hwnd());
}

void ListView::ItemCollection::delete_selected() const {
	for (;;) {
		int idxFound = ListView_GetNextItem(_owner.hwnd(), -1, LVNI_SELECTED); // always search first one
		if (idxFound == -1) break;
		ListView_DeleteItem(_owner.hwnd(), idxFound);
	}
}

std::optional<ListView::Item> ListView::ItemCollection::focused() const {
	int idxFound = ListView_GetNextItem(_owner.hwnd(), -1, LVNI_FOCUSED);
	return idxFound == -1 ? std::nullopt : std::make_optional<Item>(_owner, idxFound);
}

std::optional<ListView::Item> ListView::ItemCollection::get_by_unique_id(UINT uid) const {
	int idx = ListView_MapIDToIndex(_owner.hwnd(), uid);
	return idx == -1 ? std::nullopt : std::make_optional<Item>(_owner, idx);
}

std::optional<ListView::Item> ListView::ItemCollection::hit_test(POINT pt) const {
	LVHITTESTINFO lvhti{.pt = pt};
	int idxFound = ListView_HitTestEx(_owner.hwnd(), &lvhti);
	return idxFound == -1 ? std::nullopt : std::make_optional<Item>(_owner, idxFound);
}

void ListView::ItemCollection::select_all(bool doSelect) const {
	if (GetWindowLongPtrW(_owner.hwnd(), GWL_STYLE) & LVS_SINGLESEL) [[unlikely]] {
		return; // single-sel list views cannot have all items selected
	}
	ListView_SetItemState(_owner.hwnd(), -1, doSelect ? LVIS_SELECTED : 0, LVIS_SELECTED);
}

std::vector<ListView::Item> ListView::ItemCollection::selected() const {
	std::vector<Item> items;
	items.reserve(selected_count());

	int idx = -1;
	for (;;) {
		idx = ListView_GetNextItem(_owner.hwnd(), idx, LVNI_SELECTED);
		if (idx == -1) break;
		items.emplace_back(_owner, idx);
	}
	return items;
}

size_t ListView::ItemCollection::selected_count() const {
	return ListView_GetSelectedCount(_owner.hwnd());
}

void ListView::ItemCollection::sort(std::function<int(Item, Item)> cb) const {
	struct Info final {
		const ListView &owner;
		std::function<int(Item, Item)> cb;
	};
	Info nfo = {
		.owner = _owner,
		.cb = std::move(cb),
	};

	ListView_SortItemsEx(_owner.hwnd(), [](LPARAM idxA, LPARAM idxB, LPARAM lp) -> int { // receives indexes
		Info* pNfo = reinterpret_cast<Info*>(lp);
		return pNfo->cb(
			pNfo->owner.items[static_cast<int>(idxA)],
			pNfo->owner.items[static_cast<int>(idxB)]);
	}, &nfo);
}

std::optional<ListView::Item> ListView::ItemCollection::topmost_visible() const {
	int idx = ListView_GetTopIndex(_owner.hwnd());
	return idx == -1 ? std::nullopt : std::make_optional<Item>(_owner, idx);
}

//------------------------------------------------------------------------------

ListView::ListView(IWindowParent &owner, ListViewOpts creationOpts)
	: _ctrl{owner.base()}, _events{owner.base(), valid_ctrl_id(creationOpts.ctrlId)}
{
	load_context_menu(creationOpts.contextMenuId);

	_ctrl._parent._preEvents.wm_create_or_init_dialog(
		[this, pOwner = &owner, opts = std::move(creationOpts)]() mutable -> void {
			if (opts.size == ListViewOpts{}.size)
				opts.size = dpi::sz(opts.size); // special case: default size

			_ctrl.create_wnd(ctrl_id(), opts.styleEx, WC_LISTVIEWW, {},
				opts.style | LVS_SHAREIMAGELISTS, opts.pos, opts.size);
			if (opts.styleExListView)
				set_extended_style(true, opts.styleExListView);
			_ctrl._parent._layout.add(hwnd(), opts.layout);
			for (auto &&c : opts.cols)
				cols.add(c.text, c.width);
		});

	custom_events();
}

ListView::ListView(IWindowParent &owner, WORD ctrlId, Lay layout, WORD contextMenuId)
	: _ctrl{owner.base()}, _events{owner.base(), ctrlId}
{
	load_context_menu(contextMenuId);

	_ctrl._parent._preEvents.wm_create_or_init_dialog([this, layout]() -> void {
		_ctrl.assign_dlg(ctrl_id());
		SetWindowLongPtrW(hwnd(), GWL_STYLE, GetWindowLongPtrW(hwnd(), GWL_STYLE) | LVS_SHAREIMAGELISTS);
		_ctrl._parent._layout.add(hwnd(), layout);
	});

	custom_events();
}

const ListView& ListView::set_extended_style(bool doSet, DWORD exStyle) const {
	ListView_SetExtendedListViewStyleEx(hwnd(), exStyle, doSet ? exStyle : 0);
	return *this;
}

IStoreIcon& ListView::icons_16() {
	if (!_imgList16.himagelist()) { // not created yet?
		_imgList16.create();
		ListView_SetImageList(hwnd(), _imgList16.himagelist(), LVSIL_SMALL);
	}
	return _imgList16;
}

IStoreIcon& ListView::icons_32() {
	if (!_imgList32.himagelist()) { // not created yet?
		_imgList32.create();
		ListView_SetImageList(hwnd(), _imgList32.himagelist(), LVSIL_NORMAL);
	}
	return _imgList32;
}

void ListView::load_context_menu(WORD contextMenuId) {
	if (!contextMenuId) return;

	_hMenuContext = LoadMenuW(GetModuleHandleW(nullptr), MAKEINTRESOURCEW(contextMenuId));
	#ifdef _DEBUG
	if (!_hMenuContext)
		throw std::system_error(GetLastError(), std::system_category(), "LoadMenu failed to load ListView context menu");
	#endif
}

void ListView::custom_events() {
	_ctrl._subclassEvents.wm_get_dlg_code([this](wm::GetDlgCode p) -> WORD {
		if (!p.is_query() && p.msg()->message == WM_KEYDOWN && p.vkey_code() == VK_RETURN) { // Enter key
			NMLVKEYDOWN nmlvkd{
				.hdr{
					.hwndFrom = hwnd(),
					.idFrom = ctrl_id(),
					.code = LVN_KEYDOWN,
				},
				.wVKey = VK_RETURN,
			};
			HWND hParent = GetAncestor(hwnd(), GA_PARENT);
			SendMessageW(hParent, WM_NOTIFY, ctrl_id(), reinterpret_cast<LPARAM>(&nmlvkd)); // send Enter key to parent
		}
		return static_cast<WORD>(DefSubclassProc(hwnd(), WM_GETDLGCODE, p.wparam(), p.lparam())); // let system define DLGC
	});

	_ctrl._parent._preEvents.wm_notify(ctrl_id(), LVN_KEYDOWN, [this](wm::Notify p) -> void {
		NMLVKEYDOWN &nmk = p.hdr<NMLVKEYDOWN>();
		bool hasCtrl = GetAsyncKeyState(VK_CONTROL) & 0x8000;

		if (hasCtrl && nmk.wVKey == 'A') { // Ctrl+A pressed?
			items.select_all(true);
		} else if (nmk.wVKey == VK_APPS) { // context menu key?
			bool hasShift = GetAsyncKeyState(VK_SHIFT) & 0x8000;
			show_context_menu(false, hasCtrl, hasShift);
		}
	});

	_ctrl._parent._preEvents.wm_notify(ctrl_id(), NM_RCLICK, [this](wm::Notify p) -> void {
		NMITEMACTIVATE &nmi = p.hdr<NMITEMACTIVATE>();
		bool hasCtrl = nmi.uKeyFlags & LVKF_CONTROL;
		bool hasShift = nmi.uKeyFlags & LVKF_SHIFT;

		show_context_menu(true, hasCtrl, hasShift);
	});

	_ctrl._parent._postEvents.wm(WM_DESTROY, [this](wm::Msg) -> void {
		if (_hMenuContext)
			DestroyMenu(_hMenuContext);
	});
}

void ListView::show_context_menu(bool followCursor, bool hasCtrl, bool hasShift) {
	if (!_hMenuContext) return;

	POINT menuPos{};
	if (followCursor) { // usually when fired by a right-click
		GetCursorPos(&menuPos); // relative to screen
		ScreenToClient(hwnd(), &menuPos); // now relative to listview
		std::optional<Item> itemOver = items.hit_test(menuPos);
		if (!itemOver.has_value()) { // no item was right-clicked
			items.select_all(false);
		} else if (!hasCtrl && !hasShift) {
			itemOver.value().select(true).focus(); // if note yet
		}
		SetFocus(hwnd()); // because a right-click won't set the focus by itself
	} else { // usually fired by the context meny key
		std::optional<Item> itemFocused = items.focused();
		if (itemFocused.has_value() && itemFocused.value().is_visible()) {
			RECT rc{.left = LVIR_BOUNDS};
			SendMessageW(hwnd(), LVM_GETITEMRECT, itemFocused.value().index(), reinterpret_cast<LPARAM>(&rc));
			menuPos = {.x = rc.left + 16, .y = rc.top + (rc.bottom - rc.top) / 2};
		} else { // no item is focused and visible
			menuPos = {.x = 6, .y = 10}; // arbitrary coordinates
		}
	}

	HWND hParent = GetParent(hwnd());
	HMENU hSubMenu = GetSubMenu(_hMenuContext, 0); // pop the first submenu
	#ifdef _DEBUG
	if (!hSubMenu)
		throw std::invalid_argument{"GetSubMenu failed to load ListView context submenu."};
	#endif
	ClientToScreen(hwnd(), &menuPos); // from listview to screen
	SetForegroundWindow(hParent);
	BOOL retTrk = TrackPopupMenu(hSubMenu, TPM_LEFTBUTTON, menuPos.x, menuPos.y, 0, hParent, nullptr);
	#ifdef _DEBUG
	if (!retTrk)
		throw std::runtime_error{"TrackPopupMenu failed to load ListView context submenu."};
	#endif
	PostMessageW(hParent, WM_NULL, 0, 0); // necessary according to TrackPopupMenu docs
}

////////////////////////////////////////////////////////////////////////////////

MonthCalendar::MonthCalendar(IWindowParent &owner, wl::MonthCalendarOpts creationOpts)
	: _ctrl{owner.base()}, _events{owner.base(), valid_ctrl_id(creationOpts.ctrlId)}
{
	_ctrl._parent._preEvents.wm_create_or_init_dialog(
		[this, pOwner = &owner, opts = std::move(creationOpts)]() -> void {
			_ctrl.create_wnd(ctrl_id(), opts.styleEx, MONTHCAL_CLASSW, {}, opts.style, opts.pos, {});

			RECT rcBounds{};
			SendMessageW(hwnd(), MCM_GETMINREQRECT, 0, reinterpret_cast<LPARAM>(&rcBounds)); // request ideal size
			SetWindowPos(hwnd(), nullptr, 0, 0, rcBounds.right, rcBounds.bottom, SWP_NOZORDER | SWP_NOMOVE);

			if (opts.value.wYear && opts.value.wMonth)
				set_value(opts.value);

			_ctrl._parent._layout.add(hwnd(), opts.layout);
		});
}

MonthCalendar::MonthCalendar(IWindowParent &owner, WORD ctrlId, Lay layout)
	: _ctrl{owner.base()}, _events{owner.base(), ctrlId}
{
	_ctrl._parent._preEvents.wm_create_or_init_dialog([this, layout]() -> void {
		_ctrl.assign_dlg(ctrl_id());
		_ctrl._parent._layout.add(hwnd(), layout);
	});
}

SYSTEMTIME MonthCalendar::value() const {
	SYSTEMTIME st{};
	SendMessageW(hwnd(), MCM_GETCURSEL, 0, reinterpret_cast<LPARAM>(&st));
	return st;
}

const MonthCalendar& MonthCalendar::set_value(const SYSTEMTIME &st) const {
	SendMessageW(hwnd(), MCM_SETCURSEL, 0, reinterpret_cast<LPARAM>(&st));
	return *this;
}

////////////////////////////////////////////////////////////////////////////////

RadioButton::RadioButton(IWindowParent &owner, RadioButtonOpts creationOpts)
	: _ctrl{owner.base()}, _events{owner.base(), valid_ctrl_id(creationOpts.ctrlId)}
{
	_ctrl._parent._preEvents.wm_create_or_init_dialog(
		[this, pOwner = &owner, opts = std::move(creationOpts)]() mutable -> void {
			if (!opts.size.cx && !opts.size.cy)
				opts.size = calc_text_bound_box_with_check(str::remove_accel_ampersands(opts.text));

			_ctrl.create_wnd(ctrl_id(), opts.styleEx, L"BUTTON", std::move(opts.text),
				opts.style, opts.pos, opts.size);
			apply_ui_font(hwnd());
			_ctrl._parent._layout.add(hwnd(), opts.layout);
			if (opts.selected) select();
		});
}

RadioButton::RadioButton(IWindowParent &owner, WORD ctrlId, Lay layout)
	: _ctrl{owner.base()}, _events{owner.base(), ctrlId}
{
	_ctrl._parent._preEvents.wm_create_or_init_dialog([this, layout]() -> void {
		_ctrl.assign_dlg(ctrl_id());
		_ctrl._parent._layout.add(hwnd(), layout);
	});
}

bool RadioButton::is_selected() const {
	return SendMessageW(hwnd(), BM_GETCHECK, 0, 0) == BST_CHECKED;
}

const RadioButton& RadioButton::select() const {
	SendMessageW(hwnd(), BM_SETCHECK, BST_CHECKED, 0);
	return *this;
}

const RadioButton& RadioButton::set_text(WStrView newText) const {
	set_wnd_text(hwnd(), newText);
	return *this;
}

const RadioButton& RadioButton::set_text_resize(WStrView newText) const {
	set_text(newText);
	SIZE bounds = calc_text_bound_box_with_check(str::remove_accel_ampersands(newText));
	SetWindowPos(hwnd(), nullptr, 0, 0, bounds.cx, bounds.cy, SWP_NOZORDER | SWP_NOMOVE);
	return *this;
}

////////////////////////////////////////////////////////////////////////////////

RadioGroup::RadioGroup(IWindowParent &owner, std::initializer_list<RadioButtonOpts> creationOpts)
	: _owner{owner}, _radios{creationOpts.size()}, _events{*this}
{
	#ifdef _DEBUG
	if (!creationOpts.size())
		throw std::invalid_argument{"Cannot create a RadioGroup with zero radio controls."};
	#endif

	size_t i = 0;
	for (auto &&opt : creationOpts) {
		RadioButtonOpts fixedOpts{opt};
		if (!i) fixedOpts.style |= WS_GROUP; // first radio of the group
		else    fixedOpts.style &= ~WS_GROUP;

		new (&_radios[i++]) RadioButton{owner, std::move(fixedOpts)}; // invoke constructor manually
	}
}

RadioGroup::RadioGroup(IWindowParent &owner, Lay layout, std::initializer_list<WORD> ctrlIds)
	: _owner{owner}, _radios{ctrlIds.size()}, _events{*this}
{
	#ifdef _DEBUG
	if (!ctrlIds.size())
		throw std::invalid_argument{"Cannot create a RadioGroup with zero radio controls."};
	#endif

	size_t i = 0;
	for (WORD ctrlId : ctrlIds)
		new (&_radios[i++]) RadioButton{owner, ctrlId, layout}; // invoke constructor manually
}

////////////////////////////////////////////////////////////////////////////////

Static::Static(IWindowParent &owner, StaticOpts creationOpts)
	: _ctrl{owner.base()}, _events{owner.base(), valid_ctrl_id(creationOpts.ctrlId)}
{
	_ctrl._parent._preEvents.wm_create_or_init_dialog(
		[this, pOwner = &owner, opts = std::move(creationOpts)]() mutable -> void {
			if (!opts.size.cx && !opts.size.cy)
				opts.size = calc_text_bound_box(str::remove_accel_ampersands(opts.text));

			_ctrl.create_wnd(ctrl_id(), opts.styleEx, L"STATIC", std::move(opts.text),
				opts.style, opts.pos, opts.size);
			apply_ui_font(hwnd());
			_ctrl._parent._layout.add(hwnd(), opts.layout);
		});
}

Static::Static(IWindowParent &owner, WORD ctrlId, Lay layout)
	: _ctrl{owner.base()}, _events{owner.base(), ctrlId}
{
	_ctrl._parent._preEvents.wm_create_or_init_dialog([this, layout]() -> void {
		_ctrl.assign_dlg(ctrl_id());
		_ctrl._parent._layout.add(hwnd(), layout);
	});
}

const Static& Static::set_text(WStrView newText) const {
	set_wnd_text(hwnd(), newText);
	return *this;
}

const Static& Static::set_text_resize(WStrView newText) const {
	set_text(newText);
	SIZE bounds = calc_text_bound_box(str::remove_accel_ampersands(newText));
	SetWindowPos(hwnd(), nullptr, 0, 0, bounds.cx, bounds.cy, SWP_NOZORDER | SWP_NOMOVE);
	return *this;
}

////////////////////////////////////////////////////////////////////////////////

std::wstring StatusBar::Part::text() const {
	LRESULT len = SendMessageW(_owner.hwnd(), SB_GETTEXTLENGTHW, 0, 0);
	std::wstring buf(len + 1, L'\0');
	SendMessageW(_owner.hwnd(), SB_GETTEXTW, _index, reinterpret_cast<LPARAM>(buf.data()));
	buf.resize(len);
	return buf;
}

const StatusBar::Part& StatusBar::Part::set_text(WStrView newText) const {
	SendMessageW(_owner.hwnd(), SB_SETTEXTW, MAKELONG(_index, 0),
		reinterpret_cast<LPARAM>(newText.c_str()));
	return *this;
}

const StatusBar::Part& StatusBar::Part::set_icon_index(int iconIndex) const {
	#ifdef _DEBUG
	if (!_owner._iconStore16.count())
		throw std::logic_error{"No icons have been added to the icon store."};
	#endif

	SendMessageW(_owner.hwnd(), SB_SETICON, _index,
		reinterpret_cast<LPARAM>(_owner._iconStore16[iconIndex]));
	return *this;
}

//------------------------------------------------------------------------------

StatusBar::StatusBar(IWindowParent &owner, StatusBarOpts creationOpts) :
	_ctrl{owner.base()},
	_events{owner.base(), valid_ctrl_id(creationOpts.ctrlId)},
	_parts{std::move(creationOpts.parts)}
{
	_rightEdges.resize(_parts.size(), 0);
	for (auto &&ico : creationOpts.icons) {
		if (ico.id) _iconStore16.add_resource(ico.id);
		else        _iconStore16.add_shell_ext(ico.ext);
	}

	_ctrl._parent._preEvents.wm_create_or_init_dialog([this, pOwner = &owner]() -> void {
		DWORD parentStyle = static_cast<DWORD>(GetWindowLongPtrW(pOwner->hwnd(), GWL_STYLE));
		bool isParentResizable = (parentStyle & WS_MAXIMIZEBOX) || (parentStyle & WS_SIZEBOX);
		DWORD style = WS_CHILD | WS_VISIBLE | SBARS_TOOLTIPS | (isParentResizable ? SBARS_SIZEGRIP : 0);
		_ctrl.create_wnd(ctrl_id(), WS_EX_LEFT, STATUSCLASSNAMEW, {}, style, {}, {});

		RECT rcParent{};
		GetClientRect(pOwner->hwnd(), &rcParent);
		resize_to_fit_parent(wm::Msg{WM_SIZE, SIZE_RESTORED, MAKELPARAM(rcParent.right, 0)}); // will create the parts

		for (UINT i = 0; i < _parts.size(); ++i) { // add text
			if (!_parts[i].text.empty())
				parts[i].set_text(_parts[i].text);
		}
	});

	_ctrl._parent._postEvents.wm_create_or_init_dialog([this]() -> void {
		for (UINT i = 0; i < _parts.size(); ++i) { // icons are manually added by user in WM_CREATE/WM_INITDIALOG
			if (_parts[i].iconIndex != -1)
				parts[i].set_icon_index(_parts[i].iconIndex);
		}
	});

	_ctrl._parent._preEvents.wm(WM_SIZE, [this](wm::Size p) -> void {
		resize_to_fit_parent(p);
	});
}

void StatusBar::resize_to_fit_parent(wm::Size p) {
	if (p.is_minimized() || !hwnd()) return;

	SendMessageW(hwnd(), WM_SIZE, 0, 0); // tell status bar to fit parent
	if (_parts.empty()) return;

	int totalWeight = 0; // total weight of all flex parts
	int cxVariable = p.sz().cx; // remaning width to be divided among flex parts
	for (int i = 0; i < _parts.size(); ++i) {
		if (parts[i].is_fixed_width())
			cxVariable -= _parts[i].width; // decrease available width room
		else
			totalWeight += _parts[i].flex;
	}

	int cxTotal = p.sz().cx;
	for (int i = static_cast<int>(_parts.size()); i-- > 0; ) { // fill right edges array with the right edge of each part
		_rightEdges[i] = cxTotal;
		if (parts[i].is_fixed_width())
			cxTotal -= _parts[i].width;
		else
			cxTotal -= (cxVariable / totalWeight) * _parts[i].flex;
	}
	SendMessageW(hwnd(), SB_SETPARTS, _rightEdges.size(), reinterpret_cast<LPARAM>(_rightEdges.data()));
}

////////////////////////////////////////////////////////////////////////////////

const Tab::Item& Tab::Item::select() const {
	TabCtrl_SetCurSel(_owner.hwnd(), _index);
	return *this;
}

std::wstring Tab::Item::text() const {
	std::wstring buf(128, L'\0'); // arbitrary

	TCITEMW tci{
		.mask = TCIF_TEXT,
		.pszText = buf.data(),
		.cchTextMax = static_cast<int>(buf.size()),
	};

	TabCtrl_GetItem(_owner.hwnd(), _index, &tci);
	str::trim_nulls(buf);
	return buf;
}

const Tab::Item& Tab::Item::set_text(WStrView newText) const {
	TCITEMW tci{
		.mask = TCIF_TEXT,
		.pszText = const_cast<LPWSTR>(newText.c_str()),
	};
	TabCtrl_SetItem(_owner.hwnd(), _index, &tci);
	return *this;
}

//------------------------------------------------------------------------------

size_t Tab::ItemCollection::count() const {
	return TabCtrl_GetItemCount(_owner.hwnd());
}

std::optional<Tab::Item> Tab::ItemCollection::focused() const {
	int idxFound = TabCtrl_GetCurFocus(_owner.hwnd());
	return idxFound == -1 ? std::nullopt : std::make_optional(Item{_owner, idxFound});
}

std::optional<Tab::Item> Tab::ItemCollection::selected() const {
	int idxFound = TabCtrl_GetCurSel(_owner.hwnd());
	return idxFound == -1 ? std::nullopt : std::make_optional(Item{_owner, idxFound});
}

//------------------------------------------------------------------------------

Tab::Tab(IWindowParent &owner, TabOpts creationOpts) :
	_ctrl{owner.base()},
	_events{owner.base(), valid_ctrl_id(creationOpts.ctrlId)},
	_children{creationOpts.titles.size()}
{
	#ifdef _DEBUG
	if (creationOpts.titles.empty())
		throw std::invalid_argument{"Cannot create a Tab with zero items."};
	else if (creationOpts.titles.size() > 100) // arbitrary
		throw std::invalid_argument{"Cannot create a Tab with more than 100 items."};
	#endif

	for (size_t i = 0; i < creationOpts.titles.size(); ++i)
		new (&_children[i]) WindowControl{owner, ControlOpts{}}; // invoke constructor manually

	_ctrl._parent._preEvents.wm_create_or_init_dialog(
		[this, pOwner = &owner, opts = std::move(creationOpts)]() mutable -> void {
			if (opts.size == TabOpts{}.size)
				opts.size = dpi::sz(opts.size); // special case: default size

			_ctrl.create_wnd(ctrl_id(), opts.styleEx, WC_TABCONTROLW, {},
				opts.style, opts.pos, opts.size);
			apply_ui_font(hwnd());
			if (opts.styleExTab)
				set_extended_style(true, opts.styleExTab);
			_ctrl._parent._layout.add(hwnd(), opts.layout);
			for (auto &&title : opts.titles)
				create_tab(title);
			if (opts.selected)
				items[static_cast<int>(opts.selected)].select();
			display_cur_tab();
		});

	custom_events();
}

Tab::Tab(IWindowParent &owner, WORD ctrlId, Lay layout,
	std::initializer_list<WORD> childrenDlgIds, std::initializer_list<WStrView> titles)
	: _ctrl{owner.base()}, _events{owner.base(), ctrlId}, _children{childrenDlgIds.size()}
{
	#ifdef _DEBUG
	if (childrenDlgIds.size() != titles.size())
		throw std::invalid_argument{"You must provide the same number of childrenDlgIds and titles."};
	else if (!childrenDlgIds.size())
		throw std::invalid_argument{"Cannot create a Tab with zero items."};
	else if (childrenDlgIds.size() > 100) // arbitrary
		throw std::invalid_argument{"Cannot create a Tab with more than 100 items."};
	#endif

	size_t i = 0;
	for (WORD dlgId : childrenDlgIds)
		new (&_children[i]) WindowControl{owner, dlgId}; // invoke constructor manually

	_ctrl._parent._preEvents.wm_create_or_init_dialog(
		[this, layout, titles = std::move(titles)]() -> void {
			_ctrl.assign_dlg(ctrl_id());
			apply_ui_font(hwnd());
			_ctrl._parent._layout.add(hwnd(), layout);
			for (auto &&title : titles)
				create_tab(title);
			display_cur_tab(); // 1st tab will be selected by default
		});

	custom_events();
}

const Tab& Tab::set_extended_style(bool doSet, DWORD exStyle) const {
	SendMessageW(hwnd(), TCM_SETEXTENDEDSTYLE, exStyle, doSet ? exStyle : 0);
	return *this;
}

void Tab::create_tab(WStrView title) const {
	TCITEMW tci{
		.mask = TCIF_TEXT,
		.pszText = const_cast<wchar_t*>(title.c_str()),
	};
	TabCtrl_InsertItem(hwnd(), 0x0fff'ffff, &tci);
}

void Tab::display_cur_tab() const {
	if (_children.empty()) return;

	std::optional<Tab::Item> selTab = items.selected();
	if (!selTab.has_value()) return;
	size_t index = selTab.value().index();

	for (size_t i = 0; i < _children.size(); ++i) {
		if (i != index) ShowWindow(_children[i].hwnd(), SW_HIDE); // hide all others
	}

	RECT rcTab{};
	GetWindowRect(hwnd(), &rcTab);
	screen_to_client_rc(GetParent(hwnd()), &rcTab);
	TabCtrl_AdjustRect(hwnd(), FALSE, &rcTab);
	SetWindowPos(_children[index].hwnd(), nullptr,
		rcTab.left, rcTab.top, rcTab.right - rcTab.left, rcTab.bottom - rcTab.top,
		SWP_NOZORDER | SWP_SHOWWINDOW);
}

void Tab::custom_events() {
	_ctrl._parent._preEvents.wm_notify(ctrl_id(), TCN_SELCHANGE, [this](wm::Notify) -> void {
		display_cur_tab();
	});

	_ctrl._parent._preEvents.wm(WM_SIZE, [this](wm::Size p) -> void {
		display_cur_tab();
	});
}

////////////////////////////////////////////////////////////////////////////////

Trackbar::Trackbar(IWindowParent &owner, TrackbarOpts creationOpts)
	: _ctrl{owner.base()}, _events{owner.base(), valid_ctrl_id(creationOpts.ctrlId)}
{
	_ctrl._parent._preEvents.wm_create_or_init_dialog(
		[this, pOwner = &owner, opts = std::move(creationOpts)]() mutable -> void {
			if (opts.size == TrackbarOpts{}.size)
				opts.size = dpi::sz(opts.size); // special case: default size

			_ctrl.create_wnd(ctrl_id(), opts.styleEx, TRACKBAR_CLASSW, {},
				opts.style, opts.pos, opts.size);
			_ctrl._parent._layout.add(hwnd(), opts.layout);
			if (opts.range != std::pair{0, 100}) set_range(opts.range);
			if (opts.pageSize) set_page_size(opts.pageSize);
			if (opts.value) set_pos(opts.value);
		});
}

Trackbar::Trackbar(IWindowParent &owner, WORD ctrlId, Lay layout)
	: _ctrl{owner.base()}, _events{owner.base(), ctrlId}
{
	_ctrl._parent._preEvents.wm_create_or_init_dialog([this, layout]() -> void {
		_ctrl.assign_dlg(ctrl_id());
		_ctrl._parent._layout.add(hwnd(), layout);
	});
}

int Trackbar::page_size() const {
	return static_cast<int>(SendMessageW(hwnd(), TBM_GETPAGESIZE, 0, 0));
}

const Trackbar& Trackbar::set_page_size(int pageSize) const {
	SendMessageW(hwnd(), TBM_SETPAGESIZE, 0, pageSize);
	return *this;
}

int Trackbar::pos() const {
	return static_cast<int>(SendMessageW(hwnd(), TBM_GETPOS, 0, 0));
}

const Trackbar& Trackbar::set_pos(int value) const {
	SendMessageW(hwnd(), TBM_SETPOS, TRUE, value);
	return *this;
}

std::pair<int, int> Trackbar::range() const {
	int rmin = static_cast<int>(SendMessageW(hwnd(), TBM_GETRANGEMIN, 0, 0));
	int rmax = static_cast<int>(SendMessageW(hwnd(), TBM_GETRANGEMAX, 0, 0));
	return {rmin, rmax};
}

const Trackbar& Trackbar::set_range(int rangeMin, int rangeMax) const {
	SendMessageW(hwnd(), TBM_SETRANGEMIN, TRUE, rangeMin);
	SendMessageW(hwnd(), TBM_SETRANGEMAX, TRUE, rangeMax);
	return *this;
}

const Trackbar& Trackbar::set_range(std::pair<int, int> rangeMinMax) const {
	return set_range(rangeMinMax.first, rangeMinMax.second);
}

////////////////////////////////////////////////////////////////////////////////

TreeView::Item TreeView::Item::add_child(WStrView itemText, int iconIndex) const {
	TVINSERTSTRUCTW tvi{
		.hParent = _hItem,
		.hInsertAfter = TVI_LAST,
		.itemex = {
			.mask = TVIF_TEXT | static_cast<UINT>(iconIndex != -1 ? TVIF_IMAGE : 0),
			.pszText = const_cast<LPWSTR>(itemText.c_str()),
			.iImage = iconIndex,
		},
	};
	HTREEITEM hItemNew = TreeView_InsertItem(_owner.hwnd(), &tvi);
	return Item{_owner, hItemNew};
}

std::vector<TreeView::Item> TreeView::Item::children() const {
	std::vector<Item> items;
	HTREEITEM hItem = nullptr;
	for (;;) {
		hItem = TreeView_GetNextItem(_owner.hwnd(), hItem, TVGN_NEXT);
		if (!hItem) break;
		items.emplace_back(_owner, hItem);
	}
	return items;
}

const TreeView::Item& TreeView::Item::remove() const {
	TreeView_DeleteItem(_owner.hwnd(), _hItem);
	return *this;
}

const TreeView::Item& TreeView::Item::ensure_visible() const {
	TreeView_EnsureVisible(_owner.hwnd(), _hItem);
	return *this;
}

bool TreeView::Item::is_expanded() const {
	return TreeView_GetItemState(_owner.hwnd(), _hItem, TVIS_EXPANDED) & TVIS_EXPANDED;
}

const TreeView::Item& TreeView::Item::expand(bool doExpand) const {
	TreeView_Expand(_owner.hwnd(), _hItem, doExpand ? TVE_EXPAND : TVE_COLLAPSE);
	return *this;
}

int TreeView::Item::icon_index() const {
	#ifdef _DEBUG
	if (!_owner._imgList16.himagelist() || !_owner._imgList16.count())
		throw std::logic_error{"No icons have been added to any image list."};
	#endif

	TVITEMEXW tvi{
		.mask = TVIF_IMAGE,
		.hItem = _hItem,
	};
	TreeView_GetItem(_owner.hwnd(), &tvi);
	return tvi.iImage;
}

const TreeView::Item& TreeView::Item::set_icon_index(int iconIndex) const {
	#ifdef _DEBUG
	if (!_owner._imgList16.himagelist() || !_owner._imgList16.count())
		throw std::logic_error{"No icons have been added to any image list."};
	#endif

	TVITEMEXW tvi{
		.mask = LVIF_IMAGE,
		.hItem = _hItem,
		.iImage = iconIndex,
	};
	TreeView_SetItem(_owner.hwnd(), &tvi);
	return *this;
}

TreeView::Item TreeView::Item::next_sibling() const {
	HTREEITEM hItem = TreeView_GetNextItem(_owner.hwnd(), _hItem, TVGN_NEXT);
	return Item{_owner, hItem};
}

TreeView::Item TreeView::Item::parent() const {
	HTREEITEM hItem = TreeView_GetNextItem(_owner.hwnd(), _hItem, TVGN_PARENT);
	return Item{_owner, hItem};
}

TreeView::Item TreeView::Item::prev_sibling() const {
	HTREEITEM hItem = TreeView_GetNextItem(_owner.hwnd(), _hItem, TVGN_PREVIOUS);
	return Item{_owner, hItem};
}

std::wstring TreeView::Item::text() const {
	std::wstring buf(128, L'\0'); // arbitrary

	TVITEMEXW tvi{
		.mask = TVIF_TEXT,
		.hItem = _hItem,
		.pszText = buf.data(),
		.cchTextMax = static_cast<int>(buf.size()),
	};

	TreeView_GetItem(_owner.hwnd(), &tvi);
	str::trim_nulls(buf);
	return buf;
}

const TreeView::Item& TreeView::Item::set_text(WStrView newText) const {
	TVITEMEXW tvi{
		.mask = TVIF_TEXT,
		.hItem = _hItem,
		.pszText = const_cast<LPWSTR>(newText.c_str()),
	};
	TreeView_SetItem(_owner.hwnd(), &tvi);
	return *this;
}

LPARAM TreeView::Item::raw_data() const {
	TVITEMEXW tvi{
		.mask = TVIF_PARAM,
		.hItem = _hItem,
	};
	TreeView_GetItem(_owner.hwnd(), &tvi);
	return tvi.lParam;
}

const TreeView::Item& TreeView::Item::set_raw_data(LPARAM data) const {
	TVITEMEXW tvi{
		.mask = TVIF_PARAM,
		.hItem = _hItem,
		.lParam = data,
	};
	TreeView_SetItem(_owner.hwnd(), &tvi);
	return *this;
}

//------------------------------------------------------------------------------

TreeView::Item TreeView::ItemCollection::add_root(WStrView text, int iconIndex) const {
	return Item{_owner, nullptr}.add_child(text, iconIndex);
}

size_t TreeView::ItemCollection::count() const {
	return TreeView_GetCount(_owner.hwnd());
}

void TreeView::ItemCollection::delete_all() const {
	TreeView_DeleteAllItems(_owner.hwnd());
}

TreeView::Item TreeView::ItemCollection::first_visible() const {
	HTREEITEM hItem = TreeView_GetFirstVisible(_owner.hwnd());
	return Item{_owner, hItem};
}

std::vector<TreeView::Item> TreeView::ItemCollection::roots() const {
	return Item{_owner, nullptr}.children();
}

TreeView::Item TreeView::ItemCollection::selected() const {
	HTREEITEM hItem = TreeView_GetSelection(_owner.hwnd());
	return Item{_owner, hItem};
}

//------------------------------------------------------------------------------

TreeView::TreeView(IWindowParent &owner, TreeViewOpts creationOpts)
	: _ctrl{owner.base()}, _events{owner.base(), valid_ctrl_id(creationOpts.ctrlId)}
{
	_ctrl._parent._preEvents.wm_create_or_init_dialog(
		[this, pOwner = &owner, opts = std::move(creationOpts)]() mutable -> void {
			if (opts.size == TreeViewOpts{}.size)
				opts.size = dpi::sz(opts.size); // special case: default size

			_ctrl.create_wnd(ctrl_id(), opts.styleEx, WC_TREEVIEWW, {},
				opts.style, opts.pos, opts.size);
			if (opts.styleExTreeView)
				set_extended_style(true, opts.styleExTreeView);
			_ctrl._parent._layout.add(hwnd(), opts.layout);
		});
}

TreeView::TreeView(IWindowParent &owner, WORD ctrlId, Lay layout)
	: _ctrl{owner.base()}, _events{owner.base(), ctrlId}
{
	_ctrl._parent._preEvents.wm_create_or_init_dialog([this, layout]() -> void {
		_ctrl.assign_dlg(ctrl_id());
		_ctrl._parent._layout.add(hwnd(), layout);
	});
}

const TreeView& TreeView::set_extended_style(bool doSet, DWORD exStyle) const {
	TreeView_SetExtendedStyle(hwnd(), doSet ? exStyle : 0, exStyle);
	return *this;
}

IStoreIcon& TreeView::icons_16() {
	if (!_imgList16.himagelist()) { // not created yet?
		_imgList16.create();
		TreeView_SetImageList(hwnd(), _imgList16.himagelist(), LVSIL_SMALL);
	}
	return _imgList16;
}
