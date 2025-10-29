#pragma once
#include <functional>
#include <optional>
#include <vector>
#include "lib-include-win.h"

/// @brief [Message] crackers, passed as arguments to the window events.
///
/// [Message]: https://learn.microsoft.com/en-us/windows/win32/learnwin32/window-messages
namespace wl::wm {

	/// @brief Raw data of an ordinary window message.
	///
	/// Convertible to the specific window message structs.
	struct Msg {
		UINT wm;
		WPARAM wp;
		LPARAM lp;
	};

	struct Command : protected Msg {
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

	struct Notify : protected Msg {
		constexpr Notify(const Msg &p) : Msg{p} { }
		template<typename T> [[nodiscard]] T& hdr() { return *reinterpret_cast<T*>(lp); }
	};

	struct Activate : protected Msg {
		constexpr Activate(const Msg &p) : Msg{p} { }
		[[nodiscard]] constexpr WORD active_state() const   { return LOWORD(wp); }
		[[nodiscard]] constexpr bool is_minimized() const   { return HIWORD(wp) != 0; }
		[[nodiscard]] HWND           swapped_window() const { return reinterpret_cast<HWND>(lp); }
	};

	struct ActivateApp : protected Msg {
		constexpr ActivateApp(const Msg &p) : Msg{p} { }
		[[nodiscard]] constexpr bool  is_being_discarded() const { return wp != 0; }
		[[nodiscard]] constexpr DWORD thread_id() const          { return static_cast<DWORD>(lp); }
	};

	struct Create : protected Msg {
		constexpr Create(const Msg &p) : Msg{p} { }
		[[nodiscard]] const CREATESTRUCTW& crate_struct() const { return *reinterpret_cast<const CREATESTRUCTW*>(lp); }
	};

	struct Enable : protected Msg {
		constexpr Enable(const Msg &p) : Msg{p} { }
		[[nodiscard]] constexpr bool was_enabled() const { return wp != FALSE; }
	};

	struct EndSession : protected Msg {
		constexpr EndSession(const Msg &p) : Msg{p} { }
		[[nodiscard]] constexpr DWORD endsession_flag() const        { return static_cast<DWORD>(lp); }
		[[nodiscard]] constexpr bool  is_session_being_ended() const { return wp != FALSE; }
		[[nodiscard]] constexpr bool  is_system_issue() const        { return (endsession_flag() & ENDSESSION_CLOSEAPP) != 0; }
		[[nodiscard]] constexpr bool  is_forced_critical() const     { return (endsession_flag() & ENDSESSION_CRITICAL) != 0; }
		[[nodiscard]] constexpr bool  is_logoff() const              { return (endsession_flag() & ENDSESSION_LOGOFF) != 0; }
		[[nodiscard]] constexpr bool  is_shutdown_restart() const    { return lp == 0; }
	};

	struct EraseBkgnd : protected Msg {
		constexpr EraseBkgnd(const Msg &p) : Msg{p} { }
		[[nodiscard]] HDC hdc() const { return reinterpret_cast<HDC>(wp); }
	};

