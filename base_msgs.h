/**
 * Part of WinLamb - Win32 API Lambda Library
 * @author Rodrigo Cesar de Freitas Dias
 * @see https://github.com/rodrigocfd/winlamb
 */

#pragma once
#include "base_inventory.h"

/**
 * base_wnd <-- base_inventory <-- base_msgs
 */

namespace wl {
namespace base {

	class msgs : virtual public inventory {
	protected:
		msgs() = default;

		LONG_PTR _proc_handled(params& p) {
			return this->inventory::_procHandled(p);
		}

		LONG_PTR _proc_unhandled(params& p) {
			return this->inventory::_procUnhandled(p);
		}
	};

}//namespace base
}//namespace wl