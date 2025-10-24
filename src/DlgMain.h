#pragma once
#include "winlamb/lib.h"

#define ICO_MAIN        101
#define MNU_FILES       200
#define MNU_FILES_ABOUT 207
#define DLG_MAIN        1000
#define LST_FILES       1001

class DlgMain final {
public:
	DEL_COPY_MOVE(DlgMain);
	DlgMain();

	wl::WindowMain wnd{DLG_MAIN, ICO_MAIN};
	wl::DropFiles dropFiles{wnd};
	wl::ListView lstFiles{wnd, LST_FILES, MNU_FILES, wl::Lay::resize_resize};

private:
	bool on_init_dialog(wl::wm::InitDialog);
	void on_cancel();
	void on_drop_files(const std::vector<std::wstring> files);
	void on_lst_files_item_changed(NMLISTVIEW &p);
	void on_about();
};

////////////////////////////////////////////////////////////////////////////////

class RawMain final {
public:
	DEL_COPY_MOVE(RawMain);
	RawMain();

	wl::WindowMain wnd;
	wl::ListView lst;

private:
	int on_create(wl::wm::Create);
};
