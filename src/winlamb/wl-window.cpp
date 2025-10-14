#include <system_error>
#include <thread>
#include "window.h"
#include "window-user.h"
#include "runnable.h"
#include <CommCtrl.h>
using namespace _wl_internal;
using namespace wl;

std::wstring Window::text() const {
	UINT len = GetWindowTextLengthW(hwnd());
	if (!len) {
		DWORD err = GetLastError();
		if (err != ERROR_SUCCESS) [[unlikely]] {
			throw std::system_error(err, std::system_category(), "GetWindowTextLength failed.");
		}
		return std::wstring{}; // actual empty string
	}

	std::wstring buf(len + 1, L'\0'); // alloc receiving buffer
	GetWindowTextW(hwnd(), buf.data(), len + 1);
	buf.resize(len);
	return buf;
}

void Window::set_text(WStrPtr text) const {
	if (!SetWindowTextW(hwnd(), text)) [[unlikely]] {
		throw std::system_error(GetLastError(), std::system_category(), "SetWindowText failed.");
	}
}

////////////////////////////////////////////////////////////////////////////////

EventsUser& WindowMsg::on() {
	#ifdef _DEBUG
	if (hwnd()) [[unlikely]] {
		throw std::logic_error("Cannot add events after the window is created.");
	}
	#endif
	return _userEvents;
}

struct ThreadPack final {
	std::optional<std::function<void()>> cb = std::nullopt;
	std::optional<std::exception> excep = std::nullopt;
};
static constexpr UINT WM_THREAD = WM_APP + 0x3fff; // last WM_APP value

void WindowMsg::thread_detach(std::function<void()> cb) const {
	std::thread([cb = std::move(cb), this]() {
		try {
			cb();
		} catch (const std::exception &e) {
			auto pPack = std::make_unique<ThreadPack>(std::nullopt, e);
			SendMessageW(hwnd(), WM_THREAD, WM_THREAD, reinterpret_cast<LPARAM>(pPack.release()));
		}
	}).detach();
}

void WindowMsg::thread_ui(std::function<void()> cb) const {
	auto pPack = std::make_unique<ThreadPack>(std::move(cb), std::nullopt);
	SendMessageW(hwnd(), WM_THREAD, WM_THREAD, reinterpret_cast<LPARAM>(pPack.release()));
}

WindowMsg::ProcResult WindowMsg::process_msgs(UINT msg, WPARAM wp, LPARAM lp) {
	if (msg == WM_THREAD && wp == WM_THREAD) {
		std::unique_ptr<ThreadPack> pPack{reinterpret_cast<ThreadPack*>(lp)};
		if (pPack->excep.has_value()) { // catching an exception from another thread
			uncaught_exception(pPack->excep.value());
		} else if (pPack->cb.has_value()) { // running a thread UI callback
			try {
				pPack->cb.value()();
			} catch (const std::exception &e) {
				uncaught_exception(e);
			}
		}
		return {true, false, std::nullopt};
	}

	bool hasPre = _preEvents.process_all({msg, wp, lp});
	std::optional<LRESULT> userRet = _userEvents.process_last({msg, wp, lp});
	bool hasPost = _postEvents.process_all({msg, wp, lp});

	switch (msg) {
	case WM_CREATE:
	case WM_INITDIALOG:
		_preEvents.clear_inis(); // since initial messages will never be called again, release
		_userEvents.clear_inis();
		_postEvents.clear_inis();
		break;
	case WM_NCDESTROY:
		_preEvents.clear();
		_userEvents.clear();
		_postEvents.clear();
	}

	return {hasPre, hasPost, userRet};
}

int WindowMsg::main_loop(HACCEL hAccel) {
	MSG msg{};
	BOOL ret = FALSE;
	while ((ret = GetMessageW(&msg, nullptr, 0, 0)) != 0) {
		if (ret == -1) [[unlikely]] {
			throw std::system_error(GetLastError(), std::system_category(), "GetMessage failed.");
		}
		if (hAccel && TranslateAcceleratorW(hwnd(), hAccel, &msg)) continue;
		if (IsDialogMessageW(hwnd(), &msg)) continue;
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return static_cast<int>(msg.wParam); // can be used as program return value
}

void WindowMsg::modal_loop() {
	MSG msg{};
	BOOL ret = FALSE;
	while ((ret = GetMessageW(&msg, nullptr, 0, 0)) != 0) {
		if (ret == -1) [[unlikely]] {
			throw std::system_error(GetLastError(), std::system_category(), "GetMessage failed.");
		}
		if (!hwnd() || !IsWindow(hwnd())) break; // our modal was destroyed

		// If a child window, will retrieve its top-level parent.
		// If a top-level, use itself.
		HWND hWndTopLevel = GetAncestor(hwnd(), GA_ROOT);
		if (!hWndTopLevel) hWndTopLevel = hwnd();

		// Try to process keyboard actions for child controls.
		if (IsDialogMessageW(hwnd(), &msg)) {
			// Processed all keyboard actions for child controls.
			if (!hwnd()) break; // our modal was destroyed
			else continue;
		}

		TranslateMessage(&msg);
		DispatchMessageW(&msg);

		if (!hwnd() || !IsWindow(hwnd())) break; // our modal was destroyed
	}
}

////////////////////////////////////////////////////////////////////////////////

NativeCtrl::NativeCtrl(WindowMain &owner)
	: _owner{owner.wnd_msg()}
{
}

NativeCtrl::NativeCtrl(WindowModal &owner)
	: _owner{owner.wnd_msg()}
{
}

UINT_PTR NativeCtrl::_subclassId = 0;

EventsUser& NativeCtrl::subclass_on() {
	#ifdef _DEBUG
	if (hwnd()) [[unlikely]] {
		throw std::logic_error("Cannot add subclass events after the control is created.");
	}
	#endif
	return _subclassEvents;
}

void NativeCtrl::set_hwnd(HWND hWnd) {
	_wnd._hWnd = hWnd;
}

void NativeCtrl::install_subclass() {
	if (_subclassEvents.has_message()) {
		_subclassId++;
		BOOL ret = SetWindowSubclass(hwnd(), subclass_proc, _subclassId, reinterpret_cast<DWORD_PTR>(this));
		#ifdef _DEBUG
		if (!ret) [[unlikely]] {
			throw std::runtime_error("SetWindowSubclass failed.");
		}
		#endif
	}
}

LRESULT CALLBACK NativeCtrl::subclass_proc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp,
	UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	NativeCtrl *pSelf = reinterpret_cast<NativeCtrl*>(dwRefData);

	std::optional<LRESULT> ret;
	if (pSelf) {
		ret = pSelf->_subclassEvents.process_last({msg, wp, lp});
	}

	if (msg == WM_NCDESTROY) { // always check
		// https://devblogs.microsoft.com/oldnewthing/20031111-00/?p=41883
		RemoveWindowSubclass(hWnd, subclass_proc, uIdSubclass);
		if (pSelf) pSelf->_subclassEvents.clear();
	}

	return ret.has_value() ? ret.value() : DefSubclassProc(hWnd, msg, wp, lp);
}
