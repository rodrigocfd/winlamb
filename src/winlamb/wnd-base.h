#pragma once
#include <functional>
#include <optional>
#include <string>
#include "lib-include-win.h"
#include "wnd-interfaces.h"
#include "events-wnd.h"
#include "layout.h"

namespace _wl_internal {

	/// Base to all raw and dialog windows.
	/// Stores the pre, user and post window messages for container windows.
	/// Exposes exception-safe multi-threading operations.
	class WndBase final {
	public:
		WndBase(WndBase&&) = delete; // non-copyable, non-movable

		constexpr explicit WndBase(bool isDlg)
			: _isDlg{isDlg}, _preEvents{isDlg}, _userEvents{isDlg}, _postEvents{isDlg} { }

		void ui_thread(std::function<void()> &&cb) const;

		struct ProcResult final {
			bool hasPre, hasPost;
			std::optional<LRESULT> userRet;
		};
		ProcResult process_msgs(UINT msg, WPARAM wp, LPARAM lp);

		int main_loop(HACCEL hAccel, bool processDlgMsgs);
		void modal_loop(bool processDlgMsgs);

		bool _isDlg;
		HWND _hWnd = nullptr; // _hWnd member is set in wndproc
		Layout _layout{};
		InternalEvents _preEvents;
		wl::events::WindowEvents _userEvents;
		InternalEvents _postEvents;
	};

	/// Base to all native controls.
	/// Stores the subclass messages.
	class NativeCtrlBase final {
	public:
		NativeCtrlBase(NativeCtrlBase&&) = delete; // non-copyable, non-movable

		explicit NativeCtrlBase(wl::WindowParent &owner);

		void create_wnd(WORD ctrlId, DWORD exStyle, const wchar_t *className,
			std::wstring &&title, DWORD style, POINT pos, SIZE size);
		void assign_dlg(WORD ctrlId);
		void install_subclass();

		static LRESULT CALLBACK subclass_proc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp,
			UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

		HWND _hWnd = nullptr; // _hWnd member is set during control creation
		WndBase &_parentWndBase;
		wl::events::WindowEvents _subclassEvents{false};
	};

}
