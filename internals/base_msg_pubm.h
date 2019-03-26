/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "base_msg.h"

namespace wl {
namespace wli {

// Provides public methods for base_msg class.
template<typename retT>
class base_msg_pubm {
private:
	base_msg<retT>& _baseMsg;

public:
	base_msg_pubm(base_msg<retT>& baseMsg) noexcept :
		_baseMsg(baseMsg) { }

	// Assigns a lambda to handle a window message.
	void on_message(UINT msg, std::function<retT(params)>&& func) {
		this->_baseMsg.throw_if_cant_add();
		this->_baseMsg.msgs.add(msg, std::move(func));
	}
	// Assigns a lambda to handle a window message.
	void on_message(std::initializer_list<UINT> msgs,
		std::function<retT(params)>&& func)
	{
		this->_baseMsg.throw_if_cant_add();
		this->_baseMsg.msgs.add(msgs, std::move(func));
	}

	// Assigns a lambda to handle a WM_COMMAND message.
	void on_command(WORD cmd, std::function<retT(params)>&& func) {
		this->_baseMsg.throw_if_cant_add();
		this->_baseMsg.cmds.add(cmd, std::move(func));
	}
	// Assigns a lambda to handle a WM_COMMAND message.
	void on_command(std::initializer_list<WORD> cmds,
		std::function<retT(params)>&& func)
	{
		this->_baseMsg.throw_if_cant_add();
		this->_baseMsg.cmds.add(cmds, std::move(func));
	}

	// Assigns a lambda to handle a WM_NOTIFY message.
	void on_notify(UINT_PTR idFrom, UINT code,
		std::function<retT(params)>&& func)
	{
		this->_baseMsg.throw_if_cant_add();
		this->_baseMsg.ntfs.add({idFrom, code}, std::move(func));
	}
	// Assigns a lambda to handle a WM_NOTIFY message.
	void on_notify(std::pair<UINT_PTR, UINT> idFromAndCode,
		std::function<retT(params)>&& func)
	{
		this->_baseMsg.throw_if_cant_add();
		this->_baseMsg.ntfs.add(idFromAndCode, std::move(func));
	}
	// Assigns a lambda to handle a WM_NOTIFY message.
	void on_notify(
		std::initializer_list<std::pair<UINT_PTR, UINT>> idFromAndCodes,
		std::function<retT(params)>&& func)
	{
		this->_baseMsg.throw_if_cant_add();
		this->_baseMsg.ntfs.add(idFromAndCodes, std::move(func));
	}
};

}//namespace wli
}//namespace wl
