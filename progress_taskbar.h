/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <stdexcept>
#include "hwnd_base.h"
#include "com.h"
#include <ShObjIdl.h>

namespace wl {

// Allows to show a progress bar in the taskbar button of the window, in green, yellow or red.
class progress_taskbar final {
private:
	HWND                    _hWnd = nullptr;
	com::lib                _comLib{com::lib::init::LATER};
	com::ptr<ITaskbarList3> _bar;

public:
	progress_taskbar() = default;

	progress_taskbar(const progress_taskbar&) = delete;
	progress_taskbar& operator=(const progress_taskbar&) = delete; // non-copyable, non-movable

	progress_taskbar& init(HWND hOwner) {
		if (this->_bar) {
			throw std::logic_error("Trying to init a progress taskbar twice.");
		} else {
			this->_hWnd = hOwner;
			this->_comLib.initialize();
			this->_bar.co_create_instance(CLSID_TaskbarList);
		}
		return *this;
	}

	progress_taskbar& init(const hwnd_base* owner) {
		return this->init(owner->hwnd());
	}

	progress_taskbar& set_pos(size_t percent, size_t total) noexcept {
		this->_bar->SetProgressValue(this->_hWnd, static_cast<ULONGLONG>(percent),
			static_cast<ULONGLONG>(total));
		return *this;
	}

	progress_taskbar& set_pos(double percent) noexcept     { return this->set_pos(static_cast<size_t>(percent + 0.5), 100); }
	progress_taskbar& set_waiting(bool isWaiting) noexcept { return this->_set_state(isWaiting ? TBPF_INDETERMINATE : TBPF_NORMAL); }
	progress_taskbar& set_pause(bool isPaused) noexcept    { return this->_set_state(isPaused ? TBPF_PAUSED : TBPF_NORMAL); }
	progress_taskbar& set_error(bool hasError) noexcept    { return this->_set_state(hasError ? TBPF_ERROR : TBPF_NORMAL); }
	progress_taskbar& clear() noexcept                     { return this->_set_state(TBPF_NOPROGRESS); }

private:
	progress_taskbar& _set_state(TBPFLAG state) noexcept {
		this->_bar->SetProgressState(this->_hWnd, state);
		return *this;
	}
};

}//namespace wl
