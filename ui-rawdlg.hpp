#pragma once
#include "ui-event.hpp"

namespace _wl_internal {

	/// Base to all raw and dialog windows.
	/// Stores the pre, user and post window messages for container windows.
	/// Exposes exception-safe multi-threading operations.
	class WndBase final : private wl::NoCopyNoMove {
	public:
		constexpr explicit WndBase(bool isDlg)
			: _preEvents{isDlg}, _userEvents{isDlg}, _postEvents{isDlg} { }

		struct ThreadPack final {
			std::function<void()> cb;
		};
		void ui_thread(std::function<void()> &&cb) const;

		struct ProcResult final {
			bool hasPre, hasPost;
			std::optional<LRESULT> userRet;
		};
		ProcResult process_msgs(UINT msg, WPARAM wp, LPARAM lp);

		int main_loop(HACCEL hAccel, bool processDlgMsgs);
		void modal_loop(bool processDlgMsgs);

		HWND _hWnd = nullptr; // _hWnd member is set in wndproc
		Layout _layout{};
		InternalEvents _preEvents;
		wl::events::WindowEvents _userEvents;
		InternalEvents _postEvents;
	};

	class RawMain; // forward declaration
	class RawModal;
	class RawControl;

}

namespace wl {

	class IWindowParent; // forward declaration

	/** Options to create a `WindowMain` programmatically. */
	struct WindowMainOpts final {
		/** Constructor. */
		constexpr WindowMainOpts() { }
		/// Class name passed to [`WNDCLASSEX`].
		///
		/// Defaults to an auto-generated string.
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		WindowMainOpts class_name(WStrView n) { _className = n.c_str(); return std::move(*this); }
		/// Class style passed to [`WNDCLASSEX`].
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		WindowMainOpts class_style(DWORD s) { _classStyle = s; return std::move(*this); }
		/// The window background brush passed to [`WNDCLASSEX`].
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		WindowMainOpts hbr_background(HBRUSH b) { _hbrBackground = b; return std::move(*this); }
		/// The window cursor passed to [`WNDCLASSEX`].
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		WindowMainOpts cursor(HCURSOR c) { _hCursor = c; return std::move(*this); }
		/// The window title, passed to [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		WindowMainOpts title(WStrView t) { _title = t.c_str(); return std::move(*this); }
		/** Optional window icon resource ID. */
		WindowMainOpts icon_id(WORD id) { _iconId = id; return std::move(*this); }
		/// The window size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::WindowMain{
		///     wl::WindowMainOpts{}
		///         .size(wl::dpi::sz(500, 300)),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		WindowMainOpts size(SIZE s) { _size = s; return std::move(*this); }
		/// The window size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::WindowMain{
		///     wl::WindowMainOpts{}
		///         .size(wl::dpi::x(500), wl::dpi::y(300)),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		WindowMainOpts size(int cx, int cy) { return size({cx, cy}); }
		/// The [window style] passed to [`CreateWindowEx`].
		///
		/// For a resizable window, use:
		///
		/// ```cpp
		/// wl::WindowMain{
		///     wl::WindowMainOpts{}
		///         .style(WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN | WS_BORDER | WS_VISIBLE | WS_MINIMIZEBOX),
		/// };
		/// ```
		///
		/// [window style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		WindowMainOpts style(DWORD s) { _style = s; return std::move(*this); }
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		WindowMainOpts style_ex(DWORD sx) { _styleEx = sx; return std::move(*this); }
		/** Adds `WS_SIZEBOX` and `WS_MAXIMIZEBOX` styles to the window. */
		WindowMainOpts resizable() { _style |= WS_SIZEBOX | WS_MAXIMIZEBOX; return std::move(*this); }
		/// Optional window main menu, passed to [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		WindowMainOpts menu(HMENU m) { _hMenu = m; return std::move(*this); }
		/// Window [accelerator table], to define keyboard shortcuts.
		///
		/// [accelerator table]: https://learn.microsoft.com/en-us/windows/win32/learnwin32/accelerator-tables
		WindowMainOpts accel_table(HACCEL a) { _hAccelTable = a; return std::move(*this); }
		/// In most applications, the window loop calls [`IsDialogMessage`] so
		/// child control messages will properly work. However, this has the
		/// side-effect of inhibiting [`WM_CHAR`] messages from being sent, which
		/// is bad for applications like text editors.
		///
		/// So if your application has no child controls and needs to process
		/// `WM_CHAR` messages, pass `false` to suppress `IsDialogMessage` call.
		///
		/// [`IsDialogMessage`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-isdialogmessagew
		/// [`WM_CHAR`]: https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-char
		WindowMainOpts process_dlg_msgs(bool p) { _processDlgMsgs = p; return std::move(*this); }

