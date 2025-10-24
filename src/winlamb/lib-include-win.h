#pragma once


// Proper #include <Windows.h>

#include <sdkddkver.h>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


// "Rule of 5" shorthand macros for classes.

#define DEF_COPY(c) \
	constexpr c(const c&) = default; \
	constexpr c& operator=(const c&) = default;

#define DEF_MOVE(c) \
	constexpr c(c&&) = default; \
	constexpr c& operator=(c&&) = default;

#define DEF_COPY_MOVE(c) \
	DEF_COPY(c); \
	DEF_MOVE(c);

#define DEL_COPY(c) \
	c(const c&) = delete; \
	c& operator=(const c&) = delete;

#define DEL_MOVE(c) \
	c(c&&) = delete; \
	c& operator=(c&&) = delete;

#define DEL_COPY_MOVE(c) \
	DEL_COPY(c); \
	DEL_MOVE(c);
