#pragma once
#include "lib-include-win.h"
#include <CommCtrl.h>
#include "window-user.h"

namespace _wl_internal {

	// Native list view control events.
	class EventsListView final {
	public:
		EventsListView() = delete;
		EventsListView(const EventsListView&) = delete;
		EventsListView(EventsListView&&) = delete;
		EventsListView& operator=(const EventsListView&) = delete;
		EventsListView& operator=(EventsListView&&) = delete;

		EventsListView(wl::WindowParent &owner, WORD ctrlId) : _events{owner, ctrlId} { }

		void lvn_begin_label_edit(std::function<bool(NMLVDISPINFOW&)> cb);
		void lvn_column_click(std::function<void(NMLISTVIEW&)> cb);
		void lvn_delete_all_items(std::function<bool(NMLISTVIEW&)> cb);
		void lvn_delete_item(std::function<void(NMLISTVIEW&)> cb);
		void lvn_end_label_edit(std::function<bool(NMLVDISPINFOW&)> cb);
		void lvn_insert_item(std::function<void(NMLISTVIEW&)> cb);
		void lvn_item_activate(std::function<void(NMITEMACTIVATE&)> cb);
		void lvn_item_changed(std::function<void(NMLISTVIEW&)> cb);
		void lvn_item_changing(std::function<bool(NMLISTVIEW&)> cb);
		void lvn_key_down(std::function<void(NMLVKEYDOWN&)> cb);
		void nm_click(std::function<void(NMITEMACTIVATE&)> cb);
		void nm_custom_draw(std::function<DWORD(NMLVCUSTOMDRAW&)> cb);
		void nm_dbl_clk(std::function<void(NMITEMACTIVATE&)> cb);
		void nm_kill_focus(std::function<void(NMHDR&)> cb);
		void nm_r_click(std::function<void(NMITEMACTIVATE&)> cb);
		void nm_r_dbl_clk(std::function<void(NMITEMACTIVATE&)> cb);
		void nm_set_focus(std::function<void(NMHDR&)> cb);

	private:
		EventsNativeCtrl _events;
	};

}

namespace wl {

	// Native list view control.
	class ListView final {
	public:
		// A single column of the ListView.
		class Column final {
		public:
			Column() = delete;
			constexpr Column(const Column&) = default;
			constexpr Column(Column&&) = default;
			constexpr Column& operator=(const Column&) = default;
			constexpr Column& operator=(Column&&) = default;

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
			const Column& set_width_to_fill(UINT width) const;

		private:
			const ListView *_pOwner;
			int _index;
		};

	private:
		// Operations over the columns.
		class ColumnCollection final {
		private:
			ColumnCollection() = delete;
			ColumnCollection(const ColumnCollection&) = delete;
			ColumnCollection(ColumnCollection&&) = delete;
			ColumnCollection& operator=(const ColumnCollection&) = delete;
			ColumnCollection& operator=(ColumnCollection&&) = delete;

			constexpr explicit ColumnCollection(const ListView *pOwner) : _pOwner{pOwner} { }

		public:
			[[nodiscard]] constexpr Column operator[](int index) const { return Column{*_pOwner, index}; }
			Column add(WStrPtr text, UINT width) const;
			[[nodiscard]] UINT count() const;

		private:
			const ListView *_pOwner;
			friend ListView;
		};

	public:
		// A single item of the ListView.
		class Item final {
		public:
			Item() = delete;
			constexpr Item(const Item&) = default;
			constexpr Item(Item&&) = default;
			constexpr Item& operator=(const Item&) = default;
			constexpr Item& operator=(Item&&) = default;

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

	private:
		// Operations over the items.
		class ItemCollection final {
		private:
			ItemCollection() = delete;
			ItemCollection(const ItemCollection&) = delete;
			ItemCollection(ItemCollection&&) = delete;
			ItemCollection& operator=(const ItemCollection&) = delete;
			ItemCollection& operator=(ItemCollection&&) = delete;

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

	public:
		ListView() = delete;
		ListView(const ListView&) = delete;
		ListView(ListView&&) = delete;
		ListView& operator=(const ListView&) = delete;
		ListView& operator=(ListView&&) = delete;

		ListView(WindowParent &owner, WORD ctrlId, WORD contextMenuId = 0);

		ColumnCollection cols{this};
		ItemCollection items{this};

		[[nodiscard]] constexpr HWND hwnd() const { return _ctrl.hwnd(); }
		[[nodiscard]] _wl_internal::EventsListView& on() { return _events; }

	private:
		void show_context_menu(bool followCursor, bool hasCtrl, bool hasShift);

		_wl_internal::NativeCtrl _ctrl;
		_wl_internal::EventsListView _events;
		HMENU _hMenuContext = nullptr;
	};

}
