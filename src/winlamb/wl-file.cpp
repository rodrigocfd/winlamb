#include <system_error>
#include "file.h"
using namespace wl;

File& File::operator=(File&& other) noexcept {
	close();
	std::swap(_hFile, other._hFile);
	return *this;
}

void File::close() noexcept {
	if (_hFile) {
		CloseHandle(_hFile);
		_hFile = nullptr;
	}
}

File& File::open(WStrPtr path, Access access) {
	close();
	DWORD acc = 0, share = 0, disp = 0;

	switch (access) {
	case Access::ExistingReadOnly:
		acc = GENERIC_READ;
		share = FILE_SHARE_READ;
		disp = OPEN_EXISTING;
		break;
	case Access::ExistingRW:
		acc = GENERIC_READ | GENERIC_WRITE;
		disp = OPEN_EXISTING;
		break;
	case Access::OpenOrCreateRW:
		acc = GENERIC_READ | GENERIC_WRITE;
		disp = OPEN_ALWAYS;
		break;
	case Access::CreateRW:
		acc = GENERIC_READ | GENERIC_WRITE;
		disp = CREATE_NEW;
	}

	_hFile = CreateFileW(path, acc, share, nullptr, disp, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (!_hFile || _hFile == INVALID_HANDLE_VALUE) [[unlikely]] {
		throw std::system_error(GetLastError(), std::system_category(), "CreateFile failed.");
	}
	return *this;
}

size_t File::ptr_offset() const {
	LARGE_INTEGER zeroOffset{}, curOffset{};
	if (!SetFilePointerEx(_hFile, zeroOffset, &curOffset, FILE_CURRENT)) [[unlikely]] {
		throw std::system_error(GetLastError(), std::system_category(), "SetFilePointerEx failed.");
	}
	return static_cast<size_t>(curOffset.QuadPart);
}

std::vector<BYTE> File::read(size_t numBytes) const {
	std::vector<BYTE> buf(numBytes ? numBytes : size(), 0x00);
	read_buf(buf);
	return buf;
}

const File& File::read_buf(std::vector<BYTE>& buf) const {
	DWORD read = 0;
	if (!ReadFile(_hFile, buf.data(), static_cast<DWORD>(buf.size()), &read, nullptr)) [[unlikely]] {
		throw std::system_error(GetLastError(), std::system_category(), "ReadFile failed.");
	}
	return *this;
}

const File& File::set_ptr_offset(size_t offset) const {
	LARGE_INTEGER off{.QuadPart = static_cast<LONGLONG>(offset)};
	if (!SetFilePointerEx(_hFile, off, nullptr, FILE_BEGIN)) [[unlikely]] {
		throw std::system_error(GetLastError(), std::system_category(), "SetFilePointerEx failed.");
	}
	return *this;
}

size_t File::size() const {
	LARGE_INTEGER sz{};
	if (!GetFileSizeEx(_hFile, &sz)) [[unlikely]] {
		throw std::system_error(GetLastError(), std::system_category(), "GetFileSizeEx failed.");
	}
	return static_cast<size_t>(sz.QuadPart);
}

File::Times File::times() const {
	FILETIME ftCreation{}, ftLastAccess{}, ftLastWrite{};
	if (!GetFileTime(_hFile, &ftCreation, &ftLastAccess, &ftLastWrite)) [[unlikely]] {
		throw std::system_error(GetLastError(), std::system_category(), "GetFileTime failed.");
	}

	SYSTEMTIME stCreationUtc{}, stLastAccessUtc{}, stLastWriteUtc{};
	FileTimeToSystemTime(&ftCreation, &stCreationUtc);
	FileTimeToSystemTime(&ftLastAccess, &stLastAccessUtc);
	FileTimeToSystemTime(&ftLastWrite, &stLastWriteUtc);

	Times ret;
	SystemTimeToTzSpecificLocalTime(nullptr, &stCreationUtc, &ret.creation);
	SystemTimeToTzSpecificLocalTime(nullptr, &stLastAccessUtc, &ret.lastAccess);
	SystemTimeToTzSpecificLocalTime(nullptr, &stLastWriteUtc, &ret.lastWrite);

	return ret;
}

const File& File::truncate() const {
	set_ptr_offset(0);
	if (!SetEndOfFile(_hFile)) [[unlikely]] {
		throw std::system_error(GetLastError(), std::system_category(), "SetEndOfFile failed.");
	}
	return *this;
}

const File& File::write(std::span<BYTE> data) const {
	DWORD written = 0;
	if (!WriteFile(_hFile, data.data(), static_cast<DWORD>(data.size_bytes()), &written, nullptr)) [[unlikely]] {
		throw std::system_error(GetLastError(), std::system_category(), "WriteFile failed.");
	}
	return *this;
}

void File::erase_and_write(WStrPtr path, std::span<BYTE> data) {
	File f{path, Access::OpenOrCreateRW};
	f.truncate();
	f.write(data);
}

////////////////////////////////////////////////////////////////////////////////

FileMapped& FileMapped::operator=(FileMapped&& other) noexcept {
	close();
	std::swap(_file, other._file);
	std::swap(_hMap, other._hMap);
	std::swap(_pMem, other._pMem);
	std::swap(_sz, other._sz);
	return *this;
}

void FileMapped::close() noexcept {
	_sz = 0;
	if (_pMem) {
		UnmapViewOfFile(_pMem);
		_pMem = nullptr;
	}
	if (_hMap) {
		CloseHandle(_hMap);
		_hMap = nullptr;
	}
	_file.close();
}

FileMapped& FileMapped::open(WStrPtr path, Access access) {
	close();
	auto facc = (access == Access::ExistingReadOnly) ? // translate FileMap to File access
		File::Access::ExistingReadOnly : File::Access::ExistingRW;
	DWORD page = (access == Access::ExistingReadOnly) ? PAGE_READONLY : PAGE_READWRITE;

	_file.open(path, facc);

	_hMap = CreateFileMappingW(_file.hFile(), nullptr, page, 0, 0, nullptr);
	if (!_hMap) [[unlikely]] {
		throw std::system_error(GetLastError(), std::system_category(), "CreateFileMapping failed.");
	}

	_pMem = MapViewOfFile(_hMap, FILE_MAP_READ | (access == Access::ExistingRW ? FILE_MAP_WRITE : 0), 0, 0, 0);
	if (!_pMem) [[unlikely]] {
		throw std::system_error(GetLastError(), std::system_category(), "MapViewOfFile failed.");
	}

	_sz = _file.size();
	_access = access;
	return *this;
}

std::span<BYTE> FileMapped::view() {
	if (_access != Access::ExistingRW) [[unlikely]] {
		throw std::logic_error("Cannot write to read-only mapped file.");
	}
	return {reinterpret_cast<BYTE*>(_pMem), size()};
}

const BYTE& FileMapped::operator[](size_t index) const {
	return reinterpret_cast<BYTE*>(_pMem)[index];
}

BYTE& FileMapped::operator[](size_t index) {
	if (_access != Access::ExistingRW) [[unlikely]] {
		throw std::logic_error("Cannot write to read-only mapped file.");
	}
	return reinterpret_cast<BYTE*>(_pMem)[index];
}

std::vector<BYTE> FileMapped::read_all(WStrPtr path) {
	const FileMapped f{path, Access::ExistingReadOnly};
	const std::span<BYTE> raw = f.view();
	return {raw.begin(), raw.end()}; // copy all bytes
}
