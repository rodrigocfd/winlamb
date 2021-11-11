
#include "window_dlg_modal.h"
using namespace wl;

window_dlg_modal::window_dlg_modal(int dialog_id)
	: window_dlg{dialog_id}
{
	on().close([this]() {
		EndDialog(hwnd(), IDCANCEL);
	});
}

void window_dlg_modal::show_modal(HWND hparent) const
{
	_dialog_box(reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(hparent, GWLP_HINSTANCE)), hparent);
}
