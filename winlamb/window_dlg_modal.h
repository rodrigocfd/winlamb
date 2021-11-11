
#pragma once
#include "window_dlg.h"

namespace wl {

// A modal window which loads a dialog template resource.
class window_dlg_modal : public _wli::window_dlg {
public:
	virtual ~window_dlg_modal() { }
	explicit window_dlg_modal(int dialog_id);

	// Shows the modal window.
	virtual void show_modal(HWND hparent) const;
};

}
