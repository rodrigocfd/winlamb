/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "internals/native_control.h"
#include "internals/styler.h"
#include <CommCtrl.h>

/**
 * hwnd_base
 *  native_control
 *   progressbar
 */

namespace wl {

// Wrapper to progressbar control from Common Controls library.
class progressbar final : public wli::native_control<progressbar> {
private:
	class _styler final : public wli::styler<progressbar> {
	public:
		explicit _styler(progressbar* pPb) noexcept : styler(pPb) { }

		progressbar& vertical(bool doSet) noexcept {
			return this->set_style(doSet, PBS_VERTICAL);
		}

		progressbar& smooth_reverse(bool doSet) noexcept {
			return this->set_style(doSet, PBS_SMOOTHREVERSE);
		}
	};

public:
	_styler style{this};

	progressbar& create(HWND hParent, int ctrlId, const wchar_t* caption, POINT pos, SIZE size) {
		return this->native_control::create(hParent, ctrlId, nullptr, pos, size, PROGRESS_CLASS);
	}

	progressbar& create(const wli::hwnd_base* parent, int ctrlId, const wchar_t* caption, POINT pos, SIZE size) {
		return this->create(parent->hwnd(), ctrlId, caption, pos, size);
	}

	progressbar& set_range(int minVal, int maxVal) noexcept {
		SendMessageW(this->hwnd(), PBM_SETRANGE, 0, MAKELPARAM(minVal, maxVal));
		return *this;
	}

	progressbar& set_range(int minVal, size_t maxVal) noexcept {
		return this->set_range(minVal, static_cast<int>(maxVal));
	}

	progressbar& set_pos(int pos) noexcept {
		SendMessageW(this->hwnd(), PBM_SETPOS, pos, 0);
		return *this;
	}

	progressbar& set_pos(size_t pos) noexcept {
		return this->set_pos(static_cast<int>(pos));
	}

	progressbar& set_pos(double pos) noexcept {
		return this->set_pos(static_cast<int>(pos + 0.5));
	}

	progressbar& set_waiting(bool isWaiting) noexcept {
		if (isWaiting) {
			this->style.set_style(true, PBS_MARQUEE); // set this on resource editor won't work
		}
		SendMessageW(this->hwnd(), PBM_SETMARQUEE, static_cast<WPARAM>(isWaiting), 0);

		if (!isWaiting) { // http://stackoverflow.com/a/23689663
			this->style.set_style(false, PBS_MARQUEE);
		}
		return *this;
	}

	int get_pos() noexcept {
		return static_cast<int>(SendMessageW(this->hwnd(), PBM_GETPOS, 0, 0));
	}
};

}//namespace wl
