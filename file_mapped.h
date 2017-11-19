/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "file.h"

namespace wl {

// Wrapper to a memory-mapped file.
class file_mapped final {
private:
	file   _file;
	HANDLE _hMap = nullptr;
	void*  _pMem = nullptr;

public:
	~file_mapped() {
		this->close();
	}

	file_mapped() = default;
	file_mapped(file_mapped&& other) { this->operator=(std::move(other)); }

	file_mapped& operator=(file_mapped&& other) {
		this->close();
		std::swap(this->_file, other._file);
		std::swap(this->_hMap, other._hMap);
		std::swap(this->_pMem, other._pMem);
		return *this;
	}

	file::access access_type() const { return this->_file.access_type(); }
	size_t       size()              { return this->_file.size(); }
	BYTE*        p_mem() const       { return reinterpret_cast<BYTE*>(this->_pMem); }
	BYTE*        p_past_mem()        { return p_mem() + this->size(); }

	file_mapped& close() {
		if (this->_pMem) {
			UnmapViewOfFile(this->_pMem);
			this->_pMem = nullptr;
		}
		if (this->_hMap) {
			CloseHandle(this->_hMap);
			this->_hMap = nullptr;
		}
		this->_file.close();
		return *this;
	}

	file_mapped& open(const std::wstring& filePath, file::access accessType) {
		this->close();

		// Open file.
		this->_file.open_existing(filePath, accessType);

		auto tooBad = [this](DWORD err, const char* msg)->void {
			this->close();
			throw std::system_error(err, std::system_category(), msg);
		};

		// Mapping into memory.
		this->_hMap = CreateFileMappingW(this->_file.hfile(), nullptr,
			(accessType == file::access::READWRITE) ? PAGE_READWRITE : PAGE_READONLY, 0, 0, nullptr);
		if (!this->_hMap) {
			tooBad(GetLastError(), accessType == file::access::READWRITE ?
				"CreateFileMapping failed to map file as read-write" :
				"CreateFileMapping failed to map file as read-only");
		}

		// Get pointer to data block.
		this->_pMem = MapViewOfFile(this->_hMap,
			(accessType == file::access::READWRITE) ? FILE_MAP_WRITE : FILE_MAP_READ, 0, 0, 0);
		if (!this->_pMem) {
			tooBad(GetLastError(), accessType == file::access::READWRITE ?
				"CreateFileMapping failed to map file as read-write" :
				"CreateFileMapping failed to map file as read-only");
		}

		return *this;
	}

private:
	void _check_file_mapped() const {
		if (!this->_hMap || !this->_pMem || !this->_file.hfile()) {
			throw std::logic_error("File has not been mapped.");
		}
	}

public:
	file_mapped& set_new_size(size_t newSize) {
		// This method will truncate or expand the file, according to the new size.
		// It will probably fail if file was opened as read-only.

		this->_check_file_mapped();

		// Unmap file, but keep it open.
		UnmapViewOfFile(this->_pMem);
		CloseHandle(this->_hMap);

		// Truncate/expand file.
		this->_file.set_new_size(newSize);

		auto tooBad = [this](DWORD err, const char* msg)->void {
			this->close();
			throw std::system_error(err, std::system_category(), msg);
		};

		// Remap into memory.
		this->_hMap = CreateFileMappingW(this->_file.hfile(), 0, PAGE_READWRITE, 0, 0, nullptr);
		if (!this->_hMap) {
			tooBad(GetLastError(), "CreateFileMapping failed to recreate mapping");
		}

		// Get new pointer to data block, old one just became invalid.
		this->_pMem = MapViewOfFile(this->_hMap, FILE_MAP_WRITE, 0, 0, 0);
		if (!this->_pMem) {
			tooBad(GetLastError(), "MapViewOfFile failed to recreate mapping");
		}

		return *this;
	}

	file_mapped& read_to_buffer(std::vector<BYTE>& buf, size_t offset = 0, size_t numBytes = -1) {
		this->_check_file_mapped();
		if (offset >= this->size()) {
			throw std::invalid_argument("Offset is beyond end of file.");
		} else if (numBytes == -1 || offset + numBytes > this->size()) {
			numBytes = this->size() - offset; // avoid reading beyond EOF
		}

		buf.resize(numBytes);
		memcpy(&buf[0], this->p_mem() + offset, numBytes * sizeof(BYTE));
		return *this;
	}

	std::vector<BYTE> read(size_t offset = 0, size_t numBytes = -1) {
		std::vector<BYTE> buf;
		this->read_to_buffer(buf, offset, numBytes);
		return buf;
	}

public:
	static void quick_read_to_buffer(const wchar_t* filePath, std::vector<BYTE>& buf) {
		file_mapped fin;
		fin.open(filePath, file::access::READONLY);
		fin.read_to_buffer(buf);
	}

	static std::vector<BYTE> quick_read(const wchar_t* filePath) {
		std::vector<BYTE> buf;
		quick_read_to_buffer(filePath, buf);
		return buf;
	}

	static void              quick_read_to_buffer(const std::wstring& filePath, std::vector<BYTE>& buf) { quick_read_to_buffer(filePath.c_str(), buf); }
	static std::vector<BYTE> quick_read(const std::wstring& filePath)                                   { return quick_read(filePath.c_str()); }
};

}//namespace wl
