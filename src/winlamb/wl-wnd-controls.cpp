#include <system_error>
#include "wnd-controls.h"
#include "runnable.h"
using namespace wl;
using namespace _wl_internal;

constexpr bool operator==(const POINT a, const POINT b) { return a.x == b.x && a.y == b.y; }
constexpr bool operator==(const SIZE a, const SIZE b) { return a.cx == b.cx && a.cy == b.cy; }

////////////////////////////////////////////////////////////////////////////////

Button::Button(WindowParent &owner, WORD ctrlId)
	: _ctrl{owner}, _events{owner, valid_ctrl_id(ctrlId)}
{
	_ctrl._parentWndBase._preEvents.wm_create_or_init_dialog([this, pOwner = &owner]() -> void {
		if (_opts.size == opts::ButtonOpts{}.size)
			_opts.size = dpi::sz(_opts.size); // special case: default size

		_ctrl.create_wnd(ctrl_id(), _opts.windowExStyle, L"BUTTON", _opts.text,
			_opts.windowStyle | _opts.ctrlStyle, _opts.pos, _opts.size);
		apply_ui_font(hwnd());
		_ctrl._parentWndBase._layout.add(hwnd(), _opts.layout);
	});
}

Button::Button(WindowParent &owner, WORD ctrlId, Lay layout)
	: _ctrl{owner}, _events{owner, valid_ctrl_id(ctrlId)}
{
	_ctrl._parentWndBase._preEvents.wm_create_or_init_dialog([this, layout]() -> void {
		_ctrl.assign_dlg(ctrl_id());
		_ctrl._parentWndBase._layout.add(hwnd(), layout);
	});
}

const Button& Button::set_text(WStrPtr text) const {
	set_wnd_text(hwnd(), text);
	return *this;
}

const Button& Button::trigger_click() const {
	SendMessageW(hwnd(), BM_CLICK, 0, 0);
	return *this;
}

////////////////////////////////////////////////////////////////////////////////

CheckBox::CheckBox(WindowParent &owner, WORD ctrlId)
	: _ctrl{owner}, _events{owner, valid_ctrl_id(ctrlId)}
{
	_ctrl._parentWndBase._preEvents.wm_create_or_init_dialog([this, pOwner = &owner]() -> void {
		if (!_opts.size.cx && !_opts.size.cy)
			_opts.size = calc_text_bound_box_with_check(str::remove_accel_ampersands(_opts.text));

		_ctrl.create_wnd(ctrl_id(), _opts.windowExStyle, L"BUTTON", _opts.text,
			_opts.windowStyle | _opts.ctrlStyle, _opts.pos, _opts.size);
		apply_ui_font(hwnd());
		_ctrl._parentWndBase._layout.add(hwnd(), _opts.layout);
		set_state(_opts.state);
	});
}

