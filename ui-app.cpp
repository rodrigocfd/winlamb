#include <system_error>
#include "ui-app.hpp"
#include <VersionHelpers.h>
#include <ole2.h>
#include <shellapi.h>
#pragma comment(lib, "comctl32.lib") // not linked by default by MSVC
#pragma comment(lib, "uxtheme.lib")
using namespace _wl_internal;
using namespace wl;

int GuiApp::logPixelsX = 0, GuiApp::logPixelsY = 0;
HFONT GuiApp::hUiFont = nullptr;

GuiApp::~GuiApp() {
	DeleteObject(hUiFont);
	OleUninitialize();
}

GuiApp::GuiApp() {
	InitCommonControls();
	OleInitialize(nullptr);

	if (IsWindows8OrGreater()) [[likely]] {
		HANDLE hProcess = GetCurrentProcess();
		BOOL bVal = FALSE;
		BOOL ok = SetUserObjectInformationW(hProcess, UOI_TIMERPROC_EXCEPTION_SUPPRESSION, &bVal, sizeof(BOOL));
		#ifdef _DEBUG
		if (!ok) {
			DWORD err = GetLastError();
			if (err == ERROR_INVALID_PARAMETER) {
				// Do nothing: Wine doesn't support SetUserObjectInformation for now.
				// https://bugs.winehq.org/show_bug.cgi?id=54951
			} else {
				throw std::system_error(err, std::system_category(), "SetUserObjectInformation failed");
			}
		}
		#endif
	}

	HDC hdcScreen = GetDC(nullptr);
	logPixelsX = GetDeviceCaps(hdcScreen, LOGPIXELSX);
	logPixelsY = GetDeviceCaps(hdcScreen, LOGPIXELSY);
	ReleaseDC(nullptr, hdcScreen);

	NONCLIENTMETRICSW ncm{.cbSize = sizeof(NONCLIENTMETRICSW)};
	SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);
	hUiFont = CreateFontIndirectW(&ncm.lfMenuFont);
}

////////////////////////////////////////////////////////////////////////////////

void _wl_internal::apply_ui_font(HWND hWnd) {
	SendMessageW(hWnd, WM_SETFONT, reinterpret_cast<WPARAM>(GuiApp::hUiFont), TRUE);
}

WORD _wl_internal::valid_ctrl_id(WORD ctrlId) {
	static WORD globalCtrId = 0xdfff; // https://stackoverflow.com/a/18192766/6923555
	return ctrlId ? ctrlId : globalCtrId--;
}

HINSTANCE _wl_internal::wnd_hinst(HWND hWnd) {
	return reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(hWnd, GWLP_HINSTANCE));
}

std::wstring _wl_internal::wnd_text(HWND hWnd) {
	UINT len = GetWindowTextLengthW(hWnd);
	if (!len) {
		DWORD err = GetLastError();
		if (err != ERROR_SUCCESS) [[unlikely]] {
			throw std::system_error(err, std::system_category(), "GetWindowTextLength failed");
		} else [[likely]] {
			return std::wstring{}; // actual empty string
		}
	}

	std::wstring buf(len + 1, L'\0'); // alloc receiving buffer
	GetWindowTextW(hWnd, buf.data(), len + 1);
	buf.resize(len);
	return buf;
}

void _wl_internal::set_wnd_text(HWND hWnd, WStrView text) {
	if (!SetWindowTextW(hWnd, text.c_str())) [[unlikely]] {
		throw std::system_error(GetLastError(), std::system_category(), "SetWindowText failed");
	}
}

void _wl_internal::focus(HWND hCtrl) {
	PostMessageW(GetParent(hCtrl), WM_NEXTDLGCTL, reinterpret_cast<LPARAM>(hCtrl), MAKELONG(TRUE, 0));
}

void _wl_internal::screen_to_client_rc(HWND hWnd, RECT *pRc) {
	ScreenToClient(hWnd, reinterpret_cast<POINT*>(pRc));
	ScreenToClient(hWnd, reinterpret_cast<POINT*>(&pRc->right));
}

