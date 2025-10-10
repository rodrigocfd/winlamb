#include "window-final.h"
using namespace _wl_internal;
using namespace wl;

std::wstring WindowMain::title() const {
	return wnd_msg()._wnd.text();
}

void WindowMain::set_title(WStrPtr title) const {
	wnd_msg()._wnd.set_text(title);
}

int WindowMain::run(HINSTANCE hInst, int cmdShow) {
	return _dlgMain.has_value() ? _dlgMain.value().run(hInst, cmdShow) : -1;
}

const WindowMsg& WindowMain::wnd_msg() const {
	return _rawMain.has_value()
		? _rawMain.value()._dlgBase._wndMsg
		: _dlgMain.value()._dlgBase._wndMsg;
}

WindowMsg& WindowMain::wnd_msg() {
	return const_cast<WindowMsg&>(std::as_const(*this).wnd_msg()); // https://stackoverflow.com/a/47369227/6923555
}
