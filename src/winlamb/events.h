#pragma once
#include <functional>
#include <optional>
#include <vector>
#include <Windows.h>

// Window message crackers.
namespace wl::wm {

	// Raw data of an ordinary window message.
	struct Msg {
		UINT wm;
		WPARAM wp;
		LPARAM lp;
	};

	struct Command : public Msg {
		constexpr Command(const Msg &p) : Msg{p} { }
		[[nodiscard]] constexpr bool is_from_menu() const        { return HIWORD(wp) == 0; }
		[[nodiscard]] constexpr bool is_from_accelerator() const { return HIWORD(wp) == 1; }
		[[nodiscard]] constexpr bool is_from_control() const     { return !is_from_menu() && !is_from_accelerator(); }
		[[nodiscard]] constexpr WORD menu_id() const             { return control_id(); }
		[[nodiscard]] constexpr WORD accelerator_id() const      { return control_id(); }
		[[nodiscard]] constexpr WORD control_id() const          { return LOWORD(wp); }
		[[nodiscard]] constexpr WORD control_notif_code() const  { return HIWORD(wp); }
		[[nodiscard]] HWND           control_hwnd() const        { return reinterpret_cast<HWND>(lp); }
	};

	struct Notify : public Msg {
		constexpr Notify(const Msg &p) : Msg{p} { }
		template<typename T> [[nodiscard]] T& hdr() { return *reinterpret_cast<T*>(lp); }
	};

	struct Activate : public Msg {
		constexpr Activate(const Msg &p) : Msg{p} { }
		[[nodiscard]] constexpr WORD active_state() const   { return LOWORD(wp); }
		[[nodiscard]] constexpr bool is_minimized() const   { return HIWORD(wp) != 0; }
		[[nodiscard]] HWND           swapped_window() const { return reinterpret_cast<HWND>(lp); }
	};

	struct Create : public Msg {
		constexpr Create(const Msg &p) : Msg{p} { }
		[[nodiscard]] CREATESTRUCTW& crate_struct() const { return *reinterpret_cast<CREATESTRUCTW*>(lp); }
	};

	struct Enable : public Msg {
		constexpr Enable(const Msg &p) : Msg{p} { }
		[[nodiscard]] constexpr bool was_enabled() const { return wp != FALSE; }
	};

	struct EndSession : public Msg {
		constexpr EndSession(const Msg &p) : Msg{p} { }
		[[nodiscard]] constexpr bool is_session_being_ended() const { return wp != FALSE; }
		[[nodiscard]] constexpr bool is_system_issue() const        { return (lp & ENDSESSION_CLOSEAPP) != 0; }
		[[nodiscard]] constexpr bool is_forced_critical() const     { return (lp & ENDSESSION_CRITICAL) != 0; }
		[[nodiscard]] constexpr bool is_logoff() const              { return (lp & ENDSESSION_LOGOFF) != 0; }
		[[nodiscard]] constexpr bool is_shutdown() const            { return lp == 0; }
	};

	struct EraseBkgnd : public Msg {
		constexpr EraseBkgnd(const Msg &p) : Msg{p} { }
		[[nodiscard]] HDC hdc() const { return reinterpret_cast<HDC>(wp); }
	};

