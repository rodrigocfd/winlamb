/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "hwnd_wrapper.h"
#include "store.h"
#include "params.h"
#include "params_wm.h"
#include "params_wmn.h"

/**
 * hwnd_wrapper
 *  inventory
 */

namespace wl {
class subclass; // friend forward declaration

namespace wli {
template<typename baseT> class dialog; // friend forward declarations
template<typename baseT> class window;

template<typename retT>
class inventory : public hwnd_wrapper {
	friend class subclass;
	template<typename baseT> friend class dialog;
	template<typename baseT> friend class window;

public:
	using ntfT = std::pair<UINT_PTR, UINT>; // idFrom, code
	using funcT = std::function<retT(params)>; // same of store<>::funcT

private:
	store<UINT, retT> _msgs;
	store<WORD, retT> _cmds;
	store<ntfT, retT> _ntfs;

protected:
	inventory() = default;

private:
	std::pair<bool, retT> _process_msg(params p) {
		// WM_COMMAND and WM_NOTIFY messages could have been orthogonally inserted into
		// store_msgT just like any other messages, however they'd be at the bottom of
		// the linear search, while still having their own internal linear searches
		// afterwards. Keeping them outside store<> eliminates the whole first search.

		funcT* pFunc = nullptr; // user lambda

		switch (p.message) {
		case WM_COMMAND:
			pFunc = this->_cmds.find(LOWORD(p.wParam));
			break;
		case WM_NOTIFY:
			pFunc = this->_ntfs.find({
				reinterpret_cast<NMHDR*>(p.lParam)->idFrom,
				reinterpret_cast<NMHDR*>(p.lParam)->code
			});
			break;
		default:
			pFunc = this->_msgs.find(p.message);
		}

		if (pFunc) {
			try { // any exception from a message lambda which was not caught
				return {true, (*pFunc)(p)};
			} catch (const std::exception& e) {
				MessageBoxA(nullptr, e.what(), "Oops... an exception was thrown", MB_ICONERROR);
			}
		}
		return {false, -1}; // message not processed
	}

public:
	void on_message(UINT msg, funcT func)                                  { this->_msgs.add(msg, std::move(func)); }
	void on_message(std::initializer_list<UINT> msgs, funcT func)          { this->_msgs.add(msgs, std::move(func)); }
	void on_command(WORD cmd, funcT func)                                  { this->_cmds.add(cmd, std::move(func)); }
	void on_command(std::initializer_list<WORD> cmds, funcT func)          { this->_cmds.add(cmds, std::move(func)); }
	void on_notify(UINT_PTR idFrom, UINT code, funcT func)                 { this->_ntfs.add({idFrom, code}, std::move(func)); }
	void on_notify(ntfT idFromAndCode, funcT func)                         { this->_ntfs.add(idFromAndCode, std::move(func)); }
	void on_notify(std::initializer_list<ntfT> idFromAndCodes, funcT func) { this->_ntfs.add(idFromAndCodes, std::move(func)); }
};

}//namespace wli
}//namespace wl
