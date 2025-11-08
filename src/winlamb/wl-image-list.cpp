#include <system_error>
#include "image-list.h"
#include <shellapi.h>
using namespace wl;

ImageList& ImageList::operator=(ImageList &&other) noexcept {
	destroy();
	std::swap(_hImageList, other._hImageList);
	return *this;
}

HIMAGELIST ImageList::leak() {
	HIMAGELIST h = _hImageList;
	_hImageList = nullptr;
	return h;
}

void ImageList::destroy() noexcept {
	if (_hImageList) {
		ImageList_Destroy(_hImageList);
		_hImageList = nullptr;
	}
}

void ImageList::create(int cx, int cy, UINT ilcFlags, size_t initialCount, size_t growCount) {
	destroy();
	_hImageList = ImageList_Create(cx, cy, ilcFlags, static_cast<int>(initialCount), static_cast<int>(growCount));
	if (!_hImageList) [[unlikely]] {
		throw std::runtime_error("ImageList_Create failed.");
	}
}

const ImageList& ImageList::add_bitmap(HBITMAP hBmp, HBITMAP hBmpMask) const {
	int idx = ImageList_Add(_hImageList, hBmp, hBmpMask);
	if (idx == -1) [[unlikely]] {
		throw std::runtime_error("ImageList_Add failed.");
	}
	return *this;
}

const ImageList& ImageList::add_bitmap(HBITMAP hBmp, COLORREF crMask) const {
	int idx = ImageList_AddMasked(_hImageList, hBmp, crMask);
	if (idx == -1) [[unlikely]] {
		throw std::runtime_error("ImageList_AddMasked failed.");
	}
	return *this;
}

const ImageList& ImageList::add_icon(HICON hIcon) const {
	int idx = ImageList_AddIcon(_hImageList, hIcon);
	if (idx == -1) [[unlikely]] {
		throw std::runtime_error("ImageList_AddIcon failed.");
	}
	return *this;
}

const ImageList& ImageList::add_resource(WORD iconId) const {
	HINSTANCE hInst = GetModuleHandleW(nullptr);
	HICON hIcon = LoadIconW(hInst, MAKEINTRESOURCEW(iconId)); // no need to destroy it
	if (!hIcon) [[unlikely]] {
		std::wstring msg = str::fmt(L"LoadIcon resource %d failed", iconId);
		throw std::system_error(GetLastError(), std::system_category(), str::to_ansi(msg));
	}
	return add_icon(hIcon);
}

const ImageList& ImageList::add_shell_ext(WStrPtr fileExt) const {
	SIZE szIcon = icon_size();
	bool isIco16 = szIcon.cx == 16 && szIcon.cy == 16;
	bool isIco32 = szIcon.cx == 32 && szIcon.cy == 32;
	if (!isIco16 && !isIco32) [[unlikely]] {
		std::wstring msg = str::fmt(
			L"Only 16x16 or 32x32 icons can be loaded from the shell, currently: %dx%d.", szIcon.cx, szIcon.cy);
		throw std::invalid_argument(str::to_ansi(msg));
	}

	std::wstring pathExt;
	pathExt.reserve(10);
	pathExt = L"*.";
	pathExt.append(fileExt);

	struct Shfi final {
		~Shfi() { DestroyIcon(obj.hIcon); }
		SHFILEINFOW obj{};
	} shfi{};

	DWORD_PTR hr = SHGetFileInfoW(pathExt.c_str(), FILE_ATTRIBUTE_NORMAL, &shfi.obj, sizeof(SHFILEINFOW),
		SHGFI_USEFILEATTRIBUTES | SHGFI_ICON | (isIco16 ? SHGFI_SMALLICON : SHGFI_LARGEICON));
	if (FAILED(hr)) [[unlikely]] {
		throw std::system_error(static_cast<HRESULT>(hr), std::system_category(), "SHGetFileInfo failed");
	}

	return add_icon(shfi.obj.hIcon);
}

size_t ImageList::count() const {
	return ImageList_GetImageCount(_hImageList);
}

SIZE ImageList::icon_size() const {
	int cx = 0, cy = 0;
	if (!ImageList_GetIconSize(_hImageList, &cx, &cy)) [[unlikely]] {
		throw std::runtime_error("ImageList_GetIconSize failed.");
	}
	return {cx, cy};
}

const ImageList& ImageList::remove(size_t index) const {
	if (!ImageList_Remove(_hImageList, static_cast<int>(index))) [[unlikely]] {
		std::wstring msg = str::fmt(L"ImageList_Remove %d failed.", index);
		throw std::runtime_error(str::to_ansi(msg));
	}
	return *this;
}
