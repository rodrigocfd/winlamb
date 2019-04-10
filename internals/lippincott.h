/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <system_error>
#include <Windows.h>

namespace wl {
namespace _wli {

inline void lippincott() noexcept {
	const char* caption = "Oops... unknown error";
	const char* msg = "An unknown exception, not derived from std::exception, was thrown.";

	// https://stackoverflow.com/a/48036486/6923555
	try { throw; }
	catch (const std::invalid_argument& e) { msg = e.what(); caption = "Oops... invalid argument error"; }
	catch (const std::out_of_range& e)     { msg = e.what(); caption = "Oops... out of range error"; }
	catch (const std::logic_error& e)      { msg = e.what(); caption = "Oops... logic error"; }
	catch (const std::system_error& e)     { msg = e.what(); caption = "Oops... system error"; }
	catch (const std::runtime_error& e)    { msg = e.what(); caption = "Oops... runtime error"; }
	catch (const std::exception& e)        { msg = e.what(); caption = "Oops... generic error"; }

	MessageBoxA(nullptr, msg, caption, MB_ICONERROR);
}

}//namepace _wli
}//namespace wl
