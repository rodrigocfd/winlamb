
#pragma once
#include "base_raw.h"

namespace wl {

// A main application window created programmatically.
class window_main : public _wli::base_raw {
public:
	// Window creation options.
	struct opts {
		UINT class_style = 0;
		HICON hicon = nullptr;
		HCURSOR hcursor = nullptr;
		HBRUSH hbrush_bg = nullptr;
	};

private:
	opts _opts;

public:
	virtual ~window_main() = 0;
	explicit window_main(opts creation_opts);
};

}
