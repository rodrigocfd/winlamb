/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <algorithm>
#include <vector>

namespace wl {

// Utilities to std::vector.
namespace vec {

// Wrapper to std::find. Returns index of first element which is equal to value, otherwise -1.
template<typename T>
inline size_t find(const std::vector<T>& v, const T& value) {
	typename std::vector<T>::const_iterator it = std::find(v.cbegin(), v.cend(), value);
	return it == v.cend() ? -1 : it - v.cbegin();
}

// Wrapper to std::find_if. Returns index of first element which is equal to value, otherwise -1.
template<typename T, typename predicateT>
inline size_t find_if(const std::vector<T>& v, predicateT&& func) {
	// Usage example:
	// vector<wstring> ss = {L"a", L"b", L"c"};
	// vec::find_if(ss, [](const wstring& c) -> bool {
	//   return c == L"a";
	// });
	typename std::vector<T>::const_iterator it = std::find_if(v.cbegin(), v.cend(),
		std::forward<predicateT>(func));
	return it == v.cend() ? -1 : it - v.cbegin();
}

// Wrapper to std::find. Returns true if element exists.
template<typename T>
inline bool exists(const std::vector<T>& v, const T& value) {
	return find(v, value) != -1;
}

// Wrapper to std::find_if. Returns true if predicate matches an element.
template<typename T, typename predicateT>
inline bool exists_if(const std::vector<T>& v, predicateT&& func) {
	// Usage example:
	// vector<wstring> ss = {L"a", L"b", L"c"};
	// vec::exists_if(ss, [](const wstring& c) -> bool {
	//   return c == L"a";
	// });
	return find_if(v, std::forward<predicateT>(func)) != -1;
}

// Appends a vector onto another.
template<typename T>
inline void append(std::vector<T>& v, const std::vector<T>& other) {
	v.insert(v.end(), other.cbegin(), other.cend());
}

// Removes an element by index.
template<typename T>
inline void remove(std::vector<T>& v, size_t index) {
	v.erase(v.begin() + index);
}

// Wrapper to std::remove_if. Removes all elements which match the predicate.
template<typename T, typename predicateT>
inline void remove_if(std::vector<T>& v, predicateT&& func) {
	// https://stackoverflow.com/a/9053941/6923555
	// Usage example:
	// vector<wstring> ss = {L"a", L"b", L"c"};
	// vec::remove_if(ss, [](const wstring& c) -> bool {
	//   return c == L"a";
	// });
	v.erase(
		std::remove_if(v.begin(), v.end(), std::forward<predicateT>(func)),
		v.end()
	);
}

}//namespace vec
}//namespace wl
