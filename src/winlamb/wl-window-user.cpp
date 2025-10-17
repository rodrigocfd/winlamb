#include "window-user.h"
using namespace _wl_internal;
using namespace wl;

int WindowMain::run(HINSTANCE hInst, int cmdShow) {
	return _rawMain.has_value()
		? _rawMain.value().run(hInst, cmdShow)
		: _dlgMain.value().run(hInst, cmdShow);
}

const WindowMsg& WindowMain::wnd_msg() const {
	return _rawMain.has_value()
		? _rawMain.value()._rawBase._wndMsg
		: _dlgMain.value()._dlgBase._wndMsg;
}

WindowMsg& WindowMain::wnd_msg() {
	return const_cast<WindowMsg&>(std::as_const(*this).wnd_msg()); // https://stackoverflow.com/a/47369227/6923555
}

////////////////////////////////////////////////////////////////////////////////

void WindowModal::show() {
	return _rawModal.has_value()
		? _rawModal.value().show()
		: _dlgModal.value().show();
}

const WindowMsg& WindowModal::wnd_msg() const {
	return _rawModal.has_value()
		? _rawModal.value()._rawBase._wndMsg
		: _dlgModal.value()._dlgBase._wndMsg;
}

WindowMsg& WindowModal::wnd_msg() {
	return const_cast<WindowMsg&>(std::as_const(*this).wnd_msg()); // https://stackoverflow.com/a/47369227/6923555
}
