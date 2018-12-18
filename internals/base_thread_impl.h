/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "base_thread.h"

namespace wl {
namespace wli {

// Provides public methods for base_thread class.
template<typename retT, retT RET_VAL>
class base_thread_impl {
private:
	base_thread<retT, RET_VAL>& _baseThread;

public:
	base_thread_impl(base_thread<retT, RET_VAL>& baseThread) :
		_baseThread(baseThread) { }

	// Runs code asynchronously in a new detached thread.
	template<typename funcT>
	void run_thread_detached(funcT&& func) const noexcept {
		return this->_baseThread.run_thread_detached(std::move(func));
	}

	// Runs code synchronously in the UI thread.
	template<typename funcT>
	void run_thread_ui(funcT&& func) const noexcept {
		return this->_baseThread.run_thread_ui(std::move(func));
	}
};

}//namespace wli
}//namespace wl
