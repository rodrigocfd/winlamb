#pragma once
#include "lib-include-win.h"
#include "str.h"
#include "wnd-interfaces.h"
#include "wnd-funcs.h"
#include "wnd-opts.h"
#include "events-ctl.h"
#include "image-list.h"

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
		[[nodiscard]] constexpr opts::ButtonOpts& setup() { return _opts; }

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
		[[nodiscard]] events::ButtonEvents& on() { return _wl_internal::valid_event(hwnd(), _events); }

		/// [Subclasses] the control allowing message events to be added.
		///
		/// The events must be added before the control is created on the screen.
		///
		/// Note that subclassing is a potentially slow technique, prefer using ordinary events.
		///
		/// [Subclasses]: https://learn.microsoft.com/en-us/windows/win32/controls/subclassing-overview
		[[nodiscard]] events::WindowEvents& subclass_on() { return _wl_internal::valid_event(hwnd(), _ctrl._subclassEvents); }

		/// Calls [`GetWindowText`] to return the button text.
		///
		/// [`GetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowtextw
		[[nodiscard]] std::wstring text() const { return _wl_internal::wnd_text(hwnd()); }

		/// Calls [`SetWindowText`] to set the button text.
		///
		/// [`SetWindowText`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowtextw
		const Button& set_text(WStrPtr text) const;

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
		[[nodiscard]] constexpr opts::CheckBoxOpts& setup() { return _opts; }

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
		[[nodiscard]] events::ButtonEvents& on() { return _wl_internal::valid_event(hwnd(), _events); }

		/// [Subclasses] the control allowing message events to be added.
		///
		/// The events must be added before the control is created on the screen.
		///
		/// Note that subclassing is a potentially slow technique, prefer using ordinary events.
		///
		/// [Subclasses]: https://learn.microsoft.com/en-us/windows/win32/controls/subclassing-overview
		[[nodiscard]] events::WindowEvents& subclass_on() { return _wl_internal::valid_event(hwnd(), _ctrl._subclassEvents); }

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

	private:
		_wl_internal::NativeCtrlBase _ctrl;
		events::ButtonEvents _events;
		opts::CheckBoxOpts _opts{};
	};

	/// @brief Native [list view] control.
	///
	/// Example of creating a window with a `ListView` programmatically, .h and .cpp files:
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
	/// Example of creating a window with a `ListView` from a dialog resource, .h and .cpp files:
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
		/** @brief A single column of the ListView. */
		class Column final {
		public:
			constexpr Column(const ListView &owner, int index) : _pOwner{&owner}, _index{index} { }

			/** Returns the index of the column. */
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
			const Column& set_text(WStrPtr text) const;

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
			Column add(WStrPtr text, UINT width) const;

			/** Returns the column count. */
			[[nodiscard]] UINT count() const;

		private:
			const ListView *_pOwner;
			friend ListView;
		};

		/** @brief A single item of the ListView. */
		class Item final {
		public:
			constexpr Item(const ListView &owner, int index) : _pOwner{&owner}, _index{index} { }

			[[nodiscard]] constexpr int index() const { return _index; }
			[[nodiscard]] LPARAM data() const;
			const Item& set_data(LPARAM data) const;
			[[nodiscard]] bool is_focused() const;
			const Item& focus() const;
			[[nodiscard]] int icon_index() const;
			const Item& set_icon_index(int iconIndex) const;
			const Item& remove() const;
			[[nodiscard]] bool is_selected() const;
			const Item& select(bool doSelect) const;
			[[nodiscard]] std::wstring text(UINT columnIndex = 0) const;
			const Item& set_text(WStrPtr text, UINT columnIndex = 0) const;
			[[nodiscard]] UINT unique_id() const;
			[[nodiscard]] bool is_visible() const;

		private:
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
			Item add(WStrPtr text, std::initializer_list<WStrPtr> otherColumnsTexts = {}, int iconIndex = -1) const;

			/** Returns the item count. */
			[[nodiscard]] UINT count() const;

			/** Deletes all items. */
			void delete_all() const;

			/** Deletes the selected items. */
			void delete_selected() const;

			/** Returns the focused item, if any. */
			[[nodiscard]] std::optional<Item> focused() const;

			/// Calls [`ListView_MapIDToIndex`] to return the item with the given unique ID.
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
			[[nodiscard]] UINT selected_count() const;

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
		[[nodiscard]] constexpr opts::ListViewOpts& setup() { return _opts; }

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
		[[nodiscard]] events::ListViewEvents& on() { return _wl_internal::valid_event(hwnd(), _events); }

		/// [Subclasses] the control allowing message events to be added.
		///
		/// The events must be added before the control is created on the screen.
		///
		/// Note that subclassing is a potentially slow technique, prefer using ordinary events.
		///
		/// [Subclasses]: https://learn.microsoft.com/en-us/windows/win32/controls/subclassing-overview
		[[nodiscard]] events::WindowEvents& subclass_on() { return _wl_internal::valid_event(hwnd(), _ctrl._subclassEvents); }

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

		/** Retrieves the associated 16x16 `ImageList`. It will be automatically created if not yet. */
		ImageList& image_list_16();

		/** Retrieves the associated 32x32 `ImageList`. It will be automatically created if not yet. */
		ImageList& image_list_32();

	private:
		void custom_events();
		void show_context_menu(bool followCursor, bool hasCtrl, bool hasShift);

		_wl_internal::NativeCtrlBase _ctrl;
		events::ListViewEvents _events;
		opts::ListViewOpts _opts{};
		ImageList _hImg16{}, _hImg32{};
	};

}
