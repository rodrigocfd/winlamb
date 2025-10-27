#include <system_error>
#include "file.h"
using namespace wl;

File& File::operator=(File &&other) noexcept {
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
	case Access::existing_read_only:
		acc = GENERIC_READ;
		share = FILE_SHARE_READ;
		disp = OPEN_EXISTING;
		break;
	case Access::existing_rw:
		acc = GENERIC_READ | GENERIC_WRITE;
		disp = OPEN_EXISTING;
		break;
	case Access::open_or_create_rw:
		acc = GENERIC_READ | GENERIC_WRITE;
		disp = OPEN_ALWAYS;
		break;
	case Access::create_rw:
		acc = GENERIC_READ | GENERIC_WRITE;
		disp = CREATE_NEW;
	}

	_hFile = CreateFileW(path, acc, share, nullptr, disp, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (!_hFile || _hFile == INVALID_HANDLE_VALUE) [[unlikely]] {
		throw std::system_error(GetLastError(), std::system_category(), "CreateFile failed");
	}
	return *this;
}

size_t File::size() const {
	LARGE_INTEGER sz{};
	if (!GetFileSizeEx(_hFile, &sz)) [[unlikely]] {
		throw std::system_error(GetLastError(), std::system_category(), "GetFileSizeEx failed");
	}
	return static_cast<size_t>(sz.QuadPart);
}

size_t File::ptr_offset() const {
	LARGE_INTEGER zeroOffset{}, curOffset{};
	if (!SetFilePointerEx(_hFile, zeroOffset, &curOffset, FILE_CURRENT)) [[unlikely]] {
		throw std::system_error(GetLastError(), std::system_category(), "SetFilePointerEx failed");
	}
	return static_cast<size_t>(curOffset.QuadPart);
}

const File& File::set_ptr_offset(size_t offset) const {
	LARGE_INTEGER off{.QuadPart = static_cast<LONGLONG>(offset)};
	if (!SetFilePointerEx(_hFile, off, nullptr, FILE_BEGIN)) [[unlikely]] {
		throw std::system_error(GetLastError(), std::system_category(), "SetFilePointerEx failed");
	}
	return *this;
}

std::vector<BYTE> File::read(size_t numBytes) const {
	std::vector<BYTE> buf(numBytes ? numBytes : size(), 0x00);
	read_buf(buf);
	return buf;
}

const File& File::read_buf(std::vector<BYTE> &buf) const {
	DWORD read = 0;
	if (!ReadFile(_hFile, buf.data(), static_cast<DWORD>(buf.size()), &read, nullptr)) [[unlikely]] {
		throw std::system_error(GetLastError(), std::system_category(), "ReadFile failed");
	}
	return *this;
}

File::Times File::times() const {
	FILETIME ftCreation{}, ftLastAccess{}, ftLastWrite{};
	if (!GetFileTime(_hFile, &ftCreation, &ftLastAccess, &ftLastWrite)) [[unlikely]] {
		throw std::system_error(GetLastError(), std::system_category(), "GetFileTime failed");
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
		throw std::system_error(GetLastError(), std::system_category(), "SetEndOfFile failed");
	}
	return *this;
}

const File& File::write_ptr(const BYTE *p, size_t n) const {
	DWORD written = 0;
	if (!WriteFile(_hFile, p, static_cast<DWORD>(n), &written, nullptr)) [[unlikely]] {
		throw std::system_error(GetLastError(), std::system_category(), "WriteFile failed");
	}
	return *this;
}

////////////////////////////////////////////////////////////////////////////////

FileMapped& FileMapped::operator=(FileMapped &&other) noexcept {
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
	File::Access facc = (access == Access::existing_read_only) ? // translate FileMap to File access
		File::Access::existing_read_only : File::Access::existing_rw;
	DWORD page = (access == Access::existing_read_only) ? PAGE_READONLY : PAGE_READWRITE;

	_file.open(path, facc);

	_hMap = CreateFileMappingW(_file.hfile(), nullptr, page, 0, 0, nullptr);
	if (!_hMap) [[unlikely]] {
		throw std::system_error(GetLastError(), std::system_category(), "CreateFileMapping failed");
	}

	_pMem = MapViewOfFile(_hMap, FILE_MAP_READ | (access == Access::existing_rw ? FILE_MAP_WRITE : 0), 0, 0, 0);
	if (!_pMem) [[unlikely]] {
		throw std::system_error(GetLastError(), std::system_category(), "MapViewOfFile failed");
	}

	_sz = _file.size();
	_access = access;
	return *this;
}
