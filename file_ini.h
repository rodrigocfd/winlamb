/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "file_mapped.h"
#include "lazy_map.h"
#include "str.h"

namespace wl {

// Wrapper to INI file.
class file_ini final {
public:
	lazy_map<std::wstring, lazy_map<std::wstring, std::wstring>> sections;

	const lazy_map<std::wstring, std::wstring>& operator[](const std::wstring& sectionName) const {
		return this->sections.operator[](sectionName);
	}

	lazy_map<std::wstring, std::wstring>& operator[](const std::wstring& sectionName) noexcept {
		return this->sections.operator[](sectionName);
	}

	file_ini& load_from_file(const wchar_t* filePath) {
		std::wstring content = str::parse_blob(file_mapped::util::read(filePath));
		std::vector<std::wstring> lines = str::explode(content, str::get_linebreak(content));
		lazy_map<std::wstring, std::wstring>* curSection = nullptr;
		std::wstring tmpName, tmpValue; // temporary buffers

		for (std::wstring& line : lines) {
			if (str::trim(line).empty()) { // skip blank lines
				continue;
			} else if (line[0] == L'[' && line.back() == L']') { // begin of section found
				tmpName.clear();
				tmpName.insert(0, &line[1], line.length() - 2); // extract section name
				curSection = &this->sections[str::trim(tmpName)]; // if inexistent, will be inserted
			} else if (curSection && line[0] != L';') {
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

	file_ini& save_to_file(const wchar_t* filePath) const {
		file::util::write(filePath,
			str::to_utf8_blob(this->serialize(), str::write_bom::YES));
	}

	file_ini& load_from_file(const std::wstring& filePath)     { return this->load_from_file(filePath.c_str()); }
	file_ini& save_to_file(const std::wstring& filePath) const { return this->save_to_file(filePath.c_str()); }

	// Returns the INI contents as a string, ready to be written to a file.
	std::wstring serialize() const noexcept {
		std::wstring out;
		bool isFirst = true;

		using sectionT = lazy_map<std::wstring, lazy_map<std::wstring, std::wstring>>::entry;
		using entryT = lazy_map<std::wstring, std::wstring>::entry;

		for (const sectionT& sec : this->sections.entries()) {
			if (isFirst) {
				isFirst = false;
			} else {
				out.append(L"\r\n");
			}
			out.append(L"[").append(sec.key).append(L"]\r\n");

			for (const entryT& entry : sec.value.entries()) {
				out.append(entry.key).append(L"=")
					.append(entry.value).append(L"\r\n");
			}
		}
		return out;
	}
};

}//namespace wl
