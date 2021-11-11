
#include <memory>
#include <stdexcept>
#include "base_parent.h"
#include "run_funcs.h"
#include <process.h>
using namespace _wli;
using std::function;
using std::optional;
using std::unique_ptr;

static const UINT WM_UI_THREAD = WM_APP + 0x3ff;

struct thread_pack {
	function<void()> fun;
	HWND hwnd;
	std::exception_ptr cur_except;
};

base_parent::~base_parent() { }

base_parent::base_parent(creation_type creation_ty)
	: _creation_type{creation_ty}
{
	_events.any(WM_UI_THREAD, [this](WPARAM, LPARAM lp) -> LRESULT {
		unique_ptr<thread_pack> pack{reinterpret_cast<thread_pack*>(lp)}; // take ownership
		if (pack->cur_except) { // an exception was thrown in run_detached_thread()
			try {
				std::rethrow_exception(pack->cur_except);
			} catch (...) {
				lippincott();
				PostQuitMessage(-1);
			}
		} else { // from run_ui_thread()
			try {
				pack->fun();
			} catch (...) {
				lippincott();
				PostQuitMessage(-1);
			}
		}
		return _creation_type == creation_type::raw ? 0 : TRUE;
	});

	_events.size([this](WORD request, SIZE client_area) {
		_resizer.resize(request, client_area);
	});
}

events& base_parent::on()
{
	if (_hwnd != nullptr) {
		throw std::logic_error("Cannot add events after window creation.");
	}
	return _events;
}

void base_parent::run_detached_thread(function<void()> fun) const
{
	unique_ptr<thread_pack> pack = std::make_unique<thread_pack>(std::move(fun), _hwnd, nullptr);

	uintptr_t hthread = _beginthreadex(nullptr, 0, [](void* ptr) -> unsigned int { // this is a detached thread now
		unique_ptr<thread_pack> pack{reinterpret_cast<thread_pack*>(ptr)}; // take ownership
		try {
			pack->fun();
		} catch (...) {
			// If an exception is caught, dispatch it to the UI thread.
			unique_ptr<thread_pack> crashed = std::make_unique<thread_pack>([]{}, pack->hwnd, std::current_exception());
			SendMessageW(pack->hwnd, WM_UI_THREAD, 0, reinterpret_cast<LPARAM>(crashed.release()));
		}
		_endthreadex(0); // http://www.codeproject.com/Articles/7732/A-class-to-synchronise-thread-completions/
		return 0;
	}, pack.release(), 0, nullptr);

	if (hthread) CloseHandle(reinterpret_cast<HANDLE>(hthread));
}

void base_parent::run_ui_thread(function<void()> fun) const
{
	// This method is analog to SendMessage (synchronous), but intended to be called
	// from another thread, so a callback function can, tunelled by wndproc, run in
	// the original thread of the window, thus allowing GUI updates. This avoids the
	// user to deal with a custom WM_ message.
	unique_ptr<thread_pack> pack = std::make_unique<thread_pack>(std::move(fun), _hwnd, nullptr);
	SendMessageW(_hwnd, WM_UI_THREAD, 0, reinterpret_cast<LPARAM>(pack.release()));
}