	private:
		std::wstring _className{};
		DWORD _classStyle = CS_DBLCLKS;
		HBRUSH _hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
		HCURSOR _hCursor = nullptr;
		std::wstring _title{};
		WORD _iconId = 0;
		SIZE _size{-1, -1};
		DWORD _style =  WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN | WS_BORDER | WS_VISIBLE | WS_MINIMIZEBOX;
		DWORD _styleEx = WS_EX_LEFT;
		HMENU _hMenu = nullptr;
		HACCEL _hAccelTable = nullptr;
		bool _processDlgMsgs = true;
		friend _wl_internal::RawMain;
	};

	/** Options to create a `WindowModal` programmatically. */
	struct WindowModalOpts final {
		/** Constructor. */
		constexpr WindowModalOpts(const IWindowParent &parent) : _parent{parent} { }
		/// Class name passed to [`WNDCLASSEX`].
		///
		/// Defaults to an auto-generated string.
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		WindowModalOpts class_name(WStrView n) { _className = n.c_str(); return std::move(*this); }
		/// Class style passed to [`WNDCLASSEX`].
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		WindowModalOpts class_style(DWORD s) { _classStyle = s; return std::move(*this); }
		/// The window background brush passed to [`WNDCLASSEX`].
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		WindowModalOpts hbr_background(HBRUSH b) { _hbrBackground = b; return std::move(*this); }
		/// The window cursor passed to [`WNDCLASSEX`].
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		WindowModalOpts cursor(HCURSOR c) { _hCursor = c; return std::move(*this); }
		/// The window title, passed to [`CreateWindowEx`].
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		WindowModalOpts title(WStrView t) { _title = t.c_str(); return std::move(*this); }
		/** Optional window icon resource ID. */
		WindowModalOpts icon_id(WORD id) { _iconId = id; return std::move(*this); }
		/// The window size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::WindowModal{
		///     wl::WindowModalOpts{wnd}
		///         .size(wl::dpi::sz(400, 200)),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		WindowModalOpts size(SIZE s) { _size = s; return std::move(*this); }
		/// The window size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::WindowModal{
		///     wl::WindowModalOpts{wnd}
		///         .size(wl::dpi::x(400), wl::dpi::y(200)),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		WindowModalOpts size(int cx, int cy) { return size({cx, cy}); }
		/// The [window style] passed to [`CreateWindowEx`].
		///
		/// For a resizable window, use:
		///
		/// ```cpp
		/// wl::WindowModal{
		///     wl::WindowModalOpts{wnd}
		///         .style(WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN | WS_BORDER | WS_VISIBLE),
		/// };
		/// ```
		///
		/// [window style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		WindowModalOpts style(DWORD s) { _style = s; return std::move(*this); }
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		WindowModalOpts style_ex(DWORD sx) { _styleEx = sx; return std::move(*this); }
		/** Adds `WS_SIZEBOX` and `WS_MAXIMIZEBOX` styles to the window. */
		WindowModalOpts resizable() { _style |= WS_SIZEBOX | WS_MAXIMIZEBOX; return std::move(*this); }
		/// In most applications, the window loop calls [`IsDialogMessage`] so
		/// child control messages will properly work. However, this has the
		/// side-effect of inhibiting [`WM_CHAR`] messages from being sent, which
		/// is bad for applications like text editors.
		///
		/// So if your application has no child controls and needs to process
		/// `WM_CHAR` messages, pass `false` to suppress `IsDialogMessage` call.
		///
		/// [`IsDialogMessage`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-isdialogmessagew
		/// [`WM_CHAR`]: https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-char
		WindowModalOpts process_dlg_msgs(bool p) { _processDlgMsgs = p; return std::move(*this); }

