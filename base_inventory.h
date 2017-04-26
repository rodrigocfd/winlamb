/**
 * Part of WinLamb - Win32 API Lambda Library
 * @author Rodrigo Cesar de Freitas Dias
 * @see https://github.com/rodrigocfd/winlamb
 */

#pragma once
#include "base_wnd.h"
#include "base_depot.h"

/**
 * base_wnd <-- base_inventory
 */

namespace wl {

class subclass;

namespace base {

	class dialog;
	class window;
	class user_control;
	class msgs;

	class inventory : virtual public wnd {
	public:
		friend subclass;
		friend dialog;
		friend window;
		friend user_control;
		friend msgs;

		using msg_funcT = depot<UINT>::funcT;
	private:
		msg_funcT _procHandled; // the return when message is processed, like "return 0"
		msg_funcT _procUnhandled; // the return when message isn't processed, like "return DefWindowProc()"
		depot<UINT> _msgDepot;

	protected:
		inventory() = default;

		void on_message(UINT msg, msg_funcT func) {
			this->_msgDepot.add(msg, std::move(func));
		}

		void on_message(std::initializer_list<UINT> msgs, msg_funcT func) {
			this->_msgDepot.add(msgs, std::move(func));
		}
	};

}//namespace base
}//namespace wl