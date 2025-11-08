#pragma once
#include "winlamb/lib.h"

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
	wl::ListView lv{wnd};
	Contro ctl{wnd};
};
