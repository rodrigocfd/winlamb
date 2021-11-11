
#pragma once
#include "base_parent.h"

namespace wl { class dialog_main; } // friend forward declaration
namespace wl { class dialog_modal; }

namespace _wli {

// A dialog-based window.
class base_dlg : public base_parent {
private:
	friend wl::dialog_main;
	friend wl::dialog_modal;
	int _dialog_id;

public:
	virtual ~base_dlg() = 0;
	explicit base_dlg(int dialog_id)
		: base_parent{base_parent::creation_type::dialog}, _dialog_id{dialog_id} { }

private:
	void _create_dialog(HINSTANCE hinst, HWND hparent) const;
	void _dialog_box(HINSTANCE hinst, HWND hparent) const;

	static INT_PTR _dlg_proc(HWND hdlg, UINT msg, WPARAM wp, LPARAM lp);
};

}
