
#include <system_error>
#include "dialog_main.h"
#include "run_funcs.h"
using namespace _wli;
using namespace wl;

dialog_main::~dialog_main() { }

dialog_main::dialog_main(opts creation_opts)
	: base_dlg{creation_opts.dialog_id}, _opts{creation_opts}
{
	on().close([this]() {
		DestroyWindow(hwnd());
	});

	on().nc_destroy([]() {
		PostQuitMessage(ERROR_SUCCESS);
	});
}

int dialog_main::run_main(HINSTANCE hinst, int cmd_show) const
{
	first_calls();
	_create_dialog(hinst, nullptr);

	HACCEL haccel = nullptr;
	if (_opts.accel_table_id != 0) {
		haccel = LoadAcceleratorsW(hinst, MAKEINTRESOURCEW(_opts.accel_table_id));
		if (haccel == nullptr) {
			throw std::system_error(GetLastError(), std::system_category(),
				"LoadAccelerators failed for main dialog");
		}
	}

	if (_opts.icon_id != 0) {
		SendMessageW(hwnd(), WM_SETICON, ICON_SMALL,
			reinterpret_cast<LPARAM>(reinterpret_cast<HICON>(
				LoadImageW(hinst, MAKEINTRESOURCEW(_opts.icon_id), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR))));
		SendMessageW(hwnd(), WM_SETICON, ICON_BIG,
			reinterpret_cast<LPARAM>(reinterpret_cast<HICON>(
				LoadImageW(hinst, MAKEINTRESOURCEW(_opts.icon_id), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR))));
	}

	ShowWindow(hwnd(), cmd_show);
	return _wli::run_main_loop(hwnd(), haccel);
}
