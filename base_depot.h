/**
 * Part of WinLamb - Win32 API Lambda Library
 * @author Rodrigo Cesar de Freitas Dias
 * @see https://github.com/rodrigocfd/winlamb
 */

#pragma once
#include <functional>
#include <vector>
#include "params.h"

namespace wl {
namespace base {

	template<typename idT>
	class depot final {
	public:
		using funcT = std::function<LONG_PTR(params)>; // works for both LRESULT and LONG_PTR
	private:
		std::vector<std::pair<idT, funcT>> _msgUnits;

	public:
		explicit depot(size_t msgsReserve = 0) {
			this->reserve(msgsReserve); // initial reserve is useful to save realloc time
			this->_msgUnits.emplace_back(); // 1st element is sentinel room
		}

		bool empty() const {
			return this->_msgUnits.size() == 1; // sentinel always present
		}

		void reserve(size_t msgsReserve) {
			this->_msgUnits.reserve(msgsReserve + 1); // +1 because sentinel
		}

		void add(idT id, funcT func) {
			this->_msgUnits.emplace_back(id, std::move(func)); // reverse search: messages can be overwritten
		}

		void add(std::initializer_list<idT> ids, funcT func) {
			const idT* pIds = ids.begin();
			this->add(pIds[0], std::move(func)); // store user func once
			size_t funcIdx = this->_msgUnits.size() - 1;
			for (size_t i = 1; i < ids.size(); ++i) {
				if (pIds[i] != pIds[0]) { // avoid overwriting
					this->add(pIds[i], [this, funcIdx](params p)->LONG_PTR {
						return this->_msgUnits[funcIdx].second(p); // store light wrapper to 1st func
					});
				}
			}
		}

		funcT* find(idT id) {
			this->_msgUnits[0].first = id; // sentinel for reverse linear search
			std::pair<idT, funcT>* revRunner = &this->_msgUnits.back();
			while (revRunner->first != id) --revRunner;
			return revRunner == &this->_msgUnits[0] ?
				nullptr : &revRunner->second;
		}
	};

}//namespace base
}//namespace wl