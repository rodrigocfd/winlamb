
#include "dialog_modal.h"
using namespace wl;

dialog_modal::~dialog_modal() { }

dialog_modal::dialog_modal(opts creation_opts)
	: base_dlg{creation_opts.dialog_id}, _opts{creation_opts}
{
	on().wm_close([this]() {
		EndDialog(hwnd(), IDCANCEL);
	});
}

void dialog_modal::show_modal(HWND hparent) const
{
	_dialog_box(reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(hparent, GWLP_HINSTANCE)), hparent);
}
