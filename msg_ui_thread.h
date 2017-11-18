/**
 * Part of WinLamb - Win32 API Lambda Library
 * @author Rodrigo Cesar de Freitas Dias
 * @see https://github.com/rodrigocfd/winlamb
 */

#pragma once
#include "base_msgs.h"

/**
 * base_wnd <-- base_inventory <-- base_msgs <-- msg_thread
 */

namespace wl {

// Adds run_ui_thread() method to execute code in original UI thread.
class msg_ui_thread : private base::msgs {
public:
	using ui_thread_funcT = std::function<void()>;
private:
	struct _callback_pack final {
		ui_thread_funcT func;
	};

	static const UINT WM_THREAD_MESSAGE = WM_APP + 0x3FFF;

protected:
	msg_ui_thread() {
		this->on_message(WM_THREAD_MESSAGE, [&](params p)->LONG_PTR {
			this->_process_threaded(p);
			return this->msgs::_proc_handled(p);
		});
	}

	void run_ui_thread(ui_thread_funcT func) const {
		// This method is analog to SendMessage (synchronous), but intended to be called from another
		// thread, so a callback function can, tunelled by wndproc, run in the original thread of the
		// window, thus allowing GUI updates. This avoids the user to deal with a custom WM_ message.
		_callback_pack* pack = new _callback_pack{ std::move(func) };
		SendMessageW(this->hwnd(), WM_THREAD_MESSAGE, 0, reinterpret_cast<LPARAM>(pack));
	}

private:
	void _process_threaded(const params& p) const {
		_callback_pack* pack = reinterpret_cast<_callback_pack*>(p.lParam);
		pack->func();
		delete pack;
	}
};

}//namespace wl