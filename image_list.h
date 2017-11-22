/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "icon.h"

namespace wl {

// Wrapper to image list object from Common Controls library.
class image_list final {
private:
	HIMAGELIST _hImgList = nullptr;

public:
	~image_list() {
		// Image lists are destroyed automatically in some cases,
		// but we'll destroy them always:
		// http://www.catch22.net/tuts/sysimgq
		// http://www.autohotkey.com/docs/commands/ListView.htm
		this->destroy();
	}

	image_list() = default;
	image_list(image_list&& other) { this->operator=(std::move(other)); }

	HIMAGELIST himagelist() const {
		return this->_hImgList;
	}

	image_list& operator=(image_list&& other) {
		this->destroy();
		std::swap(this->_hImgList, other._hImgList);
		return *this;
	}

	image_list& destroy() {
		if (this->_hImgList) {
			ImageList_Destroy(this->_hImgList);
			this->_hImgList = nullptr;
		}
		return *this;
	}

	image_list& create(SIZE resolution, UINT flags = ILC_COLOR32, WORD szInitial = 1, WORD szGrow = 1) {
		this->destroy();
		this->_hImgList = ImageList_Create(resolution.cx, resolution.cy, flags,
			static_cast<int>(szInitial), static_cast<int>(szGrow));
		if (!this->_hImgList) {
			throw std::system_error(GetLastError(), std::system_category(),
				"ImageList_Create failed");
		}
		return *this;
	}

	image_list& load(HICON hIcon) {
		if (!this->_hImgList) {
			throw std::logic_error("Can't add icon before create image list.");
		}
		ImageList_AddIcon(this->_hImgList, hIcon);
		return *this;
	}

	image_list& load(const icon& ico) {
		return this->load(ico.hicon());
	}

	image_list& load_from_resource(int iconId, HINSTANCE hInst = nullptr) {
		icon tmpIco;
		tmpIco.load_from_resource(iconId, this->resolution(), hInst);
		return this->load(tmpIco);
	}

	image_list& load_from_resource(std::initializer_list<int> iconIds, HINSTANCE hInst = nullptr) {
		for (const int iconId : iconIds) {
			this->load_from_resource(iconId, hInst);
		}
		return *this;
	}

	image_list& load_from_resource(int iconId, HWND hParent) {
		return this->load_from_resource(iconId,
			reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(hParent, GWLP_HINSTANCE)));
	}

	image_list& load_from_resource(std::initializer_list<int> iconIds, HWND hParent) {
		return this->load_from_resource(iconIds,
			reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(hParent, GWLP_HINSTANCE)));
	}

	image_list& load_from_shell(const wchar_t* fileExtension) {
		icon::res iRes = icon::util::resolve_resolution_type(this->resolution());
		if (iRes == icon::res::OTHER) {
			throw std::logic_error("Trying to load icon from shell with unsupported resolution.");
		}
		icon tmpIco;
		tmpIco.load_from_shell(fileExtension, iRes);
		return this->load(tmpIco);
	}

	image_list& load_from_shell(std::initializer_list<const wchar_t*> fileExtensions) {
		for (const wchar_t* ext : fileExtensions) {
			this->load_from_shell(ext);
		}
		return *this;
	}

	SIZE resolution() const {
		SIZE buf{};
		if (this->_hImgList) {
			ImageList_GetIconSize(this->_hImgList,
				reinterpret_cast<int*>(&buf.cx), reinterpret_cast<int*>(&buf.cy));
		}
		return buf;
	}

	size_t size() const {
		return this->_hImgList ? ImageList_GetImageCount(this->_hImgList) : 0;
	}
};

}//namespace wl