	struct GetDlgCode : protected Msg {
		constexpr GetDlgCode(const Msg &p) : Msg{p} { }
		[[nodiscard]] constexpr WPARAM wparam() const { return wp; }
		[[nodiscard]] constexpr LPARAM lparam() const { return lp; }
		[[nodiscard]] constexpr BYTE   vkey_code() const { return static_cast<BYTE>(wm); }
		[[nodiscard]] constexpr bool   is_query() const  { return lp == 0; }
		[[nodiscard]] MSG*             msg() const       { return this->is_query() ? nullptr : reinterpret_cast<MSG*>(lp); }
		[[nodiscard]] bool             has_alt() const   { return (GetAsyncKeyState(VK_MENU) & 0x8000) != 0; }
		[[nodiscard]] bool             has_ctrl() const  { return (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0; }
		[[nodiscard]] bool             has_shift() const { return (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0; }
	};

	struct HScroll : protected Msg {
		constexpr HScroll(const Msg &p) : Msg{p} { }
		[[nodiscard]] constexpr WORD scroll_request() const { return LOWORD(wp); }
		[[nodiscard]] constexpr WORD scroll_pos() const     { return HIWORD(wp); }
		[[nodiscard]] HWND           hwnd_scrollbar() const { return reinterpret_cast<HWND>(lp); }
	};
	struct VScroll : public HScroll { constexpr VScroll(const Msg &p) : HScroll{p} { } };

	struct InitDialog : protected Msg {
		constexpr InitDialog(const Msg &p) : Msg{p} { }
		[[nodiscard]] HWND hwnd_focus() const { return reinterpret_cast<HWND>(wp); }
	};

	struct InitMenuPopup : protected Msg {
		constexpr InitMenuPopup(const Msg &p) : Msg{p} { }
		[[nodiscard]] HMENU           hmenu() const              { return reinterpret_cast<HMENU>(wp); }
		[[nodiscard]] constexpr short relative_pos() const       { return LOWORD(lp); }
		[[nodiscard]] constexpr bool  is_window_menu() const     { return HIWORD(lp) != FALSE; }
		[[nodiscard]] UINT            first_menu_item_id() const { return GetMenuItemID(hmenu(), 0); }
	};

	struct KillFocus : protected Msg {
		constexpr KillFocus(const Msg &p) : Msg{p} { }
		[[nodiscard]] HWND focused_hwnd() const { return reinterpret_cast<HWND>(wp); }
	};

	struct LButtonDblClk : protected Msg {
		constexpr LButtonDblClk(const Msg &p) : Msg{p} { }
		[[nodiscard]] constexpr WORD  mk_flag() const       { return static_cast<WORD>(wp); }
		[[nodiscard]] constexpr bool  has_ctrl() const      { return (mk_flag() & MK_CONTROL) != 0; }
		[[nodiscard]] constexpr bool  has_shift() const     { return (mk_flag() & MK_SHIFT) != 0; }
		[[nodiscard]] constexpr bool  is_left_btn() const   { return (mk_flag() & MK_LBUTTON) != 0; }
		[[nodiscard]] constexpr bool  is_middle_btn() const { return (mk_flag() & MK_MBUTTON) != 0; }
		[[nodiscard]] constexpr bool  is_right_btn() const  { return (mk_flag() & MK_RBUTTON) != 0; }
		[[nodiscard]] constexpr bool  is_x1_btn() const     { return (mk_flag() & MK_XBUTTON1) != 0; }
		[[nodiscard]] constexpr bool  is_x2_btn() const     { return (mk_flag() & MK_XBUTTON2) != 0; }
		[[nodiscard]] constexpr POINT pos() const           { return {LOWORD(lp), HIWORD(lp)}; }
	};
	struct LButtonDown   : public LButtonDblClk { constexpr LButtonDown(const Msg &p)   : LButtonDblClk{p} { } };
	struct LButtonUp     : public LButtonDblClk { constexpr LButtonUp(const Msg &p)     : LButtonDblClk{p} { } };
	struct MButtonDblClk : public LButtonDblClk { constexpr MButtonDblClk(const Msg &p) : LButtonDblClk{p} { } };
	struct MButtonDown   : public LButtonDblClk { constexpr MButtonDown(const Msg &p)   : LButtonDblClk{p} { } };
	struct MButtonUp     : public LButtonDblClk { constexpr MButtonUp(const Msg &p)     : LButtonDblClk{p} { } };
	struct MouseHover    : public LButtonDblClk { constexpr MouseHover(const Msg &p)    : LButtonDblClk{p} { } };
	struct MouseMove     : public LButtonDblClk { constexpr MouseMove(const Msg &p)     : LButtonDblClk{p} { } };
	struct RButtonDblClk : public LButtonDblClk { constexpr RButtonDblClk(const Msg &p) : LButtonDblClk{p} { } };
	struct RButtonDown   : public LButtonDblClk { constexpr RButtonDown(const Msg &p)   : LButtonDblClk{p} { } };
	struct RButtonUp     : public LButtonDblClk { constexpr RButtonUp(const Msg &p)     : LButtonDblClk{p} { } };

	struct Move : protected Msg {
		constexpr Move(const Msg &p) : Msg{p} { }
		[[nodiscard]] constexpr POINT client_area_pos() const { return {LOWORD(lp), HIWORD(lp)}; }
	};

	struct Moving : protected Msg {
		constexpr Moving(const Msg &p) : Msg{p} { }
		[[nodiscard]] RECT& window_pos() const { return *reinterpret_cast<RECT*>(lp); }
	};

	struct NcCalcSize : protected Msg {
		constexpr NcCalcSize(const Msg &p) : Msg{p} { }
		[[nodiscard]] constexpr bool     should_indicate_valid_part() const { return wp != 0; }
		[[nodiscard]] NCCALCSIZE_PARAMS& nccalcsize_params() const          { return *reinterpret_cast<NCCALCSIZE_PARAMS*>(lp); }
		[[nodiscard]] RECT&              rect() const                       { return *reinterpret_cast<RECT*>(lp); }
	};

	struct NcPaint : protected Msg {
		constexpr NcPaint(const Msg &p) : Msg{p} { }
		[[nodiscard]] HRGN hrgn() const { return reinterpret_cast<HRGN>(wp); }
	};

	struct PowerBroadcast : protected Msg {
		constexpr PowerBroadcast(const Msg &p) : Msg{p} { }
		[[nodiscard]] constexpr WORD                pbt_flag() const   { return static_cast<WORD>(wp); }
		[[nodiscard]] const POWERBROADCAST_SETTING& event_data() const { return *reinterpret_cast<const POWERBROADCAST_SETTING*>(lp); }
	};


	struct SetFocus : protected Msg {
		constexpr SetFocus(const Msg &p) : Msg{p} { }
		[[nodiscard]] HWND hwnd_losing_focus() const { return reinterpret_cast<HWND>(wp); }
	};

	struct Size : protected Msg {
		constexpr Size(const Msg &p) : Msg{p} { }
		[[nodiscard]] constexpr WORD size_flag() const { return static_cast<WORD>(wp); }
		[[nodiscard]] constexpr bool is_other_maximized() const { return size_flag() == SIZE_MAXHIDE; }
		[[nodiscard]] constexpr bool is_maximized() const       { return size_flag() == SIZE_MAXIMIZED; }
		[[nodiscard]] constexpr bool is_other_restored() const  { return size_flag() == SIZE_MAXSHOW; }
		[[nodiscard]] constexpr bool is_minimized() const       { return size_flag() == SIZE_MINIMIZED; }
		[[nodiscard]] constexpr bool is_restored() const        { return size_flag() == SIZE_RESTORED; }
		[[nodiscard]] constexpr SIZE sz() const                 { return {LOWORD(lp), HIWORD(lp)}; }
	};

	struct Sizing : protected Msg {
		constexpr Sizing(const Msg &p) : Msg{p} { }
		[[nodiscard]] constexpr WORD edge() const          { return static_cast<WORD>(wp); }
		[[nodiscard]] RECT&          screen_coords() const { return *reinterpret_cast<RECT*>(lp); }
	};

}

namespace _wl_internal {