	struct GetDlgCode : public Msg {
		constexpr GetDlgCode(const Msg &p) : Msg{p} { }
		[[nodiscard]] constexpr BYTE vkey_code() const { return static_cast<BYTE>(wm); }
		[[nodiscard]] constexpr bool is_query() const  { return lp == 0; }
		[[nodiscard]] MSG*           msg() const       { return this->is_query() ? nullptr : reinterpret_cast<MSG*>(lp); }
		[[nodiscard]] bool           has_alt() const   { return (GetAsyncKeyState(VK_MENU) & 0x8000) != 0; }
		[[nodiscard]] bool           has_ctrl() const  { return (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0; }
		[[nodiscard]] bool           has_shift() const { return (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0; }
	};

	struct InitDialog : public Msg {
		constexpr InitDialog(const Msg &p) : Msg{p} { }
		[[nodiscard]] HWND hwnd_focus() const { return reinterpret_cast<HWND>(wp); }
	};

	struct InitMenuPopup : public Msg {
		constexpr InitMenuPopup(const Msg &p) : Msg{p} { }
		[[nodiscard]] HMENU           hmenu() const              { return reinterpret_cast<HMENU>(wp); }
		[[nodiscard]] constexpr short relative_pos() const       { return LOWORD(lp); }
		[[nodiscard]] constexpr bool  is_window_menu() const     { return HIWORD(lp) != FALSE; }
		[[nodiscard]] UINT            first_menu_item_id() const { return GetMenuItemID(hmenu(), 0); }
	};

	struct KillFocus : public Msg {
		constexpr KillFocus(const Msg &p) : Msg{p} { }
		HWND focused_hwnd() const { return reinterpret_cast<HWND>(wp); }
	};

	struct NcPaint : public Msg {
		constexpr NcPaint(const Msg &p) : Msg{p} { }
		[[nodiscard]] HRGN hrgn() const { return reinterpret_cast<HRGN>(wp); }
	};

	struct SetFocus : public Msg {
		constexpr SetFocus(const Msg &p) : Msg{p} { }
		[[nodiscard]] HWND hwnd_losing_focus() const { return reinterpret_cast<HWND>(wp); }
	};

	struct Size : public Msg {
		constexpr Size(const Msg &p) : Msg{p} { }
		[[nodiscard]] constexpr bool is_other_maximized() const { return wp == 4; }
		[[nodiscard]] constexpr bool is_maximized() const       { return wp == 2; }
		[[nodiscard]] constexpr bool is_other_restored() const  { return wp == 3; }
		[[nodiscard]] constexpr bool is_minimized() const       { return wp == 1; }
		[[nodiscard]] constexpr bool is_restored() const        { return wp == 0; }
		[[nodiscard]] constexpr SIZE sz() const                 { return {LOWORD(lp), HIWORD(lp)}; }
	};

	struct Sizing : public Msg {
		constexpr Sizing(const Msg &p) : Msg{p} { }
		[[nodiscard]] constexpr WORD edge() const          { return static_cast<WORD>(wp); }
		[[nodiscard]] RECT&          screen_coords() const { return *reinterpret_cast<RECT*>(lp); }
	};

}

namespace _wl_internal {

	class WindowMsg;
	class NativeCtrl;

	// Stores library-internal message identifiers and callbacks.
	class EventsInternal final {
	public:
		struct Msg final {
			UINT wm;
			std::function<void(wl::wm::Msg)> cb;
		};
		struct Nfy final {
			WORD idFrom;
			int code;
			std::function<void(wl::wm::Notify)> cb;
		};

		constexpr EventsInternal() = delete;
		constexpr EventsInternal(const EventsInternal&) = delete;
		constexpr EventsInternal(EventsInternal&&) = delete;
		constexpr EventsInternal& operator=(const EventsInternal&) = delete;
		constexpr EventsInternal& operator=(EventsInternal&&) = delete;

		constexpr explicit EventsInternal(bool isDlg) : _isDlg{isDlg} { }

		void wm_create_or_init_dialog(std::function<void()> cb);
		void wm(UINT msg, std::function<void(wl::wm::Msg)> cb);
		void wm_notify(WORD idFrom, int code, std::function<void(wl::wm::Notify)> cb);

		void clear_inis(); // WM_CREATE and WM_INITDIALOG
		void clear();
		bool process_all(wl::wm::Msg procMsg) const;

		bool _isDlg;
		std::vector<std::function<void()>> _inis{}; // WM_CREATE, WM_INITDIALOG
		std::vector<Msg> _msgs{};
		std::vector<Nfy> _nfys{}; // WM_NOTIFY
	};

}

namespace _wl_internal {

