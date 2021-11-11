
#pragma once
#include <vector>
#include <Windows.h>

namespace wl {
	// Horizontal action to be performed when the parent window is resized.
	enum class horz {
		none,   // When parent is resized, nothing happens to the control.
		repos,  // When parent is resized, control moves anchored at right.
		resize, // When parent is resized, control is resized together.
	};

	// Vertical action to be performed when the parent window is resized.
	enum class vert {
		none,   // When parent is resized, nothing happens to the control.
		repos,  // When parent is resized, control moves anchored at bottom.
		resize, // When parent is resized, control is resized together.
	};
}

namespace _wli {

// When parent is resized, automatically resizes the registered children.
class resizer_children final {
private:
	struct ctrl_data {
		HWND hctrl;
		RECT rc_orig;
		wl::horz fhorz;
		wl::vert fvert;
	};

	std::vector<ctrl_data> _ctrls;
	SIZE _sz_orig; // original size of parent's client area

public:
	void add(HWND hparent, HWND hctrl, wl::horz fhorz, wl::vert fvert);
	void resize(WORD request, SIZE client_area);
};

}