	class WindowMsg;
	class NativeCtrl;

	/** Library-internal window message callbacks. */
	class EventsInternal final : wl::NonMovable {
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

		constexpr explicit EventsInternal(bool isDlg) : _isDlg{isDlg} { }

		void wm_create_or_init_dialog(std::function<void()> cb);
		void wm(UINT msg, std::function<void(wl::wm::Msg)> cb);
		void wm_notify(WORD idFrom, int code, std::function<void(wl::wm::Notify)> cb);

		void clear_inis(); // WM_CREATE and WM_INITDIALOG
		void clear();
		[[nodiscard]] bool process_all(wl::wm::Msg procMsg) const;

		bool _isDlg;
		std::vector<std::function<void()>> _inis{}; // WM_CREATE, WM_INITDIALOG
		std::vector<Msg> _msgs{};
		std::vector<Nfy> _nfys{}; // WM_NOTIFY
	};

}

namespace wl::events {

	/** @brief Native window message callbacks. */
	class WindowEvents final : wl::NonMovable {
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

		constexpr explicit WindowEvents(bool isDlg) : _isDlg{isDlg} { }

	public:
		/// Adds a callback to an ordinary WM message.
		///
		/// This is a general method for custom messages, always prefer using the specific message methods.
		///
		/// Example:
		///
		/// ```cpp
		/// myWindow.on().wm(WM_ENTERIDLE, [](wl::wm::Msg p) {
		///     // ...
		///     return 0;
		/// });
		/// ```
		void wm(UINT msg, std::function<LRESULT(wl::wm::Msg)> cb);

