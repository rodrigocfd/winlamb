#pragma once
#include <stdexcept>
#include "lib-include-win.h"
#include "str.h"

namespace _wl_internal {

	[[nodiscard]] HFONT ui_font();
	void apply_ui_font(HWND hWnd);
	void delete_ui_font();

	[[nodiscard]] WORD valid_ctrl_id(WORD ctrlId);
	[[nodiscard]] HINSTANCE wnd_hinst(HWND hWnd);
	[[nodiscard]] std::wstring wnd_text(HWND hWnd);
	void set_wnd_text(HWND hWnd, wl::WStrPtr text);
	[[nodiscard]] SIZE calc_text_bound_box(wl::WStrPtr text);
	[[nodiscard]] SIZE calc_text_bound_box_with_check(wl::WStrPtr text);

	/** Validates opts access. */
	template<typename O>
	constexpr O& valid_opts(HWND hWnd, O &opts) {
		#ifdef _DEBUG
		if (hWnd)
			throw "Cannot change setup options after the window or control is created.";
		#endif
		return opts;
	}

	/** Validates event access. */
	template<typename E>
	constexpr E& valid_event(HWND hWnd, E &events) {
		#ifdef _DEBUG
		if (hWnd)
			throw "Cannot add events after the window or control is created.";
		#endif
		return events;
	}

}
