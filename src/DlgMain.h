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
	Contro(wl::WindowParent &parent);
	wl::WindowControl wnd;
private:
	void on_paint();
};

class RawMain final {
public:
	RawMain();
	wl::WindowMain wnd{};
	wl::Button btn{wnd};
	wl::CheckBox chk{wnd};
	wl::ComboBox cmb{wnd};
	wl::DateTimePicker dtp{wnd};
	wl::Edit txt{wnd};
	wl::ListView lv{wnd};
	wl::MonthCalendar mcal{wnd};
	wl::Static lbl{wnd};
	wl::StatusBar sb{wnd};
	wl::RadioGroup rads{wnd, 3};
	wl::TreeView tv{wnd};
	wl::Trackbar trb{wnd};

	wl::Tab tab{wnd, 2};
	wl::Button btn2{tab.items[0].child()};
	wl::Button btn3{tab.items[1].child()};

	Contro ctl{wnd};
};
