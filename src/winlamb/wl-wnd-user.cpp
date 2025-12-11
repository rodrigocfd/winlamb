#include "wnd-user.h"
#include <Uxtheme.h>
#include <vsstyle.h>
using namespace wl;
using namespace _wl_internal;

int WindowMain::run(HINSTANCE hInst, int cmdShow) {
	return _rawMain.has_value()
		? _rawMain.value().run(hInst, cmdShow)
		: _dlgMain.value().run(hInst, cmdShow);
}

const WindowMain& WindowMain::set_title(WStrView title) const {
	set_wnd_text(hwnd(), title);
	return *this;
}

////////////////////////////////////////////////////////////////////////////////

void WindowModal::show() {
	return _rawModal.has_value()
		? _rawModal.value().show()
		: _dlgModal.value().show();
}

const WindowModal& WindowModal::set_title(WStrView title) const {
	_wl_internal::set_wnd_text(hwnd(), title);
	return *this;
}

////////////////////////////////////////////////////////////////////////////////

WindowControl::WindowControl(WindowParent &parent)
	: _rawControl{std::make_optional<RawControl>(parent)}
{
	on().wm_nc_paint(std::bind(&WindowControl::paint_custom_border, this, std::placeholders::_1));
}

WindowControl::WindowControl(WindowParent &parent, WORD dlgId, WORD ctrlId, POINT pos, Lay layout)
	: _dlgControl{std::make_optional<DlgControl>(parent, dlgId, ctrlId, pos, layout)}
{
	on().wm_nc_paint(std::bind(&WindowControl::paint_custom_border, this, std::placeholders::_1));
}

WORD WindowControl::ctrl_id() const {
	return GetDlgCtrlID(hwnd());
}

void WindowControl::paint_custom_border(wm::NcPaint p) const {
	DefWindowProcW(hwnd(), WM_NCPAINT, p.wparam(), p.lparam()); // make the system draw the scrollbar for us

	DWORD exStyle = static_cast<DWORD>(GetWindowLongPtrW(hwnd(), GWL_EXSTYLE));
	bool hasBorderStyle = (exStyle & WS_EX_CLIENTEDGE) != 0;
	if (!hasBorderStyle || !IsThemeActive() || !IsAppThemed())
		return; // no theme borders to be painted

	RECT rc{};
	GetWindowRect(hwnd(), &rc); // control window outmost coordinates, including margins
	ScreenToClient(hwnd(), reinterpret_cast<POINT*>(&rc));
	ScreenToClient(hwnd(), reinterpret_cast<POINT*>(&rc.right));
	OffsetRect(&rc, 2, 2); // because it comes up anchored at -2,-2

	HDC hdc = GetWindowDC(hwnd());

	// The HRGN which comes in WM_NCPAINT seems to be invalid, so we carve our own.
	RECT rcHole = rc;
	InflateRect(&rcHole, -2, -2);
	HRGN hRgnHole = CreateRectRgnIndirect(&rcHole);
	HRGN hRgnClip = CreateRectRgnIndirect(&rc);
	CombineRgn(hRgnClip, hRgnClip, hRgnHole, RGN_DIFF);
	SelectClipRgn(hdc, hRgnClip);

	HTHEME hTheme = OpenThemeData(hwnd(), L"LISTVIEW");
	DrawThemeBackground(hTheme, hdc, LVP_LISTDETAIL, 0, &rc, nullptr);

	CloseThemeData(hTheme);
	DeleteObject(hRgnClip);
	DeleteObject(hRgnHole);
	ReleaseDC(hwnd(), hdc);
}