	private:
		const IWindowParent &_parent;
		std::wstring _className{};
		DWORD _classStyle = CS_DBLCLKS;
		HBRUSH _hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
		HCURSOR _hCursor = nullptr;
		std::wstring _title{};
		WORD _iconId = 0;
		SIZE _size{-1, -1};
		DWORD _style =  WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN | WS_BORDER | WS_VISIBLE;
		DWORD _styleEx = WS_EX_LEFT;
		bool _processDlgMsgs = true;
		friend _wl_internal::RawModal;
	};

	/** Options to create a `WindowControl` programmatically. */
	struct WindowControlOpts final {
		/** Constructor. */
		constexpr WindowControlOpts(IWindowParent &owner) : _owner{owner} { }
		/// Control ID.
		///
		/// Defaults to an auto-generated number.
		WindowControlOpts ctrl_id(WORD id) { _ctrlId = id; return std::move(*this); }
		/// Class name passed to [`WNDCLASSEX`].
		///
		/// Defaults to an auto-generated string.
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		WindowControlOpts class_name(WStrView n) { _className = n.c_str(); return std::move(*this); }
		/// Class style passed to [`WNDCLASSEX`].
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		WindowControlOpts class_style(DWORD s) { _classStyle = s; return std::move(*this); }
		/// The window background brush passed to [`WNDCLASSEX`].
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		WindowControlOpts hbr_background(HBRUSH b) { _hbrBackground = b; return std::move(*this); }
		/// The window cursor passed to [`WNDCLASSEX`].
		///
		/// [`WNDCLASSEX`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
		WindowControlOpts cursor(HCURSOR c) { _hCursor = c; return std::move(*this); }
		/** Optional window icon resource ID. */
		WindowControlOpts icon_id(WORD id) { _iconId = id; return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::WindowControl{
		///     wl::WindowControlOpts{wnd}
		///         .pos(wl::dpi::pt(10, 10))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		WindowControlOpts pos(POINT p) { _pos = p; return std::move(*this); }
		/// Control position passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::WindowControl{
		///     wl::WindowControlOpts{wnd}
		///         .pos(wl::dpi::x(10) wl::dpi::y(10))
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		WindowControlOpts pos(int x, int y) { return pos({x, y}); }
		/// The window size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::WindowControl{
		///     wl::WindowControlOpts{wnd}
		///         .size(wl::dpi::sz(100, 100)),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		WindowControlOpts size(SIZE s) { _size = s; return std::move(*this); }
		/// The window size passed to [`CreateWindowEx`].
		///
		/// Prefer using DPI-aware values:
		///
		/// ```cpp
		/// wl::WindowControl{
		///     wl::WindowControlOpts{wnd}
		///         .size(wl::dpi::x(100), wl::dpi::y(100)),
		/// };
		/// ```
		///
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		WindowControlOpts size(int cx, int cy) { return size({cx, cy}); }
		/** Horizontal and vertical behavior of the control when the parent window is resized. */
		WindowControlOpts layout(Lay l) { _layout = l; return std::move(*this); }
		/// The [window style] passed to [`CreateWindowEx`].
		///
		/// For a resizable window, use:
		///
		/// ```cpp
		/// wl::WindowControl{
		///     wl::WindowControlOpts{wnd}
		///         .style(WS_CHILD | WS_TABSTOP | WS_GROUP | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS),
		/// };
		/// ```
		///
		/// [window style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		WindowControlOpts style(DWORD s) { _style = s; return std::move(*this); }
		/// The [window extended style] passed to [`CreateWindowEx`].
		///
		/// [window extended style]: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
		/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
		WindowControlOpts style_ex(DWORD sx) { _styleEx = sx; return std::move(*this); }
		/** Adds `WS_EX_CLIENTEDGE` extended style to the window. */
		WindowControlOpts border() { _styleEx |= WS_EX_CLIENTEDGE; return std::move(*this); }

	private:
		IWindowParent &_owner;
		WORD _ctrlId = 0;
		std::wstring _className{};
		DWORD _classStyle = CS_DBLCLKS;
		HBRUSH _hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
		HCURSOR _hCursor = nullptr;
		WORD _iconId = 0;
		POINT _pos{};
		SIZE _size{-1, -1};
		Lay _layout = Lay::hold_hold;
		DWORD _style =  WS_CHILD | WS_TABSTOP | WS_GROUP | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		DWORD _styleEx = WS_EX_LEFT;
		friend _wl_internal::RawControl;
	};

}

namespace _wl_internal {

