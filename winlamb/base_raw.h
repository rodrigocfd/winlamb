
#pragma once
#include "base_parent.h"

namespace wl { class window_main; } // friend forward declaration
namespace wl { class window_modal; }

namespace _wli {

// A window created programmatically.
class base_raw : public base_parent {
private:
	friend wl::window_main;
	friend wl::window_modal;

public:
	virtual ~base_raw() = 0;
	base_raw()
		: base_parent{base_parent::creation_type::raw} { }

private:

};

}
