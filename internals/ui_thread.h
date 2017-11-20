/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "inventory.h"

/**
 * hwnd_wrapper
 *  inventory
 *   ui_thread
 */

namespace wl {
namespace wli {

template<typename retT, retT RET_VAL>
class ui_thread : public inventory<retT> {
private:
	struct _callback_pack final {
		std::function<void()> func;
	};

	static const UINT WM_THREAD_MESSAGE = WM_APP + 0x3FFF;

protected:
	ui_thread() {
		this->on_message(WM_THREAD_MESSAGE, [this](params p)->retT {
			this->_process_threaded(p);
			return RET_VAL; // 0 for windows, TRUE for dialogs
		});
	}

	void run_ui_thread(std::function<void()> func) const {
		// This method is analog to SendMessage (synchronous), but intended to be called from another
		// thread, so a callback function can, tunelled by wndproc, run in the original thread of the
		// window, thus allowing GUI updates. This avoids the user to deal with a custom WM_ message.
		_callback_pack* pack = new _callback_pack{std::move(func)};
		SendMessageW(this->hwnd(), WM_THREAD_MESSAGE, 0, reinterpret_cast<LPARAM>(pack));
	}

private:
	void _process_threaded(params p) const {
		_callback_pack* pack = reinterpret_cast<_callback_pack*>(p.lParam);
		pack->func();
		delete pack;
	}
};

}//namespace wli
}//namespace wl
