/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "../hwnd_base.h"
#include "store.h"
#include "params.h"
#include "params_wm.h"
#include "params_wmn.h"
#include "lippincott.h"

/**
 * hwnd_base
 *  w_inventory
 */

namespace wl {
class subclass; // friend forward declaration

namespace wli {
template<typename baseT> class dialog; // friend forward declarations
template<typename baseT> class window;

template<typename retT>
class w_inventory : public hwnd_base {
	friend class subclass;
	template<typename baseT> friend class dialog;
	template<typename baseT> friend class window;

public:
	using ntfT = std::pair<UINT_PTR, UINT>; // idFrom, code

private:
	store<UINT, retT> _msgs;
	store<WORD, retT> _cmds;
	store<ntfT, retT> _ntfs;
	bool              _canAdd = true;

protected:
	w_inventory() = default;

private:
	std::pair<bool, retT> _process_msg(UINT msg, WPARAM wp, LPARAM lp) noexcept {
		this->_canAdd = false; // lock, no further message handlers can be added

		using funcT = std::function<retT(params)>; // same of store<>::funcT for message, command and notify
		funcT* pFunc = nullptr; // user lambda

		// WM_COMMAND and WM_NOTIFY messages could have been orthogonally inserted into
		// store<> just like any other messages, however they'd be at the bottom of
		// the linear search, while still having their own internal linear searches
		// afterwards. Keeping them outside store<> eliminates the whole first search,
		// which is replaced by the switch below.

		switch (msg) {
		case WM_COMMAND:
			pFunc = this->_cmds.find(LOWORD(wp));
			break;
		case WM_NOTIFY:
			pFunc = this->_ntfs.find({
				reinterpret_cast<NMHDR*>(lp)->idFrom,
				reinterpret_cast<NMHDR*>(lp)->code
			});
			break;
		default:
			pFunc = this->_msgs.find(msg);
		}

		if (pFunc) {
			try { // any exception from a message lambda which was not caught
				return {true, (*pFunc)({msg, wp, lp})};
			} catch (...) {
				lippincott();
				PostQuitMessage(-1);
			}
		}
		return {false, -1}; // message not processed
	}

public:
	template<typename handlerT> void on_message(UINT msg, handlerT&& func)                                  { this->_can(); this->_msgs.add(msg, std::move(func)); }
	template<typename handlerT> void on_message(std::initializer_list<UINT> msgs, handlerT&& func)          { this->_can(); this->_msgs.add(msgs, std::move(func)); }
	template<typename handlerT> void on_command(WORD cmd, handlerT&& func)                                  { this->_can(); this->_cmds.add(cmd, std::move(func)); }
	template<typename handlerT> void on_command(const hwnd_base& ctrl, handlerT&& func)                     { this->_can(); this->_cmds.add(GetDlgCtrlID(ctrl.hwnd()), std::move(func)); }
	template<typename handlerT> void on_command(std::initializer_list<WORD> cmds, handlerT&& func)          { this->_can(); this->_cmds.add(cmds, std::move(func)); }
	template<typename handlerT> void on_notify(UINT_PTR idFrom, UINT code, handlerT&& func)                 { this->_can(); this->_ntfs.add({idFrom, code}, std::move(func)); }
	template<typename handlerT> void on_notify(const hwnd_base& ctrl, UINT code, handlerT&& func)           { this->_can(); this->_ntfs.add({GetDlgCtrlID(ctrl.hwnd()), code}, std::move(func)); }
	template<typename handlerT> void on_notify(ntfT idFromAndCode, handlerT&& func)                         { this->_can(); this->_ntfs.add(idFromAndCode, std::move(func)); }
	template<typename handlerT> void on_notify(std::initializer_list<ntfT> idFromAndCodes, handlerT&& func) { this->_can(); this->_ntfs.add(idFromAndCodes, std::move(func)); }

private:
	void _can() const {
		if (!this->_canAdd) {
			throw std::logic_error("Can't add a message handler after the loop started.\n"
				"This would be an unsafe operation, therefore it's explicitly forbidden.");
		}
	}
};

}//namespace wli
}//namespace wl
