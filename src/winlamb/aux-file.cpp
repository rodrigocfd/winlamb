#include <system_error>
#include "aux-file.hpp"
using namespace wl;

Time::Time() {
	GetSystemTimeAsFileTime(&_ft);
}

Time::Time(const SYSTEMTIME &st) {
	BOOL ok = SystemTimeToFileTime(&st, &_ft);
	if (!ok) [[unlikely]] {
		throw std::invalid_argument{"Invalid SYSTEMTIME."};
	}
}

FILETIME Time::to_filetime_utc() const {
	FILETIME ft{};
	to_filetime_utc(ft);
	return ft;
}

FILETIME Time::to_filetime_local() const {
	FILETIME ft{};
	to_filetime_local(ft);
	return ft;
}

void Time::to_filetime_local(FILETIME &ftLocal) const {
	BOOL ok = FileTimeToLocalFileTime(&_ft, &ftLocal);
	#ifdef _DEBUG
	if (!ok)
		throw std::system_error(GetLastError(), std::system_category(), "FileTimeToLocalFileTime failed");
	#endif
}

SYSTEMTIME Time::to_systemtime_utc() const {
	SYSTEMTIME st{};
	to_systemtime_utc(st);
	return st;
}

void Time::to_systemtime_utc(SYSTEMTIME &stUtc) const {
	BOOL ok = FileTimeToSystemTime(&_ft, &stUtc);
	#ifdef _DEBUG
	if (!ok)
		throw std::system_error(GetLastError(), std::system_category(), "FileTimeToSystemTime failed");
	#endif
}

SYSTEMTIME Time::to_systemtime_local() const {
	SYSTEMTIME st{};
	to_systemtime_local(st);
	return st;
}

void Time::to_systemtime_local(SYSTEMTIME &stLocal) const {
	SYSTEMTIME stUtc = to_systemtime_utc();
	BOOL ok = SystemTimeToTzSpecificLocalTime(nullptr, &stUtc, &stLocal);
	#ifdef _DEBUG
	if (!ok)
		throw std::system_error(GetLastError(), std::system_category(), "SystemTimeToTzSpecificLocalTime failed");
	#endif
}

std::wstring Time::to_str_local_ymd() const {
	SYSTEMTIME st = to_systemtime_local();
	return str::fmt(L"%04d-%02d-%02d",
		st.wYear, st.wMonth, st.wDay);
}

std::wstring Time::to_str_local_ymd_hm() const {
	SYSTEMTIME st = to_systemtime_local();
	return str::fmt(L"%04d-%02d-%02d %02d:%02d",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);
}

