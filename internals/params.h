/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <Windows.h>

namespace wl {

// Simple struct with message handling data.
struct params {
	UINT   message;
	WPARAM wParam;
	LPARAM lParam;
};

}//namespace wl
