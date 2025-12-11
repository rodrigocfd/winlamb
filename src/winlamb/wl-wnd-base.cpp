#include <memory>
#include <system_error>
#include "wnd-base.h"
#include "wnd-interfaces.h"
#include "wnd-app.h"
#include <CommCtrl.h>
using namespace wl;
using namespace _wl_internal;

struct ThreadPack final {
	std::function<void()> cb;
};
static constexpr UINT WM_THREAD = WM_APP + 0x3fff; // last WM_APP value

void WndBase::ui_thread(std::function<void()> &&cb) const {
	auto pPack = std::make_unique<ThreadPack>(std::move(cb));
	SendMessageW(_hWnd, WM_THREAD, WM_THREAD, reinterpret_cast<LPARAM>(pPack.release()));
}

WndBase::ProcResult WndBase::process_msgs(UINT msg, WPARAM wp, LPARAM lp) {
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

int WndBase::main_loop(HACCEL hAccel, bool processDlgMsgs) {
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
		HWND hWndTopLevel = GetAncestor(_hWnd, GA_ROOT);
		if (!hWndTopLevel) hWndTopLevel = msg.hwnd;

		// If we have an accelerator table, try to translate the message.
		if (hAccel && TranslateAcceleratorW(_hWnd, hAccel, &msg)) continue;

		// Try to process keyboard actions for child controls.
		if (processDlgMsgs && IsDialogMessageW(_hWnd, &msg)) continue;

		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return static_cast<int>(msg.wParam); // can be used as program return value
}

void WndBase::modal_loop(bool processDlgMsgs) {
	MSG msg{};
	for (;;) {
		if (BOOL ret = GetMessageW(&msg, nullptr, 0, 0); ret == -1) [[unlikely]] {
			throw std::system_error(GetLastError(), std::system_category(), "Modal loop: GetMessage failed");
		} else if (!ret) {
			break; // our modal was destroyed
		}

		if (!_hWnd || !IsWindow(_hWnd)) break; // our modal was destroyed

		// If a child window, will retrieve its top-level parent.
		// If a top-level, use itself.
		HWND hWndTopLevel = GetAncestor(_hWnd, GA_ROOT);
		if (!hWndTopLevel) hWndTopLevel = _hWnd;

		// Try to process keyboard actions for child controls.
		if (processDlgMsgs && IsDialogMessageW(_hWnd, &msg)) {
			// Processed all keyboard actions for child controls.
			if (!_hWnd) break; // our modal was destroyed
			else continue;
		}

		TranslateMessage(&msg);
		DispatchMessageW(&msg);

		if (!_hWnd || !IsWindow(_hWnd)) break; // our modal was destroyed
	}
}

////////////////////////////////////////////////////////////////////////////////

NativeCtrlBase::NativeCtrlBase(WindowParent &owner)
	: _parentWndBase{owner.wnd_base()}
{
}

void NativeCtrlBase::create_wnd(WORD ctrlId, DWORD exStyle, const wchar_t *className,
	std::wstring &&title, DWORD style, POINT pos, SIZE size)
{
	#ifdef _DEBUG
	if (_hWnd)
		throw std::logic_error("Cannot create control twice.");
	if (!_parentWndBase._hWnd)
		throw std::logic_error("Cannot create control before parent.");
	#endif

	_hWnd = CreateWindowExW(exStyle, className, title.c_str(), style,
		pos.x, pos.y, size.cx, size.cy, _parentWndBase._hWnd,
		reinterpret_cast<HMENU>(valid_ctrl_id(ctrlId)), wnd_hinst(_parentWndBase._hWnd), nullptr);
	#ifdef _DEBUG
	if (!_hWnd)
		throw std::system_error(GetLastError(), std::system_category(), "NativeCtrlBase: CreateWindowEx failed");
	#endif

	install_subclass();
}

void NativeCtrlBase::assign_dlg(WORD ctrlId) {
	#ifdef _DEBUG
	if (_hWnd)
		throw std::logic_error("Cannot assign control twice.");
	if (!_parentWndBase._hWnd)
		throw std::logic_error("Cannot assign control before parent.");
	#endif

	_hWnd = GetDlgItem(_parentWndBase._hWnd, ctrlId);
	#ifdef _DEBUG
	if (!_hWnd)
		throw std::system_error(GetLastError(), std::system_category(), "NativeCtrlBase: GetDlgItem failed");
	#endif

	install_subclass();
}

void NativeCtrlBase::install_subclass() {
	static UINT_PTR subclassId = 0;
	if (_subclassEvents.has_message()) {
		BOOL ret = SetWindowSubclass(_hWnd, subclass_proc, ++subclassId, reinterpret_cast<DWORD_PTR>(this));
		#ifdef _DEBUG
		if (!ret)
			throw std::runtime_error("SetWindowSubclass failed.");
		#endif
	}
}

LRESULT CALLBACK NativeCtrlBase::subclass_proc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp,
	UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	NativeCtrlBase *pSelf = reinterpret_cast<NativeCtrlBase*>(dwRefData);

	std::optional<LRESULT> ret{};
	if (pSelf)
		ret = pSelf->_subclassEvents.process_last({msg, wp, lp});

	if (msg == WM_NCDESTROY) { // always check
		// https://devblogs.microsoft.com/oldnewthing/20031111-00/?p=41883
		BOOL ret = RemoveWindowSubclass(hWnd, subclass_proc, uIdSubclass);
		#ifdef _DEBUG
		if (!ret)
			throw std::runtime_error("RemoveWindowSubclass failed.");
		#endif
		if (pSelf)
			pSelf->_subclassEvents.clear();
	}

	return ret.has_value() ? ret.value() : DefSubclassProc(hWnd, msg, wp, lp);
}
