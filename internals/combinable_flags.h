/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <type_traits>

#define WINLAMB_COMBINABLE_FLAGS(flag) \
	flag operator|(flag a, flag b) noexcept { \
		return static_cast<flag>(static_cast<std::underlying_type<flag>::type>(a) | \
			static_cast<std::underlying_type<flag>::type>(b)); \
	}
