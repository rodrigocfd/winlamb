
#pragma once
#include "base_raw.h"

namespace wl {

// A main application window created programmatically.
class window_main : public _wli::base_raw {
public:
	// Window creation options.
	// Aggregate initialization: fields are in alphabetical order.
	struct opts {
		UINT class_style = 0;
		HBRUSH hbrush_bg = nullptr;
		HCURSOR hcursor = nullptr;
		int icon_id = 0;
	};

private:
	opts _opts;

public:
	virtual ~window_main() = 0;
	explicit window_main(opts creation_opts);
};

}