CheckBox::CheckBox(WindowParent &owner, WORD ctrlId, Lay layout)
	: _ctrl{owner}, _events{owner, valid_ctrl_id(ctrlId)}
{
	_ctrl._parentWndBase._preEvents.wm_create_or_init_dialog([this, layout]() -> void {
		_ctrl.assign_dlg(ctrl_id());
		_ctrl._parentWndBase._layout.add(hwnd(), layout);
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

const CheckBox& CheckBox::set_text(WStrPtr text) const {
	set_wnd_text(hwnd(), text);
	return *this;
}

const CheckBox& CheckBox::set_text_resize(WStrPtr text) const {
	set_text(text);
	SIZE bounds = calc_text_bound_box_with_check(str::remove_accel_ampersands(text));
	SetWindowPos(hwnd(), nullptr, 0, 0, bounds.cx, bounds.cy, SWP_NOZORDER | SWP_NOMOVE);
	return *this;
}

////////////////////////////////////////////////////////////////////////////////

std::wstring ComboBox::ItemCollection::operator[](int index) const {
	size_t nChars = SendMessageW(_pOwner->hwnd(), CB_GETLBTEXTLEN, index, 0);
	std::wstring s(nChars + 1, L'\0');
	SendMessageW(_pOwner->hwnd(), CB_GETLBTEXT, index, reinterpret_cast<LPARAM>(s.data()));
	s.resize(nChars);
	return s;
}

void ComboBox::ItemCollection::add(WStrPtr text) const {
	SendMessageW(_pOwner->hwnd(), CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(text.operator LPCWSTR()));
}

void ComboBox::ItemCollection::add(std::initializer_list<WStrPtr> texts) const {
	for (auto &&s : texts)
		add(s);
}

size_t ComboBox::ItemCollection::count() const {
	return SendMessageW(_pOwner->hwnd(), CB_GETCOUNT, 0, 0);
}

void ComboBox::ItemCollection::delete_all() const {
	SendMessageW(_pOwner->hwnd(), CB_RESETCONTENT, 0, 0);
}

void ComboBox::ItemCollection::select(int index) const {
	SendMessageW(_pOwner->hwnd(), CB_SETCURSEL, index, 0);
}

int ComboBox::ItemCollection::selected_index() const {
	return static_cast<int>(SendMessageW(_pOwner->hwnd(), CB_GETCURSEL, 0, 0));
}

std::optional<std::wstring> ComboBox::ItemCollection::selected_text() const {
	int selIdx = selected_index();
	return selIdx == -1 ? std::nullopt : std::make_optional(operator[](selIdx));
}

//------------------------------------------------------------------------------

ComboBox::ComboBox(WindowParent &owner, WORD ctrlId)
	: _ctrl{owner}, _events{owner, valid_ctrl_id(ctrlId)}
{
	_ctrl._parentWndBase._preEvents.wm_create_or_init_dialog([this, pOwner = &owner]() -> void {
		if (_opts.width == opts::ComboBoxOpts{}.width)
			_opts.width = dpi::x(_opts.width); // special case: default width

		_ctrl.create_wnd(ctrl_id(), _opts.windowExStyle, L"COMBOBOX", nullptr,
			_opts.windowStyle | _opts.ctrlStyle, _opts.pos, {.cx = _opts.width});
		apply_ui_font(hwnd());
		_ctrl._parentWndBase._layout.add(hwnd(), _opts.layout);
		for (auto &&s : _opts.texts)
			items.add(s);
		std::vector<std::wstring>{}.swap(_opts.texts);
	});
}

ComboBox::ComboBox(WindowParent &owner, WORD ctrlId, Lay layout)
	: _ctrl{owner}, _events{owner, valid_ctrl_id(ctrlId)}
{
	_ctrl._parentWndBase._preEvents.wm_create_or_init_dialog([this, layout]() -> void {
		_ctrl.assign_dlg(ctrl_id());
		_ctrl._parentWndBase._layout.add(hwnd(), layout);
	});
}

////////////////////////////////////////////////////////////////////////////////

DateTimePicker::DateTimePicker(WindowParent &owner, WORD ctrlId)
	: _ctrl{owner}, _events{owner, valid_ctrl_id(ctrlId)}
{
	_ctrl._parentWndBase._preEvents.wm_create_or_init_dialog([this, pOwner = &owner]() -> void {
		if (_opts.size == opts::DateTimePickerOpts{}.size)
			_opts.size = dpi::sz(_opts.size); // special case: default size

		_ctrl.create_wnd(ctrl_id(), _opts.windowExStyle, DATETIMEPICK_CLASSW, nullptr,
			_opts.windowStyle | _opts.ctrlStyle, _opts.pos, _opts.size);
		apply_ui_font(hwnd());
		_ctrl._parentWndBase._layout.add(hwnd(), _opts.layout);
		if (_opts.value.wYear)
			set_value(_opts.value);
	});
}

DateTimePicker::DateTimePicker(WindowParent &owner, WORD ctrlId, Lay layout)
	: _ctrl{owner}, _events{owner, valid_ctrl_id(ctrlId)}
{
	_ctrl._parentWndBase._preEvents.wm_create_or_init_dialog([this, layout]() -> void {
		_ctrl.assign_dlg(ctrl_id());
		_ctrl._parentWndBase._layout.add(hwnd(), layout);
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

Edit::Edit(WindowParent &owner, WORD ctrlId)
	: _ctrl{owner}, _events{owner, valid_ctrl_id(ctrlId)}
{
	_ctrl._parentWndBase._preEvents.wm_create_or_init_dialog([this, pOwner = &owner]() -> void {
		if (_opts.size == opts::EditOpts{}.size)
			_opts.size = dpi::sz(_opts.size); // special case: default size

		_ctrl.create_wnd(ctrl_id(), _opts.windowExStyle, L"EDIT", _opts.text,
			_opts.windowStyle | _opts.ctrlStyle, _opts.pos, _opts.size);
		apply_ui_font(hwnd());
		_ctrl._parentWndBase._layout.add(hwnd(), _opts.layout);
	});
}

Edit::Edit(WindowParent &owner, WORD ctrlId, Lay layout)
	: _ctrl{owner}, _events{owner, valid_ctrl_id(ctrlId)}
{
	_ctrl._parentWndBase._preEvents.wm_create_or_init_dialog([this, layout]() -> void {
		_ctrl.assign_dlg(ctrl_id());
		_ctrl._parentWndBase._layout.add(hwnd(), layout);
	});
}

const Edit& Edit::set_text(WStrPtr text) const {
	set_wnd_text(hwnd(), text);
	return *this;
}

////////////////////////////////////////////////////////////////////////////////

std::vector<std::wstring> ListView::Column::item_texts() const {
	size_t count = _pOwner->items.count();
	std::vector<std::wstring> texts;
	texts.reserve(count);
	for (UINT i = 0; i < count; ++i)
		texts.emplace_back(_pOwner->items[i].text(_index));
	return texts;
}

std::vector<std::wstring> ListView::Column::selected_item_texts() const {
	std::vector<std::wstring> texts;
	texts.reserve(_pOwner->items.selected_count());

	int idx = -1;
	for (;;) {
		idx = ListView_GetNextItem(_pOwner->hwnd(), idx, LVNI_SELECTED);
		if (idx == -1) break;
		texts.emplace_back(_pOwner->items[idx].text(_index));
	}
	return texts;
}

int ListView::Column::justif() const {
	HWND hHeader = ListView_GetHeader(_pOwner->hwnd());

	HDITEMW hdi{.mask = HDI_FORMAT};
	Header_GetItem(hHeader, _index, &hdi);

	return hdi.fmt & (HDF_LEFT | HDF_CENTER | HDF_RIGHT); // filter out
}

const ListView::Column& ListView::Column::set_justif(WORD hdf) const {
	HWND hHeader = ListView_GetHeader(_pOwner->hwnd());

	HDITEMW hdi{.mask = HDI_FORMAT};
	Header_GetItem(hHeader, _index, &hdi); // first, retrieve current

	hdi.fmt &= ~(HDF_CENTER | HDF_LEFT | HDF_RIGHT); // clear all three
	hdi.fmt |= (hdf & (HDF_LEFT | HDF_CENTER | HDF_RIGHT)); // sanitize
	Header_SetItem(hHeader, _index, &hdi);

	return *this;
}

WORD ListView::Column::sort_arrow() const {
	HWND hHeader = ListView_GetHeader(_pOwner->hwnd());

	HDITEMW hdi{.mask = HDI_FORMAT};
	Header_GetItem(hHeader, _index, &hdi);

	return hdi.fmt & (HDF_SORTDOWN | HDF_SORTUP); // filter out
}

const ListView::Column& ListView::Column::set_sort_arrow(WORD hdf) const {
	HWND hHeader = ListView_GetHeader(_pOwner->hwnd());
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

	ListView_GetColumn(_pOwner->hwnd(), _index, &lvc);
	str::trim_nulls(buf);
	return buf;
}

const ListView::Column& ListView::Column::set_text(WStrPtr text) const {
	LVCOLUMNW lvc{
		.mask = LVCF_TEXT,
		.pszText = text.lpwstr(),
	};
	ListView_SetColumn(_pOwner->hwnd(), _index, &lvc);
	return *this;
}

UINT ListView::Column::width() const {
	return ListView_GetColumnWidth(_pOwner->hwnd(), _index);
}

const ListView::Column& ListView::Column::set_width(UINT width) const {
	ListView_SetColumnWidth(_pOwner->hwnd(), _index, width);
	return *this;
}

const ListView::Column& ListView::Column::set_width_to_fill() const {
	size_t numCols = _pOwner->cols.count();
	if (numCols == 0) return *this;

	UINT cxUsed = 0;
	for (UINT i = 0; i < numCols; ++i) {
		if (i != _index)
			cxUsed += _pOwner->cols[i].width(); // retrieve cx of each column, but us
	}

	RECT rc{};
	GetClientRect(_pOwner->hwnd(), &rc); // list view client area
	return set_width(rc.right - cxUsed);
}

//------------------------------------------------------------------------------

ListView::Column ListView::ColumnCollection::add(WStrPtr text, UINT width) const {
	LVCOLUMNW lvc{
		.mask = LVCF_TEXT | LVCF_WIDTH,
		.cx = static_cast<int>(width),
		.pszText = text.lpwstr(),
	};
	int index = ListView_InsertColumn(_pOwner->hwnd(), 0xffff, &lvc); // insert as the last column
	return {*_pOwner, index}; // return newly added column
}

size_t ListView::ColumnCollection::count() const {
	HWND hHeader = ListView_GetHeader(_pOwner->hwnd());
	return Header_GetItemCount(hHeader);
}

//------------------------------------------------------------------------------

bool ListView::Item::is_focused() const {
	return ListView_GetItemState(_pOwner->hwnd(), _index, LVIS_FOCUSED) & LVIS_FOCUSED;
}

const ListView::Item& ListView::Item::focus() const {
	ListView_SetItemState(_pOwner->hwnd(), _index, LVIS_FOCUSED, LVIS_FOCUSED);
	return *this;
}

int ListView::Item::icon_index() const {
	LVITEMW lvi{
		.mask = LVIF_IMAGE,
		.iItem = _index,
	};
	ListView_GetItem(_pOwner->hwnd(), &lvi);
	return lvi.iImage;
}

const ListView::Item& ListView::Item::set_icon_index(int iconIndex) const {
	LVITEMW lvi{
		.mask = LVIF_IMAGE,
		.iItem = _index,
		.iImage = iconIndex,
	};
	ListView_SetItem(_pOwner->hwnd(), &lvi);
	return *this;
}

const ListView::Item& ListView::Item::remove() const {
	ListView_DeleteItem(_pOwner->hwnd(), _index);
	return *this;
}

bool ListView::Item::is_selected() const {
	return ListView_GetItemState(_pOwner->hwnd(), _index, LVIS_SELECTED) & LVIS_SELECTED;
}

const ListView::Item& ListView::Item::select(bool doSelect) const {
	ListView_SetItemState(_pOwner->hwnd(), _index, doSelect ? LVIS_SELECTED : 0, LVIS_SELECTED);
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
			SendMessageW(_pOwner->hwnd(), LVM_GETITEMTEXTW, _index, reinterpret_cast<LPARAM>(&lvi)));
		recvChars += 1; // plus terminating null count

		if (recvChars < curBufSz) { // to break, must have at least 1 char gap
			str::trim_nulls(buf);
			return buf;
		}

		curBufSz *= 2; // double the buffer size to try again
		buf.resize(curBufSz, L'\0');
	}
}

const ListView::Item& ListView::Item::set_text(WStrPtr text, UINT columnIndex) const {
	ListView_SetItemText(_pOwner->hwnd(), _index, columnIndex, text.lpwstr());
	return *this;
}

UINT ListView::Item::unique_id() const {
	return ListView_MapIndexToID(_pOwner->hwnd(), _index);
}

bool ListView::Item::is_visible() const {
	return ListView_IsItemVisible(_pOwner->hwnd(), _index);
}

LPARAM ListView::Item::raw_data() const {
	LVITEMW lvi{
		.mask = LVIF_PARAM,
		.iItem = _index,
	};
	ListView_GetItem(_pOwner->hwnd(), &lvi);
	return lvi.lParam;
}

const ListView::Item& ListView::Item::set_raw_data(LPARAM data) const {
	LVITEMW lvi{
		.mask = LVIF_PARAM,
		.iItem = _index,
		.lParam = data,
	};
	ListView_SetItem(_pOwner->hwnd(), &lvi);
	return *this;
}

//------------------------------------------------------------------------------

ListView::Item ListView::ItemCollection::add(WStrPtr text,
	std::initializer_list<WStrPtr> otherColumnsTexts, int iconIndex) const
{
	LVITEMW lvi{
		.mask = LVIF_TEXT | static_cast<UINT>(iconIndex != -1 ? LVIF_IMAGE : 0),
		.iItem = 0x0fff'ffff, // insert as the last item
		.pszText = text.lpwstr(),
		.iImage = iconIndex,
	};
	int index = ListView_InsertItem(_pOwner->hwnd(), &lvi);
	Item newItem{*_pOwner, index};

	for (auto colText = otherColumnsTexts.begin(); colText != otherColumnsTexts.end(); ++colText) {
		size_t idx = std::distance(otherColumnsTexts.begin(), colText);
		newItem.set_text(*colText, static_cast<UINT>(idx) + 1);
	}

	return newItem; // return newly added item
}

size_t ListView::ItemCollection::count() const {
	return ListView_GetItemCount(_pOwner->hwnd());
}

void ListView::ItemCollection::delete_all() const {
	ListView_DeleteAllItems(_pOwner->hwnd());
}

void ListView::ItemCollection::delete_selected() const {
	for (;;) {
		int idxFound = ListView_GetNextItem(_pOwner->hwnd(), -1, LVNI_SELECTED); // always search first one
		if (idxFound == -1) break;
		ListView_DeleteItem(_pOwner->hwnd(), idxFound);
	}
}

std::optional<ListView::Item> ListView::ItemCollection::focused() const {
	int idxFound = ListView_GetNextItem(_pOwner->hwnd(), -1, LVNI_FOCUSED);
	return idxFound == -1 ? std::nullopt : std::make_optional(Item{*_pOwner, idxFound});
}

std::optional<ListView::Item> ListView::ItemCollection::get_by_unique_id(UINT uid) const {
	int idx = ListView_MapIDToIndex(_pOwner->hwnd(), uid);
	return idx == -1 ? std::nullopt : std::make_optional(Item{*_pOwner, idx});
}

std::optional<ListView::Item> ListView::ItemCollection::hit_test(POINT pt) const {
	LVHITTESTINFO lvhti{.pt = pt};
	int idxFound = ListView_HitTestEx(_pOwner->hwnd(), &lvhti);
	return idxFound == -1 ? std::nullopt : std::make_optional(Item{*_pOwner, idxFound});
}

void ListView::ItemCollection::select_all(bool doSelect) const {
	if (GetWindowLongPtrW(_pOwner->hwnd(), GWL_STYLE) & LVS_SINGLESEL) [[unlikely]] {
		return; // single-sel list views cannot have all items selected
	}
	ListView_SetItemState(_pOwner->hwnd(), -1, doSelect ? LVIS_SELECTED : 0, LVIS_SELECTED);
}

std::vector<ListView::Item> ListView::ItemCollection::selected() const {
	std::vector<Item> items;
	items.reserve(selected_count());

	int idx = -1;
	for (;;) {
		idx = ListView_GetNextItem(_pOwner->hwnd(), idx, LVNI_SELECTED);
		if (idx == -1) break;
		items.emplace_back(*_pOwner, idx);
	}
	return items;
}

size_t ListView::ItemCollection::selected_count() const {
	return ListView_GetSelectedCount(_pOwner->hwnd());
}

void ListView::ItemCollection::sort(std::function<int(Item, Item)> cb) const {
	struct Info final {
		const ListView *pOwner;
		std::function<int(Item, Item)> cb;
	};
	Info nfo = {
		.pOwner = _pOwner,
		.cb = std::move(cb),
	};

	ListView_SortItemsEx(_pOwner->hwnd(), [](LPARAM idxA, LPARAM idxB, LPARAM lp) -> int { // receives indexes
		Info* pNfo = reinterpret_cast<Info*>(lp);
		return pNfo->cb(
			pNfo->pOwner->items[static_cast<int>(idxA)],
			pNfo->pOwner->items[static_cast<int>(idxB)]);
	}, &nfo);
}

std::optional<ListView::Item> ListView::ItemCollection::topmost_visible() const {
	int idx = ListView_GetTopIndex(_pOwner->hwnd());
	return idx == -1 ? std::nullopt : std::make_optional(Item{*_pOwner, idx});
}

//------------------------------------------------------------------------------

ListView::ListView(WindowParent &owner, WORD ctrlId)
	: _ctrl{owner}, _events{owner, valid_ctrl_id(ctrlId)}
{
	_ctrl._parentWndBase._preEvents.wm_create_or_init_dialog([this, pOwner = &owner]() -> void {
		_ctrl.create_wnd(ctrl_id(), _opts.windowExStyle, WC_LISTVIEWW, nullptr,
			_opts.windowStyle | _opts.ctrlStyle | LVS_SHAREIMAGELISTS, _opts.pos, _opts.size);
		set_extended_style(true, _opts.ctrlExStyle);
		_ctrl._parentWndBase._layout.add(hwnd(), _opts.layout);
	});

	custom_events();
}

ListView::ListView(WindowParent &owner, WORD ctrlId, Lay layout, WORD contextMenuId)
	: _ctrl{owner}, _events{owner, valid_ctrl_id(ctrlId)}
{
	_opts.contextMenuId = contextMenuId;

	_ctrl._parentWndBase._preEvents.wm_create_or_init_dialog([this, layout]() -> void {
		_ctrl.assign_dlg(ctrl_id());
		SetWindowLongPtrW(hwnd(), GWL_STYLE, GetWindowLongPtrW(hwnd(), GWL_STYLE) | LVS_SHAREIMAGELISTS);
		_ctrl._parentWndBase._layout.add(hwnd(), layout);
	});

	custom_events();
}

const ListView& ListView::set_extended_style(bool doSet, DWORD exStyle) const {
	ListView_SetExtendedListViewStyleEx(hwnd(), exStyle, doSet ? exStyle : 0);
	return *this;
}

ImageList& ListView::image_list_16() {
	if (!_hImg16.himagelist()) { // not created yet?
		_hImg16.create(16, 16, ILC_COLOR32);
		ListView_SetImageList(hwnd(), _hImg16.himagelist(), LVSIL_SMALL);
	}
	return _hImg16;
}

ImageList& ListView::image_list_32() {
	if (!_hImg32.himagelist()) { // not created yet?
		_hImg32.create(32, 32, ILC_COLOR32);
		ListView_SetImageList(hwnd(), _hImg32.himagelist(), LVSIL_NORMAL);
	}
	return _hImg32;
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

	_ctrl._parentWndBase._preEvents.wm_notify(ctrl_id(), LVN_KEYDOWN, [this](wm::Notify p) -> void {
		NMLVKEYDOWN &nmk = p.hdr<NMLVKEYDOWN>();
		bool hasCtrl = GetAsyncKeyState(VK_CONTROL) & 0x8000;

		if (hasCtrl && nmk.wVKey == 'A') { // Ctrl+A pressed?
			items.select_all(true);
		} else if (nmk.wVKey == VK_APPS) { // context menu key?
			bool hasShift = GetAsyncKeyState(VK_SHIFT) & 0x8000;
			show_context_menu(false, hasCtrl, hasShift);
		}
	});

	_ctrl._parentWndBase._preEvents.wm_notify(ctrl_id(), NM_RCLICK, [this](wm::Notify p) -> void {
		NMITEMACTIVATE &nmi = p.hdr<NMITEMACTIVATE>();
		bool hasCtrl = nmi.uKeyFlags & LVKF_CONTROL;
		bool hasShift = nmi.uKeyFlags & LVKF_SHIFT;

		show_context_menu(true, hasCtrl, hasShift);
	});

	_ctrl._parentWndBase._postEvents.wm(WM_DESTROY, [this](wm::Msg) -> void {
		if (_opts.hMenuContext)
			DestroyMenu(_opts.hMenuContext);
	});
}

void ListView::show_context_menu(bool followCursor, bool hasCtrl, bool hasShift) {
	if (!_opts.hMenuContext) {
		if (_opts.contextMenuId) {
			_opts.hMenuContext = LoadMenuW(GetModuleHandleW(nullptr), MAKEINTRESOURCEW(_opts.contextMenuId));
			#ifdef _DEBUG
			if (!_opts.hMenuContext)
				throw std::invalid_argument("LoadMenu failed to load ListView context menu resource.");
			#endif
		} else {
			return; // no context menu defined
		}
	}

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
	HMENU hSubMenu = GetSubMenu(_opts.hMenuContext, 0); // pop the first submenu
	#ifdef _DEBUG
	if (!hSubMenu)
		throw std::invalid_argument("GetSubMenu failed to load ListView context submenu.");
	#endif
	ClientToScreen(hwnd(), &menuPos); // from listview to screen
	SetForegroundWindow(hParent);
	BOOL retTrk = TrackPopupMenu(hSubMenu, TPM_LEFTBUTTON, menuPos.x, menuPos.y, 0, hParent, nullptr);
	#ifdef _DEBUG
	if (!retTrk)
		throw std::runtime_error("TrackPopupMenu failed to load ListView context submenu.");
	#endif
	PostMessageW(hParent, WM_NULL, 0, 0); // necessary according to TrackPopupMenu docs
}

////////////////////////////////////////////////////////////////////////////////

Static::Static(WindowParent &owner, WORD ctrlId)
	: _ctrl{owner}, _events{owner, valid_ctrl_id(ctrlId)}
{
	_ctrl._parentWndBase._preEvents.wm_create_or_init_dialog([this, pOwner = &owner]() -> void {
		if (!_opts.size.cx && !_opts.size.cy)
			_opts.size = calc_text_bound_box(str::remove_accel_ampersands(_opts.text));

		_ctrl.create_wnd(ctrl_id(), _opts.windowExStyle, L"STATIC", _opts.text,
			_opts.windowStyle | _opts.ctrlStyle, _opts.pos, _opts.size);
		apply_ui_font(hwnd());
		_ctrl._parentWndBase._layout.add(hwnd(), _opts.layout);
	});
}

Static::Static(WindowParent &owner, WORD ctrlId, Lay layout)
	: _ctrl{owner}, _events{owner, valid_ctrl_id(ctrlId)}
{
	_ctrl._parentWndBase._preEvents.wm_create_or_init_dialog([this, layout]() -> void {
		_ctrl.assign_dlg(ctrl_id());
		_ctrl._parentWndBase._layout.add(hwnd(), layout);
	});
}

const Static& Static::set_text(WStrPtr text) const {
	set_wnd_text(hwnd(), text);
	return *this;
}

const Static& Static::set_text_resize(WStrPtr text) const {
	set_text(text);
	SIZE bounds = calc_text_bound_box(str::remove_accel_ampersands(text));
	SetWindowPos(hwnd(), nullptr, 0, 0, bounds.cx, bounds.cy, SWP_NOZORDER | SWP_NOMOVE);
	return *this;
}
