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
	using bstr = wli::com_bstr;
	using lib = wli::com_lib;
	template<typename ptrT> using ptr = wli::com_ptr<ptrT>;
	using variant = wli::com_variant;

	// Calls FAILED() macro upon HRESULT; if failed, throws an system_error.
	static void check_hr(HRESULT hr, const char* exceptionMsg) {
		wli::check_hr(hr, exceptionMsg);
	}
};

}//namespace wl
