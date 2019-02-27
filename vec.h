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
class vec final {
private:
	vec() = delete;

public:
	// Returns index of first element which is equal to value, otherwise -1.
	template<typename T>
	static size_t find(const std::vector<T>& v, const T& value) {
		std::vector<T>::const_iterator it = std::find(v.cbegin(), v.cend(), value);
		return it == v.cend() ? -1 : it - v.cbegin();
	}

	// Returns index of first element which predicate matches, otherwise -1.
	template<typename T, typename predicateT>
	static size_t find_if(const std::vector<T>& v, predicateT&& func) {
		std::vector<T>::const_iterator it = std::find_if(v.cbegin(), v.cend(), std::move(func));
		return it == v.cend() ? -1 : it - v.cbegin();
	}

	// Returns true if element exists.
	template<typename T>
	static bool exists(const std::vector<T>& v, const T& value) {
		return find(v, value) != -1;
	}

	// Returns true if predicate matches an element.
	template<typename T, typename predicateT>
	static bool exists_if(const std::vector<T>& v, predicateT&& func) {
		return find_if(v, std::move(func)) != -1;
	}

	// Appends a vector onto another.
	template<typename T>
	static void append(std::vector<T>& v, const std::vector<T>& other) {
		v.insert(v.end(), other.cbegin(), other.cend());
	}

	// Removes an element by index.
	template<typename T>
	static void remove(std::vector<T>& v, size_t index) {
		v.erase(v.begin() + index);
	}

	// Removes all elements which match the predicate.
	template<typename T, typename predicateT>
	static void remove_if(std::vector<T>& v, predicateT&& func) {
		// https://stackoverflow.com/a/9053941/6923555
		// Usage example:
		// vector<wstring> de = {L"a", L"b", L"c"};
		// vec::remove_if(de, [](const wstring& d) -> bool {
		//		return d == L"a";
		// });
		v.erase(
			std::remove_if(v.begin(), v.end(), std::move(func)),
			v.end()
		);
	}
};

}//namespace wl
