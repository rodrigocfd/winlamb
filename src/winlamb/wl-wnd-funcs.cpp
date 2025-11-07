#include <system_error>
#include "wnd-funcs.h"
using namespace wl;

WORD _wl_internal::valid_ctrl_id(WORD ctrlId) {
	static WORD globalCtrId = 0xdfff; // https://stackoverflow.com/a/18192766/6923555
	return ctrlId ? ctrlId : globalCtrId--;
}

std::wstring _wl_internal::wnd_text(HWND hWnd) {
	UINT len = GetWindowTextLengthW(hWnd);
	if (!len) {
		DWORD err = GetLastError();
		if (err != ERROR_SUCCESS) [[unlikely]] {
			throw std::system_error(err, std::system_category(), "GetWindowTextLength failed");
		}
		return std::wstring{}; // actual empty string
	}

	std::wstring buf(len + 1, L'\0'); // alloc receiving buffer
	GetWindowTextW(hWnd, buf.data(), len + 1);
	buf.resize(len);
	return buf;
}

void _wl_internal::set_wnd_text(HWND hWnd, WStrPtr text) {
	if (!SetWindowTextW(hWnd, text)) [[unlikely]] {
		throw std::system_error(GetLastError(), std::system_category(), "SetWindowText failed");
	}
}
