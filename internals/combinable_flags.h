/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <type_traits>

#define WINLAMB_COMBINABLE_FLAGS(flag) \
	inline flag operator|(flag a, flag b) noexcept { \
		return static_cast<flag>(static_cast<std::underlying_type<flag>::type>(a) | \
			static_cast<std::underlying_type<flag>::type>(b)); \
	}

#define WINLAMB_COMBINED_FLAGS3(a, b, c) \
	a##_##b       = (a | b), \
	a##_##c       = (a | c), \
	a##_##b##_##c = (a | b | c), \
	b##_##c       = (b | c)

#define WINLAMB_COMBINED_FLAGS4(a, b, c, d) \
	a##_##b             = (a | b), \
	a##_##b##_##c       = (a | b | c), \
	a##_##b##_##d       = (a | b | d), \
	a##_##b##_##c##_##d = (a | b | c | d), \
	a##_##c             = (a | c), \
	a##_##c##_##d       = (a | c | d), \
	a##_##d             = (a | d), \
	b##_##c             = (b | c), \
	b##_##c##_##d       = (b | c | d), \
	b##_##d             = (b | d), \
	c##_##d             = (c | d)
