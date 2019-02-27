/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "file_mapped.h"
#include "insert_order_map.h"
#include "str.h"

namespace wl {

// Wrapper to INI file.
class file_ini final {
public:
	insert_order_map<std::wstring, insert_order_map<std::wstring, std::wstring>> sections;

	const insert_order_map<std::wstring, std::wstring>& operator[](const std::wstring& sectionName) const {
		return this->sections.operator[](sectionName);
	}

	insert_order_map<std::wstring, std::wstring>& operator[](const std::wstring& sectionName) noexcept {
		return this->sections.operator[](sectionName);
	}

	file_ini& load_from_file(const wchar_t* filePath) {
		std::wstring content = str::to_wstring(file_mapped::util::read(filePath));
		std::vector<std::wstring> lines = str::split_lines(content);
		insert_order_map<std::wstring, std::wstring>* curSection = nullptr; // section-less keys will be ignored
		std::wstring tmpName, tmpValue; // temporary buffers

		for (std::wstring& line : lines) {
			if (str::trim(line).empty()) { // skip blank lines
				continue;
			} else if (line[0] == L'[' && line.back() == L']') { // begin of section found
				tmpName.clear();
				tmpName.insert(0, &line[1], line.length() - 2); // extract section name
				curSection = &this->sections[str::trim(tmpName)]; // if inexistent, will be inserted
			} else if (curSection && line[0] != L';' && line[0] != L'#') { // lines starting with ; or # will be ignored
				size_t idxEq = line.find_first_of(L'=');
				if (idxEq != std::wstring::npos) {
					tmpName.clear();
					tmpName.insert(0, &line[0], idxEq); // extract key name
					tmpValue.clear();
					tmpValue.insert(0, &line[idxEq + 1], line.length() - (idxEq + 1)); // extract value
					(*curSection)[tmpName] = tmpValue;
				}
			}
		}

		return *this;
	}

	void save_to_file(const wchar_t* filePath) const {
		file::util::write(filePath,
			str::to_utf8_blob(this->serialize(), str::write_bom::YES));
	}

	file_ini& load_from_file(const std::wstring& filePath)     { return this->load_from_file(filePath.c_str()); }
	void      save_to_file(const std::wstring& filePath) const { this->save_to_file(filePath.c_str()); }

	// Returns the INI contents as a string, ready to be written to a file.
	std::wstring serialize() const {
		std::wstring out;
		bool isFirst = true;

		using sectionT = insert_order_map<std::wstring, insert_order_map<std::wstring, std::wstring>>::entry;
		using entryT = insert_order_map<std::wstring, std::wstring>::entry;

		for (const sectionT& sectionEntry : this->sections) {
			if (isFirst) {
				isFirst = false;
			} else {
				out.append(L"\r\n");
			}
			out.append(L"[").append(sectionEntry.key).append(L"]\r\n");

			for (const entryT& keyEntry : sectionEntry.value) {
				out.append(keyEntry.key).append(L"=")
					.append(keyEntry.value).append(L"\r\n");
			}
		}
		return out;
	}

	// Checks INI file structure against "[section1]keyA,keyB,keyC[section2]keyX,keyY".
	bool structure_is(const std::wstring& structure) const {
		using strvecT = std::vector<std::wstring>;
		for (const insert_order_map<std::wstring, strvecT>::entry& descrSectionEntry : this->_parse_structure(structure)) {
			const insert_order_map<std::wstring, std::wstring>* pCurSection = this->sections.get_if_exists(descrSectionEntry.key);
			if (!pCurSection) return false; // section name not found
			for (const std::wstring& descrKeyEntry : descrSectionEntry.value) {
				if (!pCurSection->has(descrKeyEntry)) return false; // key name not found
			}
		}
		return true;
	}

private:
	insert_order_map<std::wstring, std::vector<std::wstring>> _parse_structure(const std::wstring& structure) const {
		using strvecT = std::vector<std::wstring>;
		insert_order_map<std::wstring, strvecT> parsed;
		strvecT secBlocks = str::split(structure, L"[");
		for (std::wstring& secBlock : secBlocks) {
			if (secBlock.empty()) continue;
			size_t endSecIdx = secBlock.find_first_of(L']');
			strvecT& curSec = parsed[secBlock.substr(0, endSecIdx)];
			secBlock.erase(0, endSecIdx + 1);
			curSec = str::split(secBlock, L",");
		}
		return parsed;
	}
};

}//namespace wl