		/// Handles the [`WM_CREATE`] message.
		///
		/// Note that this message is sent only to windows created programmatically, not to dialog windows.
		///
		/// Example:
		///
		/// ```cpp
		/// myWindow.on().wm_create([](wl::wm::Create p) {
		///     // ...
		///     return 0;
		/// });
		/// ```
		///
		/// [`WM_CREATE`]: https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-create
		void wm_create(std::function<int(wl::wm::Create)> cb);

		/// Handles the [`WM_INITDIALOG`] message.
		///
		/// Note that this message is sent only to dialog windows, not to windows created programmatically.
		///
		/// Example:
		///
		/// ```cpp
		/// myWindow.on().wm_init_dialog([](wl::wm::InitDialog p) {
		///     // ...
		///     return true;
		/// });
		/// ```
		///
		/// [`WM_INITDIALOG`]: https://learn.microsoft.com/en-us/windows/win32/dlgbox/wm-initdialog
		void wm_init_dialog(std::function<bool(wl::wm::InitDialog)> cb);

		/// Adds a callback to a [`WM_COMMAND`] message, to an specific notification code.
		///
		/// Example:
		///
		/// ```cpp
		/// myWindow.on().wm_command(BTN_MAIN, BN_CLICKED, []() {
		///     // ...
		/// });
		/// ```
		///
		/// [`WM_COMMAND`]: https://learn.microsoft.com/en-us/windows/win32/menurc/wm-command
		void wm_command(WORD cmdId, WORD notifCode, std::function<void()> cb);

		/// Adds a callback to a [`WM_COMMAND`] message, for notifications from both accelerator and menu events.
		///
		/// This is the most common case.
		///
		/// Example:
		///
		/// ```cpp
		/// myWindow.on().wm_command(IDCANCEL, []() {
		///     // ...
		/// });
		/// ```
		///
		/// [`WM_COMMAND`]: https://learn.microsoft.com/en-us/windows/win32/menurc/wm-command
		void wm_command(WORD cmdId, std::function<void()> cb);

		/// Adds a callback to a [`WM_NOTIFY`] message, to specific control ID and notification code.
		///
		/// This is a general method for custom notifications, always prefer using the specific control notification metods.
		///
		/// Example:
		///
		/// ```cpp
		/// myWindow.on().wm_notify(LST_MAIN, LVN_BEGINDRAG, [](wl::wm::Notify p) {
		///     // ...
		///     return 0;
		/// });
		/// ```
		///
		/// [`WM_NOTIFY`]: https://learn.microsoft.com/en-us/windows/win32/controls/wm-notify
		void wm_notify(WORD idFrom, int code, std::function<LRESULT(wl::wm::Notify)> cb);

