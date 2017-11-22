/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <system_error>
#include <Windows.h>
#include <CommCtrl.h>
#include <commoncontrols.h> // IID_IImageList
#include "com.h"

namespace wl {

// Wrapper to HICON handle.
class icon final {
private:
	HICON _hIcon = nullptr;

public:
	enum class res : BYTE {
		SMALL16      = SHIL_SMALL,
		LARGE32      = SHIL_LARGE,
		EXTRALARGE48 = SHIL_EXTRALARGE,
		JUMBO256     = SHIL_JUMBO,
		OTHER        = SHIL_LAST + 1
	};

	~icon() {
		this->destroy();
	}

	icon() = default;
	icon(icon&& other) { this->operator=(std::move(other)); }

	HICON hicon() const {
		return this->_hIcon;
	}

	icon& operator=(icon&& other) {
		this->destroy();
		std::swap(this->_hIcon, other._hIcon);
		return *this;
	}

	icon& destroy() {
		if (this->_hIcon) {
			DestroyIcon(this->_hIcon);
			this->_hIcon = nullptr;
		}
		return *this;
	}

	icon& load_from_resource(int iconId, SIZE resolution, HINSTANCE hInst = nullptr) {
		this->destroy();
		this->_hIcon = static_cast<HICON>(LoadImageW(hInst ? hInst : GetModuleHandleW(nullptr),
			MAKEINTRESOURCEW(iconId), IMAGE_ICON,
			static_cast<int>(resolution.cx), static_cast<int>(resolution.cy),
			LR_DEFAULTCOLOR));
		if (!this->_hIcon) {
			throw std::system_error(GetLastError(), std::system_category(),
				"LoadImage failed when trying to load icon resource");
		}
	}

	icon& load_from_resource(int iconId, SIZE resolution, HWND hParent) {
		return this->load_from_resource(iconId, resolution,
			reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(hParent, GWLP_HINSTANCE)));
	}

	// Loads the icon used by Windows Explorer to represent the given file type.
	icon& load_from_shell(const wchar_t* fileExtension, res resolution) {
		this->destroy();
		wchar_t extens[16]{}; // arbitrary length
		lstrcpyW(extens, (fileExtension[0] == L'.') ? L"*" : L"*."); // prepend dot if it doesn't have
		lstrcatW(extens, fileExtension);

		com::lib comLib{com::lib::init::NOW};
		SHFILEINFO shfi{};

		if (resolution == res::SMALL16 || resolution == res::LARGE32) { // http://stackoverflow.com/a/28015423
			DWORD_PTR gfiOk = SHGetFileInfoW(extens, FILE_ATTRIBUTE_NORMAL, &shfi, sizeof(shfi),
				SHGFI_USEFILEATTRIBUTES | SHGFI_ICON |
				(resolution == res::SMALL16 ? SHGFI_SMALLICON : SHGFI_LARGEICON));
			if (!gfiOk) {
				throw std::system_error(GetLastError(), std::system_category(),
					"SHGetFileInfo failed when trying to load icon from shell");
			} else {
				this->_hIcon = shfi.hIcon;
			}
		} else if (resolution != res::OTHER) {
			IImageList* pImgList = nullptr; // http://stackoverflow.com/a/30496252
			HRESULT hr = SHGetImageList(static_cast<int>(resolution),
				IID_IImageList, reinterpret_cast<void**>(&pImgList));
			if (FAILED(hr)) {
				throw std::system_error(hr, std::system_category(),
					"SHGetImageList failed when trying to load icon from shell");
			} else {
				DWORD_PTR gfiOk = SHGetFileInfoW(extens, FILE_ATTRIBUTE_NORMAL, &shfi, sizeof(shfi),
					SHGFI_USEFILEATTRIBUTES | SHGFI_SYSICONINDEX);
				if (!gfiOk) {
					throw std::system_error(GetLastError(), std::system_category(),
						"SHGetFileInfo failed when trying to load system's image list");
				} else {
					this->_hIcon = ImageList_GetIcon(reinterpret_cast<HIMAGELIST>(pImgList),
						shfi.iIcon, ILD_NORMAL);
				}
			}
		}

		return *this;
	}

	icon& icon_to_label(HWND hStatic) {
		// Loads an icon into a static control; the icon can be safely destroyed then.
		// On the resource editor, change "Type" property to "Icon".
		SendMessageW(hStatic, STM_SETIMAGE, IMAGE_ICON, reinterpret_cast<LPARAM>(this->_hIcon));
		return *this;
	}

	SIZE resolution() const {
		SIZE sz{}; // http://stackoverflow.com/a/13923853
		if (this->_hIcon) {
			BITMAP bmp{};
			ICONINFO nfo{};
			GetIconInfo(this->_hIcon, &nfo);

			if (nfo.hbmColor) {
				int nWrittenBytes = GetObjectW(nfo.hbmColor, sizeof(bmp), &bmp);
				if (nWrittenBytes > 0) {
					sz.cx = bmp.bmWidth;
					sz.cy = bmp.bmHeight;
					//myinfo.nBitsPerPixel = bmp.bmBitsPixel;
				}
			} else if (nfo.hbmMask) {
				int nWrittenBytes = GetObjectW(nfo.hbmMask, sizeof(bmp), &bmp);
				if (nWrittenBytes > 0) {
					sz.cx = bmp.bmWidth;
					sz.cy = bmp.bmHeight / 2;
					//myinfo.nBitsPerPixel = 1;
				}
			}

			if (nfo.hbmColor) DeleteObject(nfo.hbmColor);
			if (nfo.hbmMask) DeleteObject(nfo.hbmMask);
		}
		return sz;
	}

	res resolution_type() const {
		return util::resolve_resolution_type(this->resolution());
	}

public:
	class util final {
	private:
		util() = delete;

	public:
		// Converts a SIZE resolution to the equivalent enum value.
		static res resolve_resolution_type(SIZE sz) {
			if (sz.cx == 16 && sz.cy == 16) return res::SMALL16;
			else if (sz.cx == 32 && sz.cy == 32) return res::LARGE32;
			else if (sz.cx == 48 && sz.cy == 48) return res::EXTRALARGE48;
			else if (sz.cx == 256 && sz.cy == 256) return res::JUMBO256;
			return res::OTHER;
		}
	};
};

}//namespace wl
