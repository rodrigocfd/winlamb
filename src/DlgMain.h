#pragma once
#include "winlamb/winlamb.hpp"

#define ICO_MAIN        101
#define MNU_FILES       200
#define MNU_FILES_ABOUT 207
#define DLG_MAIN        1000
#define LST_FILES       1001

class DlgMain final {
public:
	DlgMain();

	wl::WindowMain wnd{DLG_MAIN, ICO_MAIN};
	wl::DropFiles dropFiles{wnd};
	wl::ListView lstFiles{wnd, LST_FILES, wl::Lay::resize_resize, MNU_FILES};

private:
	bool on_init_dialog(wl::wm::InitDialog);
	void on_cancel();
	void on_drop_files(const std::vector<std::wstring> files);
	void on_lst_files_item_changed(NMLISTVIEW &p);
	void on_about();
};

////////////////////////////////////////////////////////////////////////////////

class Contro final {
public:
	Contro(wl::WindowParent &parent, POINT pos, SIZE size);
	wl::WindowControl wnd;
private:
	void on_paint();
};

class RawMain final {
public:
	RawMain();
	wl::WindowMain wnd{wl::MainOpts{
		.iconId = ICO_MAIN,
		.size = wl::dpi::sz(900, 320),
		.style = wl::MainOpts{}.style | WS_SIZEBOX | WS_MAXIMIZEBOX,
		.title = L"My main window",
	}};
	wl::Button btn{wnd, wl::ButtonOpts{
		.pos = wl::dpi::pt(10, 10),
		.text = L"&Click me",
	}};
	wl::CheckBox chk{wnd, wl::CheckBoxOpts{
		.pos = wl::dpi::pt(110, 10),
		.text = L"C&heck me",
	}};
	wl::ComboBox cmb{wnd, wl::ComboBoxOpts{
		.pos = wl::dpi::pt(200, 10),
		.texts = {L"Hello", L"World"},
	}};
	wl::DateTimePicker dtp{wnd, wl::DateTimePickerOpts{
		.pos = wl::dpi::pt(10, 48),
	}};
	wl::Edit txt{wnd, wl::EditOpts{
		.pos = wl::dpi::pt(250, 48),
	}};
	wl::ListView lv{wnd, wl::ListViewOpts{
		.cols = {
			{L"First", wl::dpi::x(200)},
			{L"Second", 1},
		},
		.contextMenuId = MNU_FILES,
		.layout = wl::Lay::resize_resize,
		.pos = wl::dpi::pt(10, 80),
		.size = wl::dpi::sz(400, 100),
	}};
	wl::MonthCalendar mcal{wnd, wl::MonthCalendarOpts{
		.layout = wl::Lay::move_hold,
		.pos = wl::dpi::pt(420, 100),
	}};
	wl::Static lbl{wnd, wl::StaticOpts{
		.pos = wl::dpi::pt(320, 10),
		.text = L"Label",
	}};
	wl::StatusBar sb{wnd, wl::StatusBarOpts{
		.parts = {
			wl::SbPart{.flex = 1, .text = L"Here"},
			wl::SbPart{.iconIndex = 0, .text = L"Hello", .width = wl::dpi::x(200)},
		},
	}};
	wl::RadioGroup rads{wnd, {
		wl::RadioButtonOpts{
			.pos = wl::dpi::pt(420, 10),
			.text = L"First",
		},
		wl::RadioButtonOpts{
			.pos = wl::dpi::pt(420, 25),
			.text = L"Second",
		},
		wl::RadioButtonOpts{
			.pos = wl::dpi::pt(420, 40),
			.selected = true,
			.text = L"Turd",
		},
	}};
	wl::TreeView tv{wnd, wl::TreeViewOpts{
		.layout = wl::Lay::hold_move,
		.pos = wl::dpi::pt(10, 190),
		.size = wl::dpi::sz(250, 90),
	}};
	wl::Trackbar trb{wnd, wl::TrackbarOpts{
		.layout = wl::Lay::hold_move,
		.pos = wl::dpi::pt(260, 190),
		.range = {0, 8},
		.size = wl::dpi::sz(140, 28),
	}};

	wl::Tab tab{wnd, wl::TabOpts{
		.layout = wl::Lay::move_hold,
		.pos = wl::dpi::pt(656, 10),
		.selected = 1,
		.size = wl::dpi::sz(220, 200),
		.titles = {L"First", L"Second"},
	}};
	wl::Button btn2{tab.items[0].child(), wl::ButtonOpts{
		.text = L"In tab",
	}};
	wl::Button btn3{tab.items[1].child(), wl::ButtonOpts{
		.text = L"Another in tab",
	}};

	Contro ctl{wnd, wl::dpi::pt(550, 10), wl::dpi::sz(80, 80)};
};
