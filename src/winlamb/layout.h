#pragma once
#include <vector>
#include "events.h"

namespace _wl_internal {

	constexpr BYTE LAY_H_MOVE   = 0b0000'0001;
	constexpr BYTE LAY_H_RESIZE = 0b0000'0010;
	constexpr BYTE LAY_V_MOVE   = 0b0000'0100;
	constexpr BYTE LAY_V_RESIZE = 0b0000'1000;

}

namespace wl {

	/** @brief Specifies the horizontal and vertical behavior for a control when the parent window is resized. */
	enum class Lay : BYTE {
		/** When parent is resized, nothing happens. */
		hold_hold = 0,
		/// When parent resizes:
		/// - horizontal: nothing happens;
		/// - vertical: control moves anchored at bottom.
		hold_move = _wl_internal::LAY_V_MOVE,
		/// When parent resizes:
		/// - horizontal: nothing happens;
		/// - vertical: control is resized together.
		hold_resize = _wl_internal::LAY_V_RESIZE,
		/// When parent resizes:
		/// - horizontal: control moves anchored at right;
		/// - vertical: nothing happens.
		move_hold = _wl_internal::LAY_H_MOVE,
		/// When parent resizes:
		/// - horizontal: control moves anchored at right;
		/// - vertical: control moves anchored at bottom.
		move_move = _wl_internal::LAY_H_MOVE | _wl_internal::LAY_V_MOVE,
		/// When parent resizes:
		/// - horizontal: control moves anchored at right;
		/// - vertical: control is resized together.
		move_resize = _wl_internal::LAY_H_MOVE | _wl_internal::LAY_V_RESIZE,
		/// When parent resizes:
		/// - horizontal: control is resized together;
		/// - vertical: nothing happens.
		resize_hold = _wl_internal::LAY_H_RESIZE,
		/// When parent resizes:
		/// - horizontal: control is resized together;
		/// - vertical: control moves anchored at bottom.
		resize_move = _wl_internal::LAY_H_RESIZE | _wl_internal::LAY_V_MOVE,
		/// When parent resizes:
		/// - horizontal: control is resized together;
		/// - vertical: control is resized together.
		resize_resize = _wl_internal::LAY_H_RESIZE | _wl_internal::LAY_V_RESIZE,
	};

}

namespace _wl_internal {

	/** Rearranges position and size of each control when the parent resizes, according to Lay flags. */
	class Layout final {
	public:
		struct Ctrl final {
			HWND hCtrl;
			wl::Lay layout;
			RECT rcOrig;
		};

		void add(HWND hCtrl, wl::Lay layout);
		void rearrange(wl::wm::Size p);

		std::vector<Ctrl> _ctrls{};
		SIZE _szOrig{}; // original parent client area
	};

}