	// Stores user message identifiers and callbacks.
	class EventsUser final {
	private:
		struct Msg final {
			UINT wm;
			std::function<LRESULT(wl::wm::Msg)> cb;
		};
		struct Cmd final {
			WORD cmdId;
			WORD notifCode;
			std::function<void()> cb;
		};
		struct Nfy final {
			WORD idFrom;
			int code;
			std::function<LRESULT(wl::wm::Notify)> cb;
		};

	public:
		constexpr EventsUser() = delete;
		constexpr EventsUser(const EventsUser&) = delete;
		constexpr EventsUser(EventsUser&&) = delete;
		constexpr EventsUser& operator=(const EventsUser&) = delete;
		constexpr EventsUser& operator=(EventsUser&&) = delete;

	private:
		constexpr explicit EventsUser(bool isDlg) : _isDlg{isDlg} { }

	public:
		void wm_create(std::function<int(wl::wm::Create)> cb);
		void wm_init_dialog(std::function<bool(wl::wm::InitDialog)> cb);

		// Adds a callback to an ordinary WM message.
		void wm(UINT msg, std::function<LRESULT(wl::wm::Msg)> cb);

		// Adds a callback to a WM_COMMAND message, to an specific notification code.
		void wm_command(WORD cmdId, WORD notifCode, std::function<void()> cb);

		// Adds a callback to a WM_COMMAND message, for notifications from
		// both accelerator and menu events. This is the most common case.
		void wm_command(WORD cmdId, std::function<void()> cb);

		// Adds a callback to a WM_NOTIFY message, to specific control ID and notification code.
		void wm_notify(WORD idFrom, int code, std::function<LRESULT(wl::wm::Notify)> cb);

		void wm_activate(std::function<void(wl::wm::Activate)> cb);
		void wm_close(std::function<void()> cb);
		void wm_destroy(std::function<void()> cb);
		void wm_enable(std::function<void(wl::wm::Enable)> cb);
		void wm_end_session(std::function<void(wl::wm::EndSession)> cb);
		void wm_erase_bkgnd(std::function<int(wl::wm::EraseBkgnd)> cb);
		void wm_set_focus(std::function<void(wl::wm::SetFocus)> cb);
		void wm_get_dlg_code(std::function<WORD(wl::wm::GetDlgCode)> cb);
		void wm_init_menu_popup(std::function<void(wl::wm::InitMenuPopup)> cb);
		void wm_kill_focus(std::function<void(wl::wm::KillFocus)> cb);
		void wm_nc_destroy(std::function<void()> cb);
		void wm_nc_paint(std::function<void(wl::wm::NcPaint)> cb);
		void wm_paint(std::function<void()> cb);
		void wm_size(std::function<void(wl::wm::Size)> cb);
		void wm_sizing(std::function<void(wl::wm::Sizing)> cb);

	private:
		[[nodiscard]] bool has_message() const;
		void clear_inis(); // WM_CREATE and WM_INITDIALOG
		void clear();
		std::optional<LRESULT> process_last(wl::wm::Msg procMsg) const;

		bool _isDlg;
		std::vector<Msg> _inis{}; // WM_CREATE, WM_INITDIALOG
		std::vector<Msg> _msgs{};
		std::vector<Cmd> _cmds{}; // WM_COMMAND
		std::vector<Nfy> _nfys{}; // WM_NOTIFY

		friend WindowMsg; // message processing
		friend NativeCtrl; // subclass processing
	};

}

namespace wl {
	class WindowMain;
	class WindowModal;
}

namespace _wl_internal {

	// Base to all native control events.
	class EventsNativeCtrl final {
	public:
		EventsNativeCtrl() = delete;
		EventsNativeCtrl(const EventsNativeCtrl&) = delete;
		EventsNativeCtrl(EventsNativeCtrl&&) = delete;
		EventsNativeCtrl& operator=(const EventsNativeCtrl&) = delete;
		EventsNativeCtrl& operator=(EventsNativeCtrl&&) = delete;

		EventsNativeCtrl(wl::WindowMain &owner, WORD ctrlId);
		EventsNativeCtrl(wl::WindowModal &owner, WORD ctrlId);

		WindowMsg &_owner;
		WORD _ctrlId;
	};

}