SIZE _wl_internal::calc_text_bound_box(WStrView text) {
	std::wstring wtext{text.c_str()};
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
		throw std::runtime_error{"Failed to clone desktop HDC."};
	#endif
	desktop.hFontPrev = reinterpret_cast<HFONT>(SelectObject(desktop.hdcCloned, GuiApp::hUiFont));

	SIZE bounds{};
	BOOL ok = GetTextExtentPoint32W(desktop.hdcCloned, wtext.c_str(), static_cast<int>(wtext.length()), &bounds);
	#ifdef _DEBUG
	if (!ok)
		throw std::runtime_error{"Calc box failed to \"" + str::to_ansi(wtext) + "\"."};
	#endif

	if (isEmpty)
		bounds.cx = 0; // if no text was given, return just the height

	return bounds;
}

SIZE _wl_internal::calc_text_bound_box_with_check(wl::WStrView text) {
	SIZE bounds = calc_text_bound_box(text);
	bounds.cx += GetSystemMetrics(SM_CXMENUCHECK) + GetSystemMetrics(SM_CXEDGE); // https://stackoverflow.com/a/1165052/6923555

	int cyCheck = GetSystemMetrics(SM_CXMENUCHECK);
	if (cyCheck > bounds.cy) // check is taller than the text?
		bounds.cy = cyCheck;

	return bounds;
}

////////////////////////////////////////////////////////////////////////////////

int wl::dpi::x(int xVal) {
	return MulDiv(xVal, GuiApp::logPixelsX, 96);
}

int wl::dpi::y(int yVal) {
	return MulDiv(yVal, GuiApp::logPixelsY, 96);
}

POINT wl::dpi::pt(int xVal, int yVal) {
	return {.x = x(xVal), .y = y(yVal)};
}

POINT wl::dpi::pt(POINT value) {
	return pt(value.x, value.y);
}

SIZE wl::dpi::sz(int xVal, int yVal) {
	return {.cx = x(xVal), .cy = y(yVal)};
}

SIZE wl::dpi::sz(SIZE value) {
	return sz(value.cx, value.cy);
}

//------------------------------------------------------------------------------

[[nodiscard]] static HICON load_icon_res(WORD iconId, SIZE szIcon) {
	HINSTANCE hInst = GetModuleHandleW(nullptr);
	HICON hIcon = reinterpret_cast<HICON>(
		LoadImageW(hInst, MAKEINTRESOURCEW(iconId), IMAGE_ICON, szIcon.cx, szIcon.cy, LR_DEFAULTCOLOR));
	#ifdef _DEBUG
	if (!hIcon) {
		std::wstring msg = str::fmt(L"LoadImage resource %d failed", iconId);
		throw std::system_error(GetLastError(), std::system_category(), str::to_ansi(msg));
	}
	#endif
	return hIcon;
}

[[nodiscard]] static HICON load_icon_shell_ext(WStrView fileExt, SIZE szIcon) {
	bool isIco16 = szIcon.cx == 16 && szIcon.cy == 16;
	bool isIco32 = szIcon.cx == 32 && szIcon.cy == 32;
	#ifdef _DEBUG
	if (!isIco16 && !isIco32) {
		std::wstring msg = str::fmt(
			L"Only 16x16 or 32x32 icons can be loaded from the shell, currently: %dx%d.", szIcon.cx, szIcon.cy);
		throw std::invalid_argument{str::to_ansi(msg)};
	}
	#endif

	std::wstring pathExt;
	pathExt.reserve(10);
	pathExt = L"*.";
	pathExt.append(fileExt.c_str());

	SHFILEINFOW shfi{};
	DWORD_PTR hr = SHGetFileInfoW(pathExt.c_str(), FILE_ATTRIBUTE_NORMAL, &shfi, sizeof(SHFILEINFOW),
		SHGFI_USEFILEATTRIBUTES | SHGFI_ICON | (isIco16 ? SHGFI_SMALLICON : SHGFI_LARGEICON));
	#ifdef _DEBUG
	if (FAILED(hr))
		throw std::system_error(static_cast<HRESULT>(hr), std::system_category(), "SHGetFileInfo failed");
	#endif
	return shfi.hIcon;
}

//------------------------------------------------------------------------------

ImageList::~ImageList() {
	if (_hImageList)
		ImageList_Destroy(_hImageList);
}

void ImageList::create(UINT ilcFlags) {
	_hImageList = ImageList_Create(_szIcon.cx, _szIcon.cy, ilcFlags, 0, 0);
	#ifdef _DEBUG
	if (!_hImageList)
		throw std::runtime_error{"ImageList_Create failed."};
	#endif
}

size_t ImageList::count() const {
	return ImageList_GetImageCount(_hImageList);
}