std::wstring Time::to_str_local_ymd_hms() const {
	SYSTEMTIME st = to_systemtime_local();
	return str::fmt(L"%04d-%02d-%02d %02d:%02d:%02d",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
}

////////////////////////////////////////////////////////////////////////////////

File& File::operator=(File &&other) noexcept {
	close();
	std::swap(_hFile, other._hFile);
	return *this;
}

HANDLE File::leak() {
	HANDLE h = _hFile;
	_hFile = nullptr;
	return h;
}

void File::close() noexcept {
	if (_hFile) {
		CloseHandle(_hFile);
		_hFile = nullptr;
	}
}

File& File::open(WStrView filePath, Access access) {
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

	_hFile = CreateFileW(filePath.c_str(), acc, share, nullptr, disp, FILE_ATTRIBUTE_NORMAL, nullptr);
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

std::vector<BYTE> File::read_all() const {
	set_ptr_offset(0);
	return read(size());
}

const File& File::read_buf(std::vector<BYTE> &buf) const {
	DWORD read = 0;
	if (!ReadFile(_hFile, buf.data(), static_cast<DWORD>(buf.size()), &read, nullptr)) [[unlikely]] {
		throw std::system_error(GetLastError(), std::system_category(), "ReadFile failed");
	}
	return *this;
}

Time File::time_creation() const {
	FILETIME ft{};
	BOOL ok = GetFileTime(_hFile, &ft, nullptr, nullptr);
	#ifdef _DEBUG
	if (!ok)
		throw std::system_error(GetLastError(), std::system_category(), "GetFileTime failed");
	#endif
	return Time{ft};
}

Time File::time_last_access() const {
	FILETIME ft{};
	BOOL ok = GetFileTime(_hFile, nullptr, &ft, nullptr);
	#ifdef _DEBUG
	if (!ok)
		throw std::system_error(GetLastError(), std::system_category(), "GetFileTime failed");
	#endif
	return Time{ft};
}

Time File::time_last_write() const {
	FILETIME ft{};
	BOOL ok = GetFileTime(_hFile, nullptr, nullptr, &ft);
	#ifdef _DEBUG
	if (!ok)
		throw std::system_error(GetLastError(), std::system_category(), "GetFileTime failed");
	#endif
	return Time{ft};
}

const File& File::truncate() const {
	set_ptr_offset(0);
	if (!SetEndOfFile(_hFile)) [[unlikely]] {
		throw std::system_error(GetLastError(), std::system_category(), "SetEndOfFile failed");
	}
	return *this;
}

const File& File::write_from_ptr(const BYTE *p, size_t n) const {
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

FileMapped& FileMapped::open(WStrView filePath, Access access) {
	close();
	File::Access facc = (access == Access::existing_read_only) ? // translate FileMap to File access
		File::Access::existing_read_only : File::Access::existing_rw;
	DWORD page = (access == Access::existing_read_only) ? PAGE_READONLY : PAGE_READWRITE;

	_file.open(filePath, facc);

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

////////////////////////////////////////////////////////////////////////////////

bool IniFile::Section::has_val(WStrView key) const {
	return get_val(key) != nullptr;
}

const std::wstring* IniFile::Section::get_val(WStrView key) const {
	for (auto &&keyVal : keysVals) {
		if (str::eq(keyVal.key, key))
			return &keyVal.val;
	}
	return nullptr;
}

void IniFile::Section::set(WStrView key, WStrView val) {
	for (auto &&keyVal : keysVals) {
		if (str::eq(keyVal.key, key)) {
			keyVal.val = val.c_str();
			return;
		}
	}
	keysVals.emplace_back(key.c_str(), val.c_str()); // create new
}

//------------------------------------------------------------------------------

void IniFile::load(WStrView filePath) {
	FileMapped fileMap{filePath, FileMapped::Access::existing_read_only};
	std::wstring contents = str::parse(fileMap.view());
	fileMap.close();

	std::vector<std::wstring> lines = str::split_lines(contents);
	Section curSection{};
	for (auto &&line : lines) {
		str::trim(line);
		if (line.empty() || line[0] == L'#' || line[0] == L';')
			continue; // skip blank and comment

		if (line[0] == L'[' && line.back() == L']') { // [section] ?
			if (!curSection.name.empty())
				sections.emplace_back(std::move(curSection));
			curSection.name = line.substr(1, line.length() - 2); // begin new section
		} else if (!curSection.name.empty()) {
			std::vector<std::wstring> keyVal = str::split_n(line, L"=", 2);
			curSection.keysVals.emplace_back(std::move(keyVal[0]), std::move(keyVal[1])); // new entry
		}
	}

	if (!curSection.name.empty()) // for the last section
		sections.emplace_back(std::move(curSection));

	iniPath = filePath.c_str(); // store path
}

void IniFile::save(const wchar_t *lineBreak) const {
	if (iniPath.empty())
		throw std::invalid_argument{"INI path not defined."};

	size_t szLineBreak = lstrlenW(lineBreak);
	size_t sz = 0;

	for (auto &&sec : sections) { // 1st pass counts size
		sz += sec.name.length() + szLineBreak;
		for (auto &&kv : sec.keysVals)
			sz += kv.key.length() + 1 + kv.val.length() + szLineBreak;
		sz += szLineBreak;
	}

	std::wstring outBuf{};
	outBuf.reserve(sz);

	for (auto &&sec : sections) { // 2nd pass copies to buffer
		outBuf += L'[';
		outBuf += sec.name;
		outBuf += L']';
		outBuf += lineBreak;

		for (auto &&kv : sec.keysVals) {
			outBuf += kv.key;
			outBuf += L'=';
			outBuf += kv.val;
			outBuf += lineBreak;
		}

		outBuf += lineBreak;
	}

	std::vector<BYTE> outBlob = str::to_utf8_blob(outBuf);
	File fout{iniPath, File::Access::open_or_create_rw};
	fout.truncate();
	fout.write(outBlob);
}

bool IniFile::has_section(WStrView sectionName) const {
	return get_section(sectionName) != nullptr;
}

bool IniFile::has_val(WStrView sectionName, WStrView key) const {
	return get_val(sectionName, key) != nullptr;
}

const IniFile::Section* IniFile::get_section(WStrView sectionName) const {
	for (auto &&sec : sections) {
		if (str::eq(sec.name, sectionName))
			return &sec;
	}
	return nullptr;
}

IniFile::Section* IniFile::get_section(WStrView sectionName) {
	for (auto &&sec : sections) {
		if (str::eq(sec.name, sectionName))
			return &sec;
	}
	return nullptr;
}

const std::wstring* IniFile::get_val(WStrView sectionName, WStrView key) const {
	for (auto &&sec : sections) {
		if (str::eq(sec.name, sectionName))
			return sec.get_val(key);
	}
	return nullptr;
}

void IniFile::set_val(WStrView sectionName, WStrView key, WStrView val) {
	for (auto &&sec : sections) {
		if (str::eq(sec.name, sectionName))
			sec.set(key, val);
	}

	sections.emplace_back(); // create new
	sections.back().name = sectionName.c_str();
	sections.back().set(key, val);
}
