#pragma once
#include <stdexcept>
#include "lib-include-win.h"
#include "str.h"

namespace _wl_internal {

	[[nodiscard]] HFONT ui_font();
	void apply_ui_font(HWND hWnd);
	void delete_ui_font();

	[[nodiscard]] WORD valid_ctrl_id(WORD ctrlId);
	[[nodiscard]] std::wstring wnd_text(HWND hWnd);
	void set_wnd_text(HWND hWnd, wl::WStrPtr text);
	[[nodiscard]] SIZE calc_text_bound_box(wl::WStrPtr text);
	[[nodiscard]] SIZE calc_text_bound_box_with_check(wl::WStrPtr text);

	/** Validates event access. */
	template<typename E>
	E& valid_event(HWND hWnd, E &events) {
		#ifdef _DEBUG
		if (hWnd)
			throw std::logic_error("Cannot add events after the window or control is created.");
		#endif
		return events;
	}

}
