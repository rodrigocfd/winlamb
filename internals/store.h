/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <functional>
#include <vector>
#include "params.h"

namespace wl {
namespace wli {

// Generic storage for message identifiers and their respective lambda handlers.
template<typename idT, typename retT>
class store final {
public:
	using funcT = std::function<retT(params)>; // retT is LRESULT or INT_PTR

private:
	struct _msg_unit final {
		idT   id;
		funcT func;
		_msg_unit() = default;
		_msg_unit(idT id, funcT&& func) noexcept : id(id), func(std::move(func)) { }
	};

	std::vector<_msg_unit> _msgUnits;

public:
	explicit store(size_t msgsReserve = 0) {
		this->reserve(msgsReserve); // initial reserve is useful to save realloc time
		this->_msgUnits.emplace_back(); // 1st element is sentinel room
	}

	bool empty() const noexcept {
		return this->_msgUnits.size() == 1; // sentinel always present
	}

	void reserve(size_t msgsReserve) {
		this->_msgUnits.reserve(msgsReserve + 1); // +1 because sentinel
	}

	template<typename handlerT>
	void add(idT id, handlerT&& func) {
		this->_msgUnits.emplace_back(id, std::move(func)); // reverse search: messages can be overwritten by a later one
	}

	template<typename handlerT>
	void add(std::initializer_list<idT> ids, handlerT&& func) {
		const idT* pIds = ids.begin();
		this->add(pIds[0], std::move(func)); // store user func once
		size_t funcIdx = this->_msgUnits.size() - 1;
		for (size_t i = 1; i < ids.size(); ++i) {
			if (pIds[i] != pIds[0]) { // avoid overwriting
				this->add(pIds[i], [this, funcIdx](params p)->retT {
					return this->_msgUnits[funcIdx].func(p); // store light wrapper to 1st func
				});
			}
		}
	}

	funcT* find(idT id) {
		this->_msgUnits[0].id = id; // sentinel for reverse linear search
		_msg_unit* revRunner = &this->_msgUnits.back(); // pointer to last element
		while (revRunner->id != id) --revRunner;
		return revRunner == &this->_msgUnits[0] ? // if we stopped only at 1st element, id wasn't found
			nullptr : &revRunner->func;
	}
};

}//namespace wli
}//namespace wl
