
#pragma once
#include "window_dlg.h"
#include "run.h"

namespace wl {

// A main application window which loads a dialog template resource.
class window_dlg_main : public _wli::window_dlg {
private:
	int _icon_id;
	int _accel_table_id;

public:
	virtual ~window_dlg_main() { }
	explicit window_dlg_main(int dialog_id, int icon_id = 0, int accel_table_id = 0);

	// Runs the window as the main application window.
	// Prefer using the RUN() macro, which also declares an exception-safe WinMain() entry-point.
	virtual int run_main(HINSTANCE hinst, int cmd_show = SW_SHOW) const;
};

}
