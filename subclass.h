/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "internals/base_msg_pubm.h"
#include "internals/base_thread_pubm.h"
#include "wnd.h"
#include <CommCtrl.h>

namespace wl {

// Manages window subclassing for a window.
class subclass final :
	public wnd,
	public wli::base_msg_pubm<LRESULT>,
	public wli::base_thread_pubm<LRESULT, 0>
{
private:
	HWND                         _hWnd = nullptr;
	UINT                         _subclassId = -1;
	wli::base_msg<LRESULT>       _baseMsg{_hWnd};
	wli::base_thread<LRESULT, 0> _baseThread{_baseMsg};

public:
	~subclass() {
		this->remove_subclass();
	}

	subclass() :
		wnd(_hWnd), base_msg_pubm(_baseMsg), base_thread_pubm(_baseThread) { }

	subclass(subclass&&) = default;
	subclass& operator=(subclass&&) = default; // movable only

	void remove_subclass() noexcept {
		if (this->hwnd()) {
			RemoveWindowSubclass(this->hwnd(), _subclass_proc, this->_subclassId);
			this->_hWnd = nullptr; // clear HWND
		}
	}

	void install_subclass(HWND hCtrl) {
		if (this->hwnd()) {
			throw std::logic_error("Trying to install subclass twice.");
		} else if (!hCtrl) {
			throw std::invalid_argument("Trying to install subclass on an empty control.");
		}

		this->_hWnd = hCtrl; // store HWND
		if (hCtrl) {
			this->_subclassId = _next_id();
			SetWindowSubclass(hCtrl, _subclass_proc, this->_subclassId,
				reinterpret_cast<DWORD_PTR>(this));
		}
	}

	void install_subclass(HWND hParent, int ctrlId) {
		this->install_subclass(GetDlgItem(hParent, ctrlId));
	}

	void install_subclass(const wnd& ctrl) {
		this->install_subclass(ctrl.hwnd());
	}

	void install_subclass(const wnd& parent, int ctrlId) {
		this->install_subclass(GetDlgItem(parent.hwnd(), ctrlId));
	}

private:
	static LRESULT CALLBACK _subclass_proc(HWND hWnd, UINT msg,
		WPARAM wp, LPARAM lp, UINT_PTR idSubclass, DWORD_PTR refData) noexcept
	{
		subclass* pSelf = reinterpret_cast<subclass*>(refData);

		if (pSelf) {
			if (pSelf->hwnd()) {
				std::pair<bool, LRESULT> procRet =
					pSelf->_baseMsg.process_msg(msg, wp, lp); // catches all message exceptions internally

				if (msg == WM_NCDESTROY) {
					pSelf->remove_subclass();
				}
				if (procRet.first) {
					return procRet.second; // message was processed
				}
			} else if (msg == WM_NCDESTROY) {
				pSelf->remove_subclass();
			}
		} else if (msg == WM_NCDESTROY) {
			RemoveWindowSubclass(hWnd, _subclass_proc, idSubclass);
		}

		return DefSubclassProc(hWnd, msg, wp, lp); // message was not processed
	}

	static UINT _next_id() noexcept {
		static UINT firstId = 0;
		return firstId++;
	}
};

}//namespace wl
