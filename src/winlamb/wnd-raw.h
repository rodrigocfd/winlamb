#pragma once
#include "lib-include-win.h"
#include "wnd-base.h"
#include "wnd-opts.h"

namespace _wl_internal {

	/** Base to all raw container windows. */
	class RawBase final {
	public:
		RawBase(RawBase&&) = delete; // non-copyable, non-movable

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
	class RawMain final {
	public:
		RawMain(RawMain&&) = delete; // non-copyable, non-movable

		RawMain();
		int run(HINSTANCE hInst, int cmdShow);

		RawBase _rawBase{};
		wl::opts::MainOpts _opts{};
		HWND _hWndChildPrevFocus = nullptr;
	};

	/** Modal raw window. */
	class RawModal final {
	public:
		RawModal(RawModal&&) = delete; // non-copyable, non-movable

		explicit RawModal(const wl::WindowParent &parent);
		void show();

		RawBase _rawBase{};
		const wl::WindowParent &_parent;
		wl::opts::ModalOpts _opts{};
		HWND _hWndChildPrevFocusParent = nullptr;
	};

	/** Control raw window. */
	class RawControl final {
	public:
		RawControl(RawControl&&) = delete; // non-copyable, non-movable

		RawControl(wl::WindowParent &parent);

		RawBase _rawBase{};
		wl::opts::ControlOpts _opts{};
	};

}
