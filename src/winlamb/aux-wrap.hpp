#pragma once
#include "aux-str.hpp"

namespace wl {

	/// @brief A base class which makes derived classes non-copyable an
	/// non-movable.
	///
	/// ```cpp
	/// class Person final : private wl::NoCopyNoMove {
	///     std::wstring name{};
	/// };
	/// ```
	class NoCopyNoMove {
	public:
		constexpr NoCopyNoMove() = default;
		NoCopyNoMove(const NoCopyNoMove&) = delete;
		NoCopyNoMove(NoCopyNoMove&&) = delete;
		NoCopyNoMove& operator=(const NoCopyNoMove&&) = delete;
		NoCopyNoMove& operator=(NoCopyNoMove&&) = delete;
	};

	/// @brief Wraps a [`FILETIME`] struct, providing `FILETIME` and
	/// [`SYSTEMTIME`] operations.
	///
	/// Stored internally as [UTC time].
	///
	/// [`FILETIME`]: https://learn.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-filetime
	/// [`SYSTEMTIME`]: https://learn.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-systemtime
	/// [UTC time]: https://en.wikipedia.org/wiki/Coordinated_Universal_Time
	class Time final {
	public:
		/// Default constructor.
		///
		/// Calls [`GetSystemTimeAsFileTime`] and stores the current system time
		/// as UTC.
		///
		/// [`GetSystemTimeAsFileTime`]: https://learn.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getsystemtimeasfiletime
		Time();

		/// Constructor.
		///
		/// Assumes `ft` as UTC time.
		constexpr explicit Time(FILETIME ft) : _ft{ft} { }

		/// Constructor.
		///
		/// Assumes `st` as UTC time.
		explicit Time(const SYSTEMTIME &st);

		/** Comparison operator. */
		[[nodiscard]] constexpr std::strong_ordering operator<=>(const Time &other) const {
			if (nano100() > other.nano100()) return std::strong_ordering::greater;
			if (nano100() < other.nano100()) return std::strong_ordering::less;
			return std::strong_ordering::equal;
		}

		/** Comparison operator. */
		[[nodiscard]] constexpr bool operator==(const Time &other) const {
			return _ft.dwHighDateTime == other._ft.dwHighDateTime && _ft.dwLowDateTime == other._ft.dwLowDateTime;
		}

		/** Comparison operator. */
		[[nodiscard]] constexpr bool operator>(const Time &other) const {
			return operator<=>(other) == std::strong_ordering::greater;
		}

		/** Comparison operator. */
		[[nodiscard]] constexpr bool operator<(const Time &other) const {
			return operator<=>(other) == std::strong_ordering::less;
		}