	/** Base to all raw container windows. */
	class RawBase final : private wl::NoCopyNoMove {
	public:
		constexpr RawBase() = default;

		[[nodiscard]] ATOM register_class(HINSTANCE hInst, std::wstring &&className, DWORD classStyle,
			WORD iconId, HBRUSH hbrBackground, HCURSOR hCursor);
		void create_window(DWORD exStyle, ATOM className, std::wstring &&title, DWORD style,
			POINT pos, SIZE sz, HWND hParent, HMENU hMenu, HINSTANCE hInst);
		void focus_first_child() const;

		static LRESULT CALLBACK raw_proc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

		WndBase _wndBase{false};
	};

	/** Main raw window. */
	class RawMain final : private wl::NoCopyNoMove {
	public:
		explicit RawMain(wl::WindowMainOpts creationOpts);
		int run(HINSTANCE hInst, int cmdShow);

		RawBase _rawBase{};
		wl::WindowMainOpts _opts;
		HWND _hWndChildPrevFocus = nullptr;
	};

	/** Modal raw window. */
	class RawModal final : private wl::NoCopyNoMove {
	public:
		explicit RawModal(wl::WindowModalOpts creationOpts);
		void show();

		RawBase _rawBase{};
		wl::WindowModalOpts _opts;
		HWND _hWndChildPrevFocusParent = nullptr;
	};

	/** Control raw window. */
	class RawControl final : private wl::NoCopyNoMove {
	public:
		explicit RawControl(wl::WindowControlOpts creationOpts);

		RawBase _rawBase{};
	};

	/** Base to all dialog container windows. */
	class DlgBase final : private wl::NoCopyNoMove {
	public:
		constexpr explicit DlgBase(WORD dlgId) : _wndBase{true}, _dlgId{dlgId} { }

		void create_dialog_param(HINSTANCE hInst, HWND hParent);
		void dialog_box_param(HINSTANCE hInst, HWND hParent);
		void set_icon(HINSTANCE hInst, WORD iconId) const;
		HACCEL load_accel(HINSTANCE hInst, WORD accelTblId) const;

		static INT_PTR CALLBACK dlg_proc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);

		WndBase _wndBase;
		WORD _dlgId;
	};

	/** Main dialog window. */
	class DlgMain final : private wl::NoCopyNoMove {
	public:
		DlgMain(WORD dlgId, WORD iconId, WORD accelTblId);
		int run(HINSTANCE hInst, int cmdShow);

		DlgBase _dlgBase;
		WORD _iconId, _accelTblId;
	};

	/** Modal dialog window. */
	class DlgModal final : private wl::NoCopyNoMove {
	public:
		DlgModal(const wl::IWindowParent &parent, WORD dlgId);
		void show();

		DlgBase _dlgBase;
		const wl::IWindowParent &_parent;
	};

	/** Control dialog window. */
	class DlgControl final : private wl::NoCopyNoMove {
	public:
		DlgControl(wl::IWindowParent &owner, WORD dlgId, WORD ctrlId, POINT pos, wl::Lay layout);

		DlgBase _dlgBase;
	};

}
