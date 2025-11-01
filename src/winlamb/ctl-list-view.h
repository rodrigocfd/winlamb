#pragma once
#include "lib-include-win.h"
#include <CommCtrl.h>
#include "window-user.h"

namespace wl { class ListView; }

namespace wl::events {

	/** @brief Native list view message callbacks. */
	class ListViewEvents final : wl::NonMovable {
	private:
		ListViewEvents(wl::WindowParent &owner, WORD ctrlId) : _events{owner, ctrlId} { }

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
		_wl_internal::EventsNativeCtrl _events;
		friend wl::ListView; // ctor
	};

}

namespace wl::opts {

	/** Options to create a ListView programmatically. */
	struct ListView final {
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::opts::ListView lvOpts{
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
		/// wl::opts::ListView lvOpts{
		///     .pos = wl::dpi::sz(120, 120),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		SIZE size = {.cx = 120, .cy = 120};
		/// The [window style] passed to [`CreateWindowEx`].
		///
		/// [window style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD windowStyle = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE;
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD windowExStyle = WS_EX_LEFT | WS_EX_CLIENTEDGE;
		/// The [ListView style] passed to [`CreateWindowEx`].
		///
		/// [ListView style]: https://learn.microsoft.com/en-us/windows/win32/controls/list-view-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		DWORD ctrlStyle = LVS_REPORT | LVS_NOSORTHEADER | LVS_SHOWSELALWAYS;
		/// The [ListView extended styles] applied right after the control is created.
		///
		/// [ListView extended styles]: https://learn.microsoft.com/en-us/windows/win32/controls/extended-list-view-styles
		DWORD ctrlExStyle = LVS_EX_FULLROWSELECT;
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		WORD ctrlId = 0;
		/** Optional ListView context menu. */
		HMENU hMenuContext = nullptr;
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		Lay layout = Lay::none_none;
	};

}

namespace wl {

	/// @brief Native [list view] control.
	///
	/// [list view]: https://learn.microsoft.com/en-us/windows/win32/controls/list-view-controls-overview
	class ListView final : NonMovable {
	public:
		/** A single column of the ListView. */
		class Column final {
		public:
			constexpr Column(const ListView &owner, int index) : _pOwner{&owner}, _index{index} { }

			[[nodiscard]] constexpr int index() const { return _index; }

			// Returns the texts of all items under this column.
			[[nodiscard]] std::vector<std::wstring> item_texts() const;

			// Returns the texts of the selected items under this column.
			[[nodiscard]] std::vector<std::wstring> selected_item_texts() const;

			// Returns HDF_CENTER, HDF_LEFT or HDF_RIGHT justification.
			[[nodiscard]] int justif() const;

			// Sets HDF_CENTER, HDF_LEFT or HDF_RIGHT justification.
			const Column& set_justif(WORD hdf) const;

			// Returns HDF_SORTUP (ascending) or HDF_SORTDOWN (descending).
			[[nodiscard]] WORD sort_arrow() const;

			// Sets HDF_SORTUP (ascending) or HDF_SORTDOWN (descending).
			const Column& set_sort_arrow(WORD hdf) const;

			[[nodiscard]] std::wstring text() const;
			const Column& set_text(WStrPtr text) const;
			[[nodiscard]] UINT width() const;
			const Column& set_width(UINT width) const;
			const Column& set_width_to_fill() const;

		private:
			const ListView *_pOwner;
			int _index;
		};

		/** Operations over the columns. */
		class ColumnCollection final : NonMovable {
		private:
			constexpr explicit ColumnCollection(const ListView *pOwner) : _pOwner{pOwner} { }

		public:
			[[nodiscard]] constexpr Column operator[](int index) const { return Column{*_pOwner, index}; }
			Column add(WStrPtr text, UINT width) const;
			[[nodiscard]] UINT count() const;

		private:
			const ListView *_pOwner;
			friend ListView;
		};

		/** A single item of the ListView. */
		class Item final {
		public:
			constexpr Item(const ListView &owner, int index) : _pOwner{&owner}, _index{index} { }

			[[nodiscard]] constexpr int index() const { return _index; }
			[[nodiscard]] LPARAM data() const;
			const Item& set_data(LPARAM data) const;
			[[nodiscard]] bool is_focused() const;
			const Item& focus() const;
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

		/** Operations over the items. */
		class ItemCollection final : NonMovable {
		private:
			constexpr explicit ItemCollection(const ListView *pOwner) : _pOwner{pOwner} { }

		public:
			[[nodiscard]] constexpr Item operator[](int index) const { return Item{*_pOwner, index}; }
			Item add(WStrPtr text, std::initializer_list<WStrPtr> otherColumnsTexts = {}, int icon = -1) const;
			[[nodiscard]] UINT count() const;
			void delete_all() const;
			void delete_selected() const;
			[[nodiscard]] std::optional<Item> focused() const;
			[[nodiscard]] std::optional<Item> get_by_unique_id(UINT uid) const;
			[[nodiscard]] std::optional<Item> hit_test(POINT pt) const;
			void select_all(bool doSelect) const;
			[[nodiscard]] std::vector<Item> selected() const;
			[[nodiscard]] UINT selected_count() const;
			void sort(std::function<int(Item, Item)> cb) const;
			[[nodiscard]] std::optional<Item> topmost_visible() const;

		private:
			const ListView *_pOwner;
			friend ListView;
		};

		/// Constructs the list view programmatically with [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		ListView(WindowParent &owner, opts::ListView options);

		/** Constructs the list view from the dialog resource. */
		ListView(WindowParent &owner, WORD ctrlId, WORD contextMenuId = 0, Lay layout = Lay::none_none);

		/** Column methods. */
		ColumnCollection cols{this};

		/** Item methods. */
		ItemCollection items{this};

		/** Returns the wrapped window handle. */
		[[nodiscard]] constexpr HWND hwnd() const { return _ctrl.hwnd(); }

		/** Returns the control ID. */
		[[nodiscard]] constexpr WORD ctrl_id() const { return _events._events._ctrlId; }

		/// Allows message events to be added.
		///
		/// The events must be added before the control is created on the screen.
		[[nodiscard]] constexpr events::ListViewEvents& on() { return _events; }

		/// Sets one or more [extended styles].
		///
		/// Example:
		///
		/// ```cpp
		/// myList.set_extended_style(true, LVS_EX_FULLROWSELECT);
		/// ```
		///
		/// [extended styles]: https://learn.microsoft.com/en-us/windows/win32/controls/extended-list-view-styles
		const ListView& set_extended_style(bool doSet, DWORD exStyle) const;

	private:
		void custom_events();
		void show_context_menu(bool followCursor, bool hasCtrl, bool hasShift);

		_wl_internal::NativeCtrl _ctrl;
		events::ListViewEvents _events;
		HMENU _hMenuContext = nullptr;
	};

}
