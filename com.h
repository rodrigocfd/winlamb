/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "internals/com_bstr.h"
#include "internals/com_lib.h"
#include "internals/com_ptr.h"
#include "internals/com_variant.h"

namespace wl {

// Utility wrappers to COM objects.
struct com final {
private:
	com() = delete;

public:
	using bstr = _wli::com_bstr;
	using lib = _wli::com_lib;
	template<typename ptrT> using ptr = _wli::com_ptr<ptrT>;
	using variant = _wli::com_variant;

	// Calls FAILED() macro upon HRESULT; if failed, throws a system_error.
	static void check_hr(HRESULT hr, const char* exceptionMsg) {
		_wli::check_hr(hr, exceptionMsg);
	}
};

}//namespace wl