		void wm_activate(std::function<void(wl::wm::Activate)> cb);
		void wm_activate_app(std::function<void(wl::wm::ActivateApp)> cb);
		void wm_child_activate(std::function<void()> cb);
		void wm_close(std::function<void()> cb);
		void wm_destroy(std::function<void()> cb);
		void wm_enable(std::function<void(wl::wm::Enable)> cb);
		void wm_end_session(std::function<void(wl::wm::EndSession)> cb);
		void wm_enter_size_move(std::function<void()> cb);
		void wm_erase_bkgnd(std::function<int(wl::wm::EraseBkgnd)> cb);
		void wm_exit_size_move(std::function<void()> cb);
		void wm_get_dlg_code(std::function<WORD(wl::wm::GetDlgCode)> cb);
		void wm_h_scroll(std::function<void(wl::wm::HScroll)> cb);
		void wm_init_menu_popup(std::function<void(wl::wm::InitMenuPopup)> cb);
		void wm_kill_focus(std::function<void(wl::wm::KillFocus)> cb);
		void wm_l_button_dbl_clk(std::function<void(wl::wm::LButtonDblClk)> cb);
		void wm_l_button_down(std::function<void(wl::wm::LButtonDown)> cb);
		void wm_l_button_up(std::function<void(wl::wm::LButtonUp)> cb);
		void wm_m_button_dbl_clk(std::function<void(wl::wm::MButtonDblClk)> cb);
		void wm_m_button_down(std::function<void(wl::wm::MButtonDown)> cb);
		void wm_m_button_up(std::function<void(wl::wm::MButtonUp)> cb);
		void wm_mouse_hover(std::function<void(wl::wm::MouseHover)> cb);
		void wm_mouse_move(std::function<void(wl::wm::MouseMove)> cb);
		void wm_move(std::function<void(wl::wm::Move)> cb);
		void wm_moving(std::function<void(wl::wm::Moving)> cb);
		void wm_nc_calc_size(std::function<WORD(wl::wm::NcCalcSize)> cb);
		void wm_nc_destroy(std::function<void()> cb);
		void wm_nc_paint(std::function<void(wl::wm::NcPaint)> cb);
		void wm_paint(std::function<void()> cb);
		void wm_power_broadcast(std::function<void(wl::wm::PowerBroadcast)> cb);
		void wm_r_button_dbl_clk(std::function<void(wl::wm::RButtonDblClk)> cb);
		void wm_r_button_down(std::function<void(wl::wm::RButtonDown)> cb);
		void wm_r_button_up(std::function<void(wl::wm::RButtonUp)> cb);
		void wm_set_focus(std::function<void(wl::wm::SetFocus)> cb);
		void wm_size(std::function<void(wl::wm::Size)> cb);
		void wm_sizing(std::function<void(wl::wm::Sizing)> cb);
		void wm_v_scroll(std::function<void(wl::wm::VScroll)> cb);

	private:
		[[nodiscard]] bool has_message() const;
		void clear_inis(); // WM_CREATE and WM_INITDIALOG
		void clear();
		[[nodiscard]] std::optional<LRESULT> process_last(wl::wm::Msg procMsg) const;

		bool _isDlg;
		std::vector<Msg> _inis{}; // WM_CREATE, WM_INITDIALOG
		std::vector<Msg> _msgs{};
		std::vector<Cmd> _cmds{}; // WM_COMMAND
		std::vector<Nfy> _nfys{}; // WM_NOTIFY

		friend _wl_internal::WindowMsg; // message processing
		friend _wl_internal::NativeCtrl; // subclass processing
	};

}

namespace wl { class WindowParent; }

namespace _wl_internal {

	/// Base to all native control events.
	/// Actually just holds a pointer to parent events, which is where the events are added.
	class EventsNativeCtrl final : wl::NonMovable {
	public:
		EventsNativeCtrl(wl::WindowParent &owner, WORD ctrlId);

		WindowMsg &_owner;
		WORD _ctrlId;
	};

}
