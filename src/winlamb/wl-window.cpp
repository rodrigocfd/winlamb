#include <system_error>
#include <thread>
#include "window.h"
#include "window-user.h"
#include "runnable.h"
#include <CommCtrl.h>
using namespace _wl_internal;
using namespace wl;
using namespace wl::events;

std::wstring Window::text() const {
	UINT len = GetWindowTextLengthW(hwnd());
	if (!len) {
		DWORD err = GetLastError();
		if (err != ERROR_SUCCESS) [[unlikely]] {
			throw std::system_error(err, std::system_category(), "GetWindowTextLength failed");
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
		throw std::system_error(GetLastError(), std::system_category(), "SetWindowText failed");
	}
}

////////////////////////////////////////////////////////////////////////////////

WindowEvents& WindowMsg::on() {
	#ifdef _DEBUG
	if (hwnd())
		throw std::logic_error("Cannot add events after the window is created.");
	#endif
	return _userEvents;
}

struct ThreadPack final {
	std::function<void()> cb;
};
static constexpr UINT WM_THREAD = WM_APP + 0x3fff; // last WM_APP value

void WindowMsg::ui_thread(std::function<void()> cb) const {
	auto pPack = std::make_unique<ThreadPack>(std::move(cb));
	SendMessageW(hwnd(), WM_THREAD, WM_THREAD, reinterpret_cast<LPARAM>(pPack.release()));
}

WindowMsg::ProcResult WindowMsg::process_msgs(UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case WM_THREAD:
		if (wp == WM_THREAD) { // additional safety check
			std::unique_ptr<ThreadPack> pPack{reinterpret_cast<ThreadPack*>(lp)};
			pPack->cb();
			return {true, false, std::nullopt};
		}
		break;
	case WM_SIZE:
		_layout.rearrange(wm::Msg{msg, wp, lp});
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

int WindowMsg::main_loop(HACCEL hAccel, bool processDlgMsgs) {
	MSG msg{};
	BOOL ret = FALSE;
	for (;;) {
		if (BOOL ret = GetMessageW(&msg, nullptr, 0, 0); ret == -1) [[unlikely]] {
			throw std::system_error(GetLastError(), std::system_category(), "Main loop: GetMessage failed");
		} else if (!ret) {
			// WM_QUIT was sent, gracefully terminate the program, wParam is the program exit code.
			// https://learn.microsoft.com/en-us/windows/win32/winmsg/using-messages-and-message-queues
			break;
		}

		// If a child window, will retrieve its top-level parent.
		// If a top-level, use itself.
		HWND hWndTopLevel = GetAncestor(hwnd(), GA_ROOT);
		if (!hWndTopLevel) hWndTopLevel = msg.hwnd;

		// If we have an accelerator table, try to translate the message.
		if (hAccel && TranslateAcceleratorW(hwnd(), hAccel, &msg)) continue;

		// Try to process keyboard actions for child controls.
		if (processDlgMsgs && IsDialogMessageW(hwnd(), &msg)) continue;

		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return static_cast<int>(msg.wParam); // can be used as program return value
}

void WindowMsg::modal_loop(bool processDlgMsgs) {
	MSG msg{};
	for (;;) {
		if (BOOL ret = GetMessageW(&msg, nullptr, 0, 0); ret == -1) [[unlikely]] {
			throw std::system_error(GetLastError(), std::system_category(), "Modal loop: GetMessage failed");
		} else if (!ret) {
			break; // our modal was destroyed
		}

		if (!hwnd() || !IsWindow(hwnd())) break; // our modal was destroyed

		// If a child window, will retrieve its top-level parent.
		// If a top-level, use itself.
		HWND hWndTopLevel = GetAncestor(hwnd(), GA_ROOT);
		if (!hWndTopLevel) hWndTopLevel = hwnd();

		// Try to process keyboard actions for child controls.
		if (processDlgMsgs && IsDialogMessageW(hwnd(), &msg)) {
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

NativeCtrl::NativeCtrl(WindowParent &owner)
	: _owner{owner.wnd_msg()}
{
}

UINT_PTR NativeCtrl::_subclassId = 0;

WindowEvents& NativeCtrl::subclass_on() {
	#ifdef _DEBUG
	if (hwnd())
		throw std::logic_error("Cannot add subclass events after the control is created.");
	#endif
	return _subclassEvents;
}

static WORD nextCtrId = 0xdfff; // https://stackoverflow.com/a/18192766/6923555

void NativeCtrl::create_wnd(WORD ctrlId, DWORD exStyle, LPCWSTR className,
	LPCWSTR title, DWORD style, POINT pos, SIZE size)
{
	#ifdef _DEBUG
	if (hwnd())
		throw std::logic_error("Cannot create control twice.");
	if (!_owner.hwnd())
		throw std::logic_error("Cannot create control before parent.");
	#endif

	_wnd._hWnd = CreateWindowExW(exStyle, className, title, style,
		pos.x, pos.y, size.cx, size.cy, _owner.hwnd(),
		reinterpret_cast<HMENU>(static_cast<UINT_PTR>(ctrlId ? ctrlId : nextCtrId--)),
		reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(_owner.hwnd(), GWLP_HINSTANCE)), nullptr);
	#ifdef _DEBUG
	if (!hwnd())
		throw std::system_error(GetLastError(), std::system_category(), "NativeCtrl: CreateWindowEx failed");
	#endif

	install_subclass();
}

void NativeCtrl::assign_dlg(WORD ctrlId) {
	#ifdef _DEBUG
	if (hwnd())
		throw std::logic_error("Cannot assign control twice.");
	if (!_owner.hwnd())
		throw std::logic_error("Cannot assign control before parent.");
	#endif

	_wnd._hWnd = GetDlgItem(_owner.hwnd(), ctrlId);
	#ifdef _DEBUG
	if (!hwnd())
		throw std::system_error(GetLastError(), std::system_category(), "NativeCtrl: GetDlgItem failed");
	#endif

	install_subclass();
}

void NativeCtrl::install_subclass() {
	if (_subclassEvents.has_message()) {
		_subclassId++;
		BOOL ret = SetWindowSubclass(hwnd(), subclass_proc, _subclassId, reinterpret_cast<DWORD_PTR>(this));
		#ifdef _DEBUG
		if (!ret)
			throw std::runtime_error("SetWindowSubclass failed.");
		#endif
	}
}

LRESULT CALLBACK NativeCtrl::subclass_proc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp,
	UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	NativeCtrl *pSelf = reinterpret_cast<NativeCtrl*>(dwRefData);

	std::optional<LRESULT> ret{};
	if (pSelf)
		ret = pSelf->_subclassEvents.process_last({msg, wp, lp});

	if (msg == WM_NCDESTROY) { // always check
		// https://devblogs.microsoft.com/oldnewthing/20031111-00/?p=41883
		RemoveWindowSubclass(hWnd, subclass_proc, uIdSubclass);
		if (pSelf)
			pSelf->_subclassEvents.clear();
	}

	return ret.has_value() ? ret.value() : DefSubclassProc(hWnd, msg, wp, lp);
}
