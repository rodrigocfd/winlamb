
#pragma once
#include "window_base.h"

namespace wl { class window_dlg_main; } // friend forward declaration
namespace wl { class window_dlg_modal; }

namespace _wli {

// A dialog-based window.
class window_dlg : public window_base {
private:
	friend wl::window_dlg_main;
	friend wl::window_dlg_modal;
	int _dialog_id;

public:
	virtual ~window_dlg() { }
	explicit window_dlg(int dialog_id)
		: window_base{window_base::creation_type::dialog}, _dialog_id{dialog_id} { }

private:
	void _create_dialog(HINSTANCE hinst, HWND hparent) const;
	void _dialog_box(HINSTANCE hinst, HWND hparent) const;

	static INT_PTR _dlg_proc(HWND hdlg, UINT msg, WPARAM wp, LPARAM lp);
};

}
