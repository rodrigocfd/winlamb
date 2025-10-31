#include "window-user.h"
using namespace _wl_internal;
using namespace wl;

int WindowMain::run(HINSTANCE hInst, int cmdShow) {
	return _rawMain.has_value()
		? _rawMain.value().run(hInst, cmdShow)
		: _dlgMain.value().run(hInst, cmdShow);
}

////////////////////////////////////////////////////////////////////////////////

void WindowModal::show() {
	return _rawModal.has_value()
		? _rawModal.value().show()
		: _dlgModal.value().show();
}
