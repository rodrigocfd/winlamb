#include <system_error>
#include "wnd-funcs.h"
using namespace wl;

static HFONT hUiFont = nullptr;

HFONT _wl_internal::ui_font() {
	if (!hUiFont) { // not cached yet?
		NONCLIENTMETRICSW ncm{.cbSize = sizeof(NONCLIENTMETRICSW)};
		SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);
		hUiFont = CreateFontIndirectW(&ncm.lfMenuFont);
	}
	return hUiFont;
}

void _wl_internal::apply_ui_font(HWND hWnd) {
	SendMessageW(hWnd, WM_SETFONT, reinterpret_cast<WPARAM>(ui_font()), TRUE);
}

void _wl_internal::delete_ui_font() {
	if (hUiFont) {
		DeleteObject(hUiFont);
		hUiFont = nullptr;
	}
}

HINSTANCE _wl_internal::wnd_hinst(HWND hWnd) {
	return reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(hWnd, GWLP_HINSTANCE));
}

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

SIZE _wl_internal::calc_text_bound_box(WStrPtr text) {
	std::wstring wtext{text};
	str::trim(wtext);
	bool isEmpty = wtext.empty();
	if (isEmpty)
		wtext = L"Pj"; // just a placeholder to retrieve the line height

	struct Desktop final {
		~Desktop() {
			if (hFontPrev) SelectObject(hdcCloned, hFontPrev); // make sure resources will be freed
			if (hdcCloned) DeleteDC(hdcCloned);
			if (hdcOrig) ReleaseDC(hWnd, hdcOrig);
		}
		HWND hWnd = nullptr;
		HDC hdcOrig = nullptr;
		HDC hdcCloned = nullptr;
		HFONT hFontPrev = nullptr;
	} desktop{};

	desktop.hWnd = GetDesktopWindow();
	desktop.hdcOrig = GetDC(desktop.hWnd);
	desktop.hdcCloned = CreateCompatibleDC(desktop.hdcOrig);
	#ifdef _DEBUG
	if (!desktop.hdcCloned)
		throw std::runtime_error("Failed to clone desktop HDC.");
	#endif
	desktop.hFontPrev = reinterpret_cast<HFONT>(SelectObject(desktop.hdcCloned, ui_font()));

	SIZE bounds{};
	BOOL ret = GetTextExtentPoint32W(desktop.hdcCloned, wtext.c_str(), static_cast<int>(wtext.length()), &bounds);
	#ifdef _DEBUG
	if (!ret)
		throw std::runtime_error(str::to_ansi(str::fmt(L"Calc box failed to \"%s\".", wtext.c_str())));
	#endif

	if (isEmpty)
		bounds.cx = 0; // if no text was given, return just the height

	return bounds;
}

SIZE _wl_internal::calc_text_bound_box_with_check(wl::WStrPtr text) {
	SIZE bounds = calc_text_bound_box(text);
	bounds.cx += GetSystemMetrics(SM_CXMENUCHECK) + GetSystemMetrics(SM_CXEDGE); // https://stackoverflow.com/a/1165052/6923555

	int cyCheck = GetSystemMetrics(SM_CXMENUCHECK);
	if (cyCheck > bounds.cy) // check is taller than the text?
		bounds.cy = cyCheck;

	return bounds;
}
