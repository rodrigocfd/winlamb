
#pragma once
#include "base_dlg.h"

namespace wl {

// A modal window which loads a dialog template resource.
class dialog_modal : public _wli::base_dlg {
public:
	// Dialog creation options.
	// Aggregate initialization: fields are in alphabetical order.
	struct opts final {
		int dialog_id = 0;
	};

private:
	opts _opts;

public:
	virtual ~dialog_modal() = 0;
	explicit dialog_modal(opts creation_opts);

	// Shows the modal window.
	virtual void show_modal(HWND hparent) const;
};

}