void ImageList::add_icon(HICON hIcon) {
	int idx = ImageList_AddIcon(_hImageList, hIcon);
	DestroyIcon(hIcon);
	#ifdef _DEBUG
	if (idx == -1)
		throw std::runtime_error{"ImageList_AddIcon failed."};
	#endif
}

void ImageList::add_resource(WORD iconId) {
	add_icon(load_icon_res(iconId, _szIcon));
}

void ImageList::add_shell_ext(WStrView fileExt) {
	add_icon(load_icon_shell_ext(fileExt, _szIcon));
}

//------------------------------------------------------------------------------

HIconStore::~HIconStore() {
	for (auto &&hIcon : _hIcons)
		DestroyIcon(hIcon);
}

void HIconStore::add_icon(HICON hIcon) {
	_hIcons.push_back(hIcon);
}

void HIconStore::add_resource(WORD iconId) {
	_hIcons.push_back(load_icon_res(iconId, _szIcon));
}

void HIconStore::add_shell_ext(wl::WStrView fileExt) {
	_hIcons.push_back(load_icon_shell_ext(fileExt, _szIcon));
}

//------------------------------------------------------------------------------

void Layout::add(HWND hCtrl, wl::Lay lay) {
	if (lay == Lay::hold_hold)
		return; // nothing to do, don't even bother adding the control

	HWND hParent = GetParent(hCtrl);

	if (_ctrls.empty()) { // first control being added?
		RECT rcParent{};
		BOOL ok = GetClientRect(hParent, &rcParent);
		#ifdef _DEBUG
		if (!ok)
			throw std::system_error(GetLastError(), std::system_category(), "GetClientRect failed");
		#endif
		_szOrig = {.cx = rcParent.right, .cy = rcParent.bottom}; // save original parent client area
	}

	RECT rcCtrl{};
	BOOL ok = GetWindowRect(hCtrl, &rcCtrl); // relative to screen
	#ifdef _DEBUG
	if (!ok)
		throw std::system_error(GetLastError(), std::system_category(), "GetWindowRect failed");
	#endif
	screen_to_client_rc(hParent, &rcCtrl); // now relative to parent

	_ctrls.emplace_back(hCtrl, lay, rcCtrl);
}

void Layout::rearrange(WPARAM wp, LPARAM lp) {
	if (_ctrls.empty() || wp == SIZE_MINIMIZED)
		return; // no need to resize if window is minimized

	HDWP hdwp = BeginDeferWindowPos(static_cast<int>(_ctrls.size()));
	#ifdef _DEBUG
	if (!hdwp)
		throw std::system_error(GetLastError(), std::system_category(), "BeginDeferWindowPos failed");
	#endif

	SIZE sz = {.cx = LOWORD(lp), .cy = HIWORD(lp)}; // of window client area

	for (auto &&ctrl : _ctrls) {
		WORD flags = SWP_NOZORDER;
		switch (ctrl.lay) {
		case Lay::move_move: // repos both horz and vert
			flags |= SWP_NOSIZE;
			break;
		case Lay::resize_resize: // resize both horz and vert
			flags |= SWP_NOMOVE;
		}

		DeferWindowPos(hdwp, ctrl.hCtrl, nullptr,
			(ctrl.lay == Lay::move_hold || ctrl.lay == Lay::move_move || ctrl.lay == Lay::move_resize) // horz move
				? sz.cx - _szOrig.cx + ctrl.rcOrig.left
				: ctrl.rcOrig.left, // keep original horz pos
			(ctrl.lay == Lay::hold_move || ctrl.lay == Lay::move_move || ctrl.lay == Lay::resize_move) // vert move
				? sz.cy - _szOrig.cy + ctrl.rcOrig.top
				: ctrl.rcOrig.top, // keep original vert pos
			(ctrl.lay == Lay::resize_hold || ctrl.lay == Lay::resize_move || ctrl.lay == Lay::resize_resize) // horz resize
				? sz.cx - _szOrig.cx + ctrl.rcOrig.right - ctrl.rcOrig.left
				: ctrl.rcOrig.right - ctrl.rcOrig.left, // keep original width
			(ctrl.lay == Lay::hold_resize || ctrl.lay == Lay::move_resize || ctrl.lay == Lay::resize_resize) // vert resize
				? sz.cy - _szOrig.cy + ctrl.rcOrig.bottom - ctrl.rcOrig.top
				: ctrl.rcOrig.bottom - ctrl.rcOrig.top, // keep original height
			flags);
	}

	EndDeferWindowPos(hdwp);
}
