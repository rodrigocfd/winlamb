/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <functional>
#include "../image_list.h"

namespace wl {
namespace _wli {

// Wrapper to image_list so it can be exposed as a public member of a control, like listview.
template<typename controlT>
class member_image_list final {
private:
	std::function<void()> _onCreate;
	controlT&             _owner;
	SIZE                  _resolution;
	image_list            _imageList;

public:
	member_image_list(controlT* pOwner, WORD resolution) noexcept :
		_owner(*pOwner), _resolution({resolution, resolution}) { }

	member_image_list(member_image_list&&) = default;
	member_image_list& operator=(member_image_list&&) = default; // movable only

	// Returns the handle to the image list.
	HIMAGELIST himagelist() const noexcept {
		return this->_imageList.himagelist();
	}

	// Returns how many images this image list has.
	size_t size() const noexcept {
		return this->_imageList.size();
	}

	// Callback to be called right after the image list is created.
	controlT& on_create(std::function<void()> callback) noexcept {
		this->_onCreate = std::move(callback);
		return this->_owner;
	}

	// Loads an icon into the image list.
	controlT& load(HICON hIcon) {
		this->_create_if_not_yet();
		this->_imageList.load(hIcon);
		return this->_owner;
	}

	// Loads an icon into the image list.
	controlT& load(const icon& ico) {
		this->_create_if_not_yet();
		this->_imageList.load(ico);
		return this->_owner;
	}

	// Loads an icon from resource into the image list.
	controlT& load_from_resource(int iconId) {
		this->_create_if_not_yet();
		this->_imageList.load_from_resource(iconId, this->_owner.hwnd());
		return this->_owner;
	}

	// Loads an icon from resource into the image list.
	controlT& load_from_resource(std::initializer_list<int> iconIds) {
		this->_create_if_not_yet();
		this->_imageList.load_from_resource(iconIds, this->_owner.hwnd());
		return this->_owner;
	}

	// Loads the icon used by Windows Explorer to represent the given file type.
	controlT& load_from_shell(const wchar_t* fileExtension) {
		this->_create_if_not_yet();
		this->_imageList.load_from_shell(fileExtension);
		return this->_owner;
	}

	// Loads the icon used by Windows Explorer to represent the given file type.
	controlT& load_from_shell(std::initializer_list<const wchar_t*> fileExtensions) {
		this->_create_if_not_yet();
		this->_imageList.load_from_shell(fileExtensions);
		return this->_owner;
	}

private:
	void _create_if_not_yet() {
		if (!this->_imageList.himagelist()) {
			this->_imageList.create(this->_resolution);
		}
		if (this->_onCreate) {
			this->_onCreate(); // to call stuff like ListView_SetImageList()
		}
	}
};

}//namepace _wli
}//namespace wl
