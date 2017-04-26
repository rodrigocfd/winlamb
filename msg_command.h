/**
 * Part of WinLamb - Win32 API Lambda Library
 * @author Rodrigo Cesar de Freitas Dias
 * @see https://github.com/rodrigocfd/winlamb
 */

#pragma once
#include "base_msgs.h"

/**
 * base_wnd <-- base_inventory <-- base_msgs <-- msg_command
 */

namespace wl {

// Adds on_command() method to handle WM_COMMAND messages.
class msg_command : private base::msgs {
public:
	using command_funcT = base::depot<WORD>::funcT;
private:
	base::depot<WORD> _cmdDepot;

protected:
	explicit msg_command(size_t msgsReserve = 0) : _cmdDepot(msgsReserve) {
		this->on_message(WM_COMMAND, [&](params& p)->LONG_PTR {
			command_funcT* pFunc = this->_cmdDepot.find(LOWORD(p.wParam));
			return pFunc ? (*pFunc)(p) : this->msgs::_proc_unhandled(p);
		});
	}

	void on_command(WORD cmd, command_funcT func) {
		this->_cmdDepot.add(cmd, std::move(func));
	}

	void on_command(std::initializer_list<WORD> cmds, command_funcT func) {
		this->_cmdDepot.add(cmds, std::move(func));
	}
};

}//namespace wl