		/// Returns the 64-bit value representing the number of 100-nanosecond
		/// intervals since January 1, 1601 (UTC).
		///
		/// This is the native representation of the `FILETIME` struct.
		[[nodiscard]] constexpr ULONGLONG nano100() const {
			return (static_cast<ULONGLONG>(_ft.dwHighDateTime) << 32) | (_ft.dwLowDateTime & 0xffff'ffff);
		}

		/// Sets the 64-bit value representing the number of 100-nanosecond
		/// intervals since January 1, 1601 (UTC).
		///
		/// This is the native representation of the `FILETIME` struct.
		constexpr void set_nano100(ULONGLONG nanoseconds) {
			_ft.dwLowDateTime = static_cast<DWORD>(nanoseconds & 0xffff'ffff);
			_ft.dwHighDateTime = static_cast<DWORD>(nanoseconds >> 32);
		}

		/** Adds or subtracts the given milliseconds to the stored `FILETIME`. */
		constexpr void add_ms(ULONGLONG milliseconds) { set_nano100(nano100() + milliseconds * 10'000); }

		/** Adds or subtracts the given seconds to the stored `FILETIME`. */
		constexpr void add_secs(ULONGLONG seconds) { add_ms(seconds * 1000); }

		/** Adds or subtracts the given minutes to the stored `FILETIME`. */
		constexpr void add_mins(ULONGLONG minutes) { add_secs(minutes * 60); }

		/** Adds or subtracts the given hours to the stored `FILETIME`. */
		constexpr void add_hours(ULONGLONG hours) { add_mins(hours * 60); }

		/** Adds or subtracts the given days to the stored `FILETIME`. */
		constexpr void add_days(ULONGLONG days) { add_hours(days * 24); }

		/** Adds or subtracts the given weeks to the stored `FILETIME`. */
		constexpr void add_weeks(ULONGLONG weeks) { add_days(weeks * 7); }

		/// Returns the difference to `other` time, in milliseconds.
		///
		/// Note that, if `other` is more recent, the difference will be negative.
		[[nodiscard]] constexpr LONGLONG diff_ms(const Time &other) const {
			return (static_cast<LONGLONG>(nano100()) - static_cast<LONGLONG>(other.nano100())) / 10'000;
		}

		/** Returns the stored time as `FILETIME`, UTC. */
		[[nodiscard]] FILETIME to_filetime_utc() const;

		/** Returns the stored time as `FILETIME`, UTC. */
		constexpr void to_filetime_utc(FILETIME &ftUtc) const { ftUtc = _ft; }

		/** Returns the stored time as `FILETIME`, local. */
		[[nodiscard]] FILETIME to_filetime_local() const;

		/** Returns the stored time as `FILETIME`, local. */
		void to_filetime_local(FILETIME &ftLocal) const;

		/** Returns the stored time as `SYSTEMTIME`, UTC. */
		[[nodiscard]] SYSTEMTIME to_systemtime_utc() const;

		/** Returns the stored time as `SYSTEMTIME`, UTC. */
		void to_systemtime_utc(SYSTEMTIME &stUtc) const;

		/** Returns the stored time as `SYSTEMTIME`, local. */
		[[nodiscard]] SYSTEMTIME to_systemtime_local() const;

		/** Returns the stored time as `SYSTEMTIME`, local. */
		void to_systemtime_local(SYSTEMTIME &stLocal) const;

		/** Returns the stored local date formatted as `YYYY-MM-DD`. */
		[[nodiscard]] std::wstring to_str_local_ymd() const;

		/** Returns the stored local date formatted as `YYYY-MM-DD hh:mm`. */
		[[nodiscard]] std::wstring to_str_local_ymd_hm() const;

		/** Returns the stored local date formatted as `YYYY-MM-DD hh:mm:ss`. */
		[[nodiscard]] std::wstring to_str_local_ymd_hms() const;

	private:
		FILETIME _ft;
	};

	/// @brief Manages a file `HANDLE`.
	///
	/// If you only need reading access, consider using `FileMapped`, which tends
	/// to be faster.
	///
	/// Example:
	///
	/// ```cpp
	/// wl::File myFile{L"C:\\Temp\\foo.txt", wl::File::Access::existing_read_only};
	/// std::vector<BYTE> contents = myFile.read_all();
	/// std::wstring text = wl::str::parse(contents);
	/// ```
	class File final {
	public:
		/** @brief Requested access to open/create a file. */
		enum class Access {
			/** Opens the file as read-only, fails if the file doesn't exist. */
			existing_read_only,
			/** Opens the file as read-write, fails if the file doesn't exist. */
			existing_rw,
			/** Opens the file as read-write, creates the file if it doesn't exist. */
			open_or_create_rw,
			/** Creates the file as read-write, fails if the file already exists. */
			create_rw,
		};

		/// Destructor.
		///
		/// Calls [`CloseHandle`].
		///
		/// [`CloseHandle`]: https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle
		~File() { close(); }

		/// Default constructor.
		///
		/// Initializes the file handle to `nullptr`.
		constexpr File() = default;

		/// Move-constructor.
		///
		/// Takes ownership of `other`, so no leaks happen.
		File(File &&other) noexcept { operator=(std::forward<File>(other)); }

		/// Constructs `File` by wrapping `hFile`.
		///
		/// Ideally, you should never need this.
		constexpr explicit File(HANDLE hFile) : _hFile{hFile} { }

		/// Calls [`CreateFile`] immediately to open the file pointed by
		/// `filePath`, according to the given `access`.
		///
		/// Example:
		///
		/// ```cpp
		/// wl::File myFile{L"C:\\Temp\\foo.txt", wl::File::Access::existing_read_only};
		/// ```
		///
		/// [`CreateFile`]: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew
		File(WStrView filePath, Access access) { open(filePath, access); }

		/// Move-assignment operator.
		///
		/// Calls [`CloseHandle`] on the current file handle and takes ownership
		/// of `other`, so no leaks happen.
		///
		/// [`CloseHandle`]: https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle
		File& operator=(File &&other) noexcept;

		/** Returns the wrapped file handle. */
		[[nodiscard]] constexpr HANDLE hfile() const { return _hFile; }

		/// Returns the wrapped `HANDLE`, setting the current `HANDLE` to
		/// `nullptr`, so that `close` won't be called.
		///
		/// It's your responsability to close the returned `HANDLE`, or a resource
		/// leak will occur.
		///
		/// Example:
		///
		/// ```cpp
		/// wl::File f{L"C:\\Temp\\foo.txt", wl::File::Access::existing_read_only};
		/// HANDLE pLeaked = f.leak();
		///
		/// wl::File f2{pLeaked}; // take ownership again
		/// ```
		[[nodiscard]] HANDLE leak();

		/// Calls [`CloseHandle`] immediately.
		///
		/// This method is automatically called by the `~File` destructor.
		///
		/// [`CloseHandle`]: https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle
		void close() noexcept;

		/// Calls [`CreateFile`] to open the file pointed by `filePath`, according
		/// to the given `access`.
		///
		/// Example:
		///
		/// ```cpp
		/// wl::File myFile{};
		/// myFile.open(L"C:\\Temp\\foo.txt", wl::File::Access::existing_read_only);
		/// ```
		///
		/// [`CreateFile`]: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew
		File& open(WStrView filePath, Access access);

		/// Returns the current file size by calling [`GetFileSizeEx`].
		///
		/// [`GetFileSizeEx`]: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getfilesizeex
		[[nodiscard]] size_t size() const;

		/// Retrieves current file pointer offset by calling [`SetFilePointerEx`].
		///
		/// [`SetFilePointerEx`]: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-setfilepointerex
		[[nodiscard]] size_t ptr_offset() const;

		/// Sets the current file pointer offset by calling [`SetFilePointerEx`].
		///
		/// [`SetFilePointerEx`]: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-setfilepointerex
		const File& set_ptr_offset(size_t offset) const;

		/// Calls [`ReadFile`] and returns a [`std::vector`] with the file
		/// contents, up to `numBytes`.
		///
		/// Example:
		///
		/// ```cpp
		/// wl::File myFile{L"C:\\Temp\\foo.txt", wl::File::Access::existing_read_only};
		/// std::vector<BYTE> contents = myFile.read(40);
		/// ```
		///
		/// [`ReadFile`]: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfile
		/// [`std::vector`]: https://en.cppreference.com/w/cpp/container/vector.html
		[[nodiscard]] std::vector<BYTE> read(size_t numBytes) const;

		/// Calls [`SetFilePointerEx`] to rewind the current file pointer, then
		/// [`ReadFile`] to return a [`std::vector`] with the entire file.
		///
		/// Example:
		///
		/// ```cpp
		/// wl::File myFile{L"C:\\Temp\\foo.txt", wl::File::Access::existing_read_only};
		/// std::vector<BYTE> contents = myFile.read_all();
		/// ```
		///
		/// [`SetFilePointerEx`]: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-setfilepointerex
		/// [`ReadFile`]: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfile
		/// [`std::vector`]: https://en.cppreference.com/w/cpp/container/vector.html
		[[nodiscard]] std::vector<BYTE> read_all() const;

		/// Calls [`ReadFile`] and copies the file contents into `buf`, up to
		/// `buf.size()`.
		///
		/// Example:
		///
		/// ```cpp
		/// wl::File myFile{L"C:\\Temp\\foo.txt", wl::File::Access::existing_read_only};
		/// std::vector<BYTE> buf(40, 0x00);
		/// myFile.read_buf(buf);
		/// ```
		///
		/// [`ReadFile`]: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfile
		const File& read_buf(std::vector<BYTE> &buf) const;

		/// Calls [`WriteFile`] to write data to the file.
		///
		/// Example:
		///
		/// ```cpp
		/// wl::File myFile{L"C:\\Temp\\foo.txt", wl::File::Access::open_or_create_rw};
		/// myFile.truncate();
		///
		/// std::wstring text{L"content text"};
		/// std::vector<BYTE> bytes = wl::str::to_utf8_blob(text);
		/// myFile.write(bytes);
		/// ```
		///
		/// [`WriteFile`]: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-writefile
		template<std::ranges::contiguous_range R>
			requires std::ranges::sized_range<R>
		const File& write(R &&contents) const { return write_from_ptr(contents.data(), contents.size()); }

		/// Calls [`WriteFile`] to write data to the file.
		///
		/// Example:
		///
		/// ```cpp
		/// wl::File myFile{L"C:\\Temp\\foo.txt", wl::File::Access::open_or_create_rw};
		/// myFile.truncate();
		///
		/// std::wstring text{L"content text"};
		/// std::vector<BYTE> bytes = wl::str::to_utf8_blob(text);
		/// myFile.write(bytes.begin(), bytes.end());
		/// ```
		///
		/// [`WriteFile`]: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-writefile
		template<std::input_iterator It, std::sentinel_for<It> End>
		const File& write(It first, End last) const { return write_from_ptr(&*first, std::distance(first, last)); }

		/// Calls [`GetFileTime`] and returns creation time.
		///
		/// [`GetFileTime`]: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getfiletime
		[[nodiscard]] Time time_creation() const;

		/// Calls [`GetFileTime`] and returns last access time.
		///
		/// [`GetFileTime`]: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getfiletime
		[[nodiscard]] Time time_last_access() const;

		/// Calls [`GetFileTime`] and returns last write time.
		///
		/// [`GetFileTime`]: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getfiletime
		[[nodiscard]] Time time_last_write() const;

		/// Calls [`SetEndOfFile`] to set the file size to zero, erasing all its
		/// contents.
		///
		/// Example:
		///
		/// ```cpp
		/// wl::File myFile{L"C:\\Temp\\foo.txt", wl::File::Access::open_or_create_rw};
		/// myFile.truncate();
		/// ```
		///
		/// [`SetEndOfFile`]: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-setendoffile
		const File& truncate() const;

	private:
		const File& write_from_ptr(const BYTE *p, size_t n) const;
		HANDLE _hFile = nullptr;
	};

	/// @brief Manages a [memory-mapped file].
	///
	/// If you need to modify the file size, use `File` instead.
	///
	/// Example:
	///
	/// ```cpp
	/// wl::FileMapped myFile{L"C:\\Temp\\foo.txt", wl::FileMapped::Access::existing_read_only};
	/// std::span<BYTE> myView = myFile.view();
	/// std::wstring text = wl::str::parse(myView);
	/// ```
	///
	/// [memory-mapped file]: https://learn.microsoft.com/en-us/windows/win32/memory/file-mapping
	class FileMapped final {
	public:
		/** @brief Requested access to open a file. */
		enum class Access {
			/** Opens the file as read-only. */
			existing_read_only,
			/** Opens the file as read-write. */
			existing_rw,
		};

	public:
		/// Destructor.
		///
		/// Calls [`UnmapViewOfFile`] and [`CloseHandle`].
		///
		/// [`UnmapViewOfFile`]: https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-unmapviewoffile
		/// [`CloseHandle`]: https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle
		~FileMapped() { close(); }

		/// Default constructor.
		///
		/// Initializes the file handle to `nullptr`.
		constexpr FileMapped() = default;

		/// Move-constructor.
		///
		/// Takes ownership of `other`, so no leaks happen.
		FileMapped(FileMapped &&other) noexcept { operator=(std::forward<FileMapped>(other)); }

		/// Calls [`CreateFile`], [`CreateFileMapping`] and [`MapViewOfFile`]
		/// immediately.
		///
		/// Example:
		///
		/// ```cpp
		/// wl::FileMapped myFile{L"C:\\Temp\\foo.txt", wl::FileMapped::Access::existing_read_only};
		/// ```
		///
		/// [`CreateFile`]: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew
		/// [`CreateFileMapping`]: https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-createfilemappingw
		/// [`MapViewOfFile`]: https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-mapviewoffile
		FileMapped(WStrView filePath, Access access) { open(filePath, access); }

		/// Move-assignment operator.
		///
		/// Calls [`UnmapViewOfFile`] and [`CloseHandle`] on the current file
		/// handle and takes ownership of `other`, so no leaks happen.
		///
		/// [`UnmapViewOfFile`]: https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-unmapviewoffile
		/// [`CloseHandle`]: https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle
		FileMapped& operator=(FileMapped &&other) noexcept;

		/** Retrieves a reference to the byte at the given index. */
		[[nodiscard]] BYTE& operator[](size_t index) const { return reinterpret_cast<BYTE*>(_pMem)[index]; }

		/// Calls [`UnmapViewOfFile`] and [`CloseHandle`] immediately.
		///
		/// This method is automatically called by the `~FileMapped` destructor.
		///
		/// [`UnmapViewOfFile`]: https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-unmapviewoffile
		/// [`CloseHandle`]: https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle
		void close() noexcept;

		/// Calls [`CreateFile`], [`CreateFileMapping`] and [`MapViewOfFile`].
		///
		/// Example:
		///
		/// ```cpp
		/// wl::FileMapped myFile{};
		/// myFile.open(L"C:\\Temp\\foo.txt", wl::FileMapped::Access::existing_read_only);
		/// ```
		///
		/// [`CreateFile`]: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew
		/// [`CreateFileMapping`]: https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-createfilemappingw
		/// [`MapViewOfFile`]: https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-mapviewoffile
		FileMapped& open(WStrView filePath, Access access);

		/** Returns the file size. This value is cached. */
		[[nodiscard]] constexpr size_t size() const { return _sz; }

		/// Returns a [`std::span`] over the memory-mapped file contents.
		///
		/// Example:
		///
		/// ```cpp
		/// wl::FileMapped myFile{L"C:\\Temp\\foo.txt", wl::FileMapped::Access::existing_read_only};
		/// std::span<BYTE> myView = myFile.view();
		/// ```
		///
		/// [`std::span`]: https://en.cppreference.com/w/cpp/container/span.html
		[[nodiscard]] constexpr std::span<BYTE> view() const { return {reinterpret_cast<BYTE*>(_pMem), size()}; }

	private:
		File _file{};
		Access _access = Access::existing_read_only;
		HANDLE _hMap = nullptr;
		LPVOID _pMem = nullptr;
		size_t _sz = 0;
	};

	/// @brief Manages an INI file.
	///
	/// Example:
	///
	/// ```cpp
	/// wl::IniFile ini{L"C:\\Temp\\foo.ini"};
	/// ini.set(L"MySection", L"MyKey", L"foo");
	/// ini.save();
	/// ```
	class IniFile final {
	public:
		/** Stores the key/value pair of an entry. */
		struct KeyVal final {
			/** Key name of the entry. */
			std::wstring key{};

			/** Value of the entry. */
			std::wstring val{};
		};

		/** Stores a section of the INI file. */
		struct Section final {
			/** Name of the section. */
			std::wstring name{};

			/** All key/value pairs of the section. */
			std::vector<KeyVal> keysVals{};

			/** Returns `true` if the given `KeyVal` exists. */
			[[nodiscard]] bool has_val(WStrView key) const;

			/** Retrieves the `KeyVal` with the given `key`, or `nullptr` if not present. */
			[[nodiscard]] const std::wstring* get_val(WStrView key) const;

			/** Directly sets the `KeyVal` value. If `key` doesn't exist, creates it. */
			void set(WStrView key, WStrView val);
		};

		/** Sections of the INI file. */
		std::vector<Section> sections{};

		/** File path of this INI file. Used when you call `save`. */
		std::wstring iniPath{};

		/** Default constructor. */
		constexpr IniFile() = default;

		/// Constructor.
		///
		/// Loads the INI file at `filePath`.
		explicit IniFile(WStrView filePath) { load(filePath); }

		/** Loads the INI file at `filePath`. */
		void load(WStrView filePath);

		/// Saves the INI file to `iniPath`.
		///
		/// If you want to save the INI in a different file, simply change the
		/// `iniPath` value.
		///
		/// Throws an exception if `iniPath` is empty.
		void save(const wchar_t *lineBreak = L"\r\n") const;

		/** Returns `true` if the given `Section` exists. */
		[[nodiscard]] bool has_section(WStrView sectionName) const;

		/** Returns `true` if the given `KeyVal` exists. */
		[[nodiscard]] bool has_val(WStrView sectionName, WStrView key) const;

		/** Returns the given `Section`, or `nullptr` if not present. */
		[[nodiscard]] const Section* get_section(WStrView sectionName) const;

		/** Returns the given `Section`, or `nullptr` if not present. */
		[[nodiscard]] Section* get_section(WStrView sectionName);

		/// Returns the `KeyVal` at the given `section` with the given `key`, or
		/// `nullptr if not present.`
		[[nodiscard]] const std::wstring* get_val(WStrView sectionName, WStrView key) const;

		/// Directly sets the `KeyVal` value. If `section` and/or `key` don't
		/// exist, creates them.
		void set_val(WStrView sectionName, WStrView key, WStrView val);
	};
}
