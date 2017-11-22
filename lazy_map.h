/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <vector>

namespace wl {

// Vector-based associative container which keeps the insertion order.
template<typename keyT, typename valueT>
class lazy_map final {
public:
	struct entry final {
		keyT   key;
		valueT value;
	};

private:
	std::vector<entry> _entries;

public:
	lazy_map() = default;
	lazy_map(std::initializer_list<entry> entries) : _entries{entries} { }

	const std::vector<entry>& entries() const            { return this->_entries; }
	size_t                    size() const               { return this->_entries.size(); }
	bool                      empty() const              { return this->_entries.empty(); }
	lazy_map&                 clear()                    { this->_entries.clear(); return *this; }
	lazy_map&                 reserve(size_t numEntries) { this->_entries.reserve(numEntries); return *this; }

	const valueT& operator[](const keyT& key) const {
		std::vector<entry>::const_iterator ite = this->_find(key);
		if (ite == this->_entries.cend()) {
			throw std::out_of_range("Key doesn't exist.");
		}
		return ite->value;
	}

	valueT& operator[](const keyT& key) {
		std::vector<entry>::iterator ite = this->_find(key);
		if (ite == this->_entries.end()) {
			this->_entries.push_back({key, {}}); // inexistent, so add
			return this->_entries.back().value;
		}
		return ite->value;
	}

	// Returns pointer to value, if key doesn't exist returns nullptr.
	const valueT* get_if_exists(const keyT& key) const {
		// Saves time, instead of calling has() and operator[]().
		std::vector<entry>::const_iterator ite = this->_find(key);
		return (ite == this->_entries.cend()) ?
			nullptr : &ite->value;
	}

	// Returns pointer to value, if key doesn't exist returns nullptr.
	valueT* get_if_exists(const keyT& key) {
		std::vector<entry>::iterator ite = this->_find(key);
		return (ite == this->_entries.end()) ?
			nullptr : &ite->value;
	}

	// Does the key exist?
	bool has(const keyT& key) const {
		return this->_find(key) != this->_entries.cend();
	}

	lazy_map& remove(const keyT& key) {
		std::vector<entry>::iterator ite = this->_find(key);
		if (ite != this->_entries.end()) { // won't fail if inexistent
			this->_entries.erase(ite);
		}
		return *this;
	}

private:
	typename std::vector<entry>::const_iterator _find(const keyT& key) const {
		for (std::vector<entry>::const_iterator ite = this->_entries.cbegin();
			ite != this->_entries.cend(); ++ite)
		{
			if (ite->key == key) return ite;
		}
		return this->_entries.cend();
	}

	typename std::vector<entry>::iterator _find(const keyT& key) {
		for (std::vector<entry>::iterator ite = this->_entries.begin();
			ite != this->_entries.end(); ++ite)
		{
			if (ite->key == key) return ite;
		}
		return this->_entries.end();
	}
};

}//namespace wl
