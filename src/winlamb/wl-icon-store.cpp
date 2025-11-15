#include <system_error>
#include "icon-store.h"
#include <shellapi.h>
using namespace wl;
using namespace _wl_internal;

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

[[nodiscard]] static HICON load_icon_shell_ext(WStrPtr fileExt, SIZE szIcon) {
	bool isIco16 = szIcon.cx == 16 && szIcon.cy == 16;
	bool isIco32 = szIcon.cx == 32 && szIcon.cy == 32;
	#ifdef _DEBUG
	if (!isIco16 && !isIco32) {
		std::wstring msg = str::fmt(
			L"Only 16x16 or 32x32 icons can be loaded from the shell, currently: %dx%d.", szIcon.cx, szIcon.cy);
		throw std::invalid_argument(str::to_ansi(msg));
	}
	#endif

	std::wstring pathExt;
	pathExt.reserve(10);
	pathExt = L"*.";
	pathExt.append(fileExt);

	SHFILEINFOW shfi{};
	DWORD_PTR hr = SHGetFileInfoW(pathExt.c_str(), FILE_ATTRIBUTE_NORMAL, &shfi, sizeof(SHFILEINFOW),
		SHGFI_USEFILEATTRIBUTES | SHGFI_ICON | (isIco16 ? SHGFI_SMALLICON : SHGFI_LARGEICON));
	#ifdef _DEBUG
	if (FAILED(hr))
		throw std::system_error(static_cast<HRESULT>(hr), std::system_category(), "SHGetFileInfo failed");
	#endif
	return shfi.hIcon;
}

////////////////////////////////////////////////////////////////////////////////

ImageList::~ImageList() {
	if (_hImageList)
		ImageList_Destroy(_hImageList);
}

void ImageList::create(UINT ilcFlags) {
	_hImageList = ImageList_Create(_szIcon.cx, _szIcon.cy, ilcFlags, 0, 0);
	#ifdef _DEBUG
	if (!_hImageList)
		throw std::runtime_error("ImageList_Create failed.");
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
		throw std::runtime_error("ImageList_AddIcon failed.");
	#endif
}

void ImageList::add_resource(WORD iconId) {
	add_icon(load_icon_res(iconId, _szIcon));
}

void ImageList::add_shell_ext(WStrPtr fileExt) {
	add_icon(load_icon_shell_ext(fileExt, _szIcon));
}

////////////////////////////////////////////////////////////////////////////////

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

void HIconStore::add_shell_ext(wl::WStrPtr fileExt) {
	_hIcons.push_back(load_icon_shell_ext(fileExt, _szIcon));
}
