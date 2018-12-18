/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "lippincott.h"
#include "params_wm.h"
#include "params_wmn.h"
#include "store.h"

namespace wl {
namespace wli {

// Stores and processes window messages.
template<typename retT>
class base_msg final {
private:
	bool        _canAdd = true;
	const HWND& _hWnd;

public:
	store<UINT, retT>                      msgs;
	store<WORD, retT>                      cmds;
	store<std::pair<UINT_PTR, UINT>, retT> ntfs; // idFrom, code

	base_msg(const HWND& hWnd) noexcept :
		_hWnd(hWnd) { }

	// Returns the window handle.
	HWND hwnd() const noexcept {
		return this->_hWnd;
	}

	std::pair<bool, retT> process_msg(UINT msg, WPARAM wp, LPARAM lp) noexcept {
		this->_canAdd = false; // lock, no further message handlers can be added
		std::function<retT(params)>* pUserLambda = nullptr;

		// WM_COMMAND and WM_NOTIFY messages could have been orthogonally inserted into
		// store<> just like any other messages, however they'd be at the bottom of
		// the linear search, while still having their own internal linear searches
		// afterwards. Keeping them outside store<> eliminates the whole first search,
		// which is replaced by the switch below.

		switch (msg) {
		case WM_COMMAND:
			pUserLambda = this->cmds.find(LOWORD(wp));
			break;
		case WM_NOTIFY:
			pUserLambda = this->ntfs.find({
				reinterpret_cast<NMHDR*>(lp)->idFrom,
				reinterpret_cast<NMHDR*>(lp)->code
			});
			break;
		default:
			pUserLambda = this->msgs.find(msg);
		}

		if (pUserLambda) {
			try { // any exception from a message lambda which was not caught
				return {true, (*pUserLambda)({msg, wp, lp})};
			} catch (...) {
				lippincott();
				PostQuitMessage(-1);
			}
		}
		return {false, -1}; // message not processed
	}

	void throw_if_cant_add() const {
		if (!this->_canAdd) {
			throw std::logic_error("Can't add a message handler after the loop started.\n"
				"This would be an unsafe operation, therefore it's explicitly forbidden.");
		}
	}
};

}//namespace wli
}//namespace wl
