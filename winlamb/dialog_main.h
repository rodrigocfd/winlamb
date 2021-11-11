
#pragma once
#include "base_dlg.h"
#include "run.h"

namespace wl {

// A main application window which loads a dialog template resource.
class dialog_main : public _wli::base_dlg {
public:
	// Creation options.
	struct opts final {
		int dialog_id = 0;
		int icon_id = 0;
		int accel_table_id = 0;
	};

private:
	opts _opts;

public:
	virtual ~dialog_main() = 0;
	explicit dialog_main(opts creation_opts);

	// Runs the window as the main application window.
	// Prefer using the RUN() macro, which also declares an exception-safe WinMain() entry-point.
	virtual int run_main(HINSTANCE hinst, int cmd_show = SW_SHOW) const;
};

}
