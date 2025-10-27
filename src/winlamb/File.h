#pragma once
#include "str.h"

namespace wl {

	/// @brief Manages a file `HANDLE`.
	///
	/// If you only need reading access, consider using `FileMapped`, which tends to be faster.
	///
	/// Example:
	///
	/// ```cpp
	/// wl::File myFile{L"C:\\Temp\\foo.txt", wl::File::Access::existing_read_only};
	/// std::vector<BYTE> contents = myFile.read();
	/// ```
	class File final : NonCopyable {
	public:
		/** @brief Requested access to open/create a file. */
		enum class Access { existing_read_only, existing_rw, open_or_create_rw, create_rw };

		/** @brief Creation, last access and last write file times. */
		struct Times final {
			SYSTEMTIME creation{}, lastAccess{}, lastWrite{};
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

		/// Calls [`CreateFile`] immediately to open the file pointed by `path`, according to the given `access`.
		///
		/// Example:
		///
		/// ```cpp
		/// wl::File myFile{L"C:\\Temp\\foo.txt", wl::File::Access::existing_read_only};
		/// ```
		///
		/// [`CreateFile`]: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew
		File(WStrPtr path, Access access) { open(path, access); }

		/// Move-assignment operator.
		///
		/// Calls [`CloseHandle`] on the current file handle and takes ownership of `other`, so no leaks happen.
		///
		/// [`CloseHandle`]: https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle
		File& operator=(File &&other) noexcept;

		/** Returns the wrapped file handle. */
		[[nodiscard]] constexpr HANDLE hfile() const { return _hFile; }

		/// Calls [`CloseHandle`] immediately.
		///
		/// This method is automatically called by the destructor.
		///
		/// [`CloseHandle`]: https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle
		void close() noexcept;

		/// Calls [`CreateFile`] to open the file pointed by `path`, according to the given `access`.
		///
		/// Example:
		///
		/// ```cpp
		/// wl::File myFile{};
		/// myFile.open(L"C:\\Temp\\foo.txt", wl::File::Access::existing_read_only);
		/// ```
		///
		/// [`CreateFile`]: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew
		File& open(WStrPtr path, Access access);

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

		/// Calls [`ReadFile`] and returns a vector with the file contents, up to `numBytes`.
		///
		/// Example:
		///
		/// ```cpp
		/// wl::File myFile{L"C:\\Temp\\foo.txt", wl::File::Access::existing_read_only};
		/// std::vector<BYTE> contents = myFile.read();
		/// ```
		///
		/// [`ReadFile`]: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfile
		[[nodiscard]] std::vector<BYTE> read(size_t numBytes = 0) const;

		/// Calls [`ReadFile`] and copies the file contents into `buf`, up to `buf.size()`.
		///
		/// Example:
		///
		/// ```cpp
		/// wl::File myFile{L"C:\\Temp\\foo.txt", wl::File::Access::existing_read_only};
		/// std::vector<BYTE> buf(100, 0x00);
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
		const File& write(R &&contents) const { return write_ptr(contents.data(), contents.size()); }

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
		const File& write(It first, End last) const { return write_ptr(&*first, std::distance(first, last)); }

		/// Calls [`GetFileTime`] and returns creation, last access and last write times.
		///
		/// [`GetFileTime`]: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getfiletime
		[[nodiscard]] Times times() const;

		/// Calls [`SetEndOfFile`] to set the file size to zero, erasing all its contents.
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
		HANDLE _hFile = nullptr;
		const File& write_ptr(const BYTE *p, size_t n) const;
	};

}

namespace wl {

	/// @brief Manages a memory-mapped file.
	///
	/// If you need to modify the file size, use `File` instead.
	///
	/// Example:
	///
	/// ```cpp
	/// wl::FileMapped myFile{L"C:\\Temp\\foo.txt", wl::FileMapped::Access::existing_read_only};
	/// std::span<BYTE> myView = myFile.view();
	/// ```
	class FileMapped final : NonCopyable {
	public:
		/** @brief Requested access to open a file. */
		enum class Access { existing_read_only, existing_rw };

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

		/// Calls [`CreateFile`], [`CreateFileMapping`] and [`MapViewOfFile`] immediately.
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
		FileMapped(WStrPtr path, Access access) { open(path, access); }

		/// Move-assignment operator.
		///
		/// Calls [`UnmapViewOfFile`] and [`CloseHandle`] on the current file handle and takes ownership of `other`, so no leaks happen.
		///
		/// [`UnmapViewOfFile`]: https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-unmapviewoffile
		/// [`CloseHandle`]: https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle
		FileMapped& operator=(FileMapped &&other) noexcept;

		/** Retrieves a reference to the byte at the given index. */
		[[nodiscard]] BYTE& operator[](size_t index) const { return reinterpret_cast<BYTE*>(_pMem)[index]; }

		/// Calls [`UnmapViewOfFile`] and [`CloseHandle`] immediately.
		///
		/// This method is automatically called by the destructor.
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
		FileMapped& open(WStrPtr path, Access access);

		/** Returns the file size. This value is cached. */
		[[nodiscard]] constexpr size_t size() const { return _sz; }

		/// Returns a view over the file contents.
		///
		/// Example:
		///
		/// ```cpp
		/// wl::FileMapped myFile{L"C:\\Temp\\foo.txt", wl::FileMapped::Access::existing_read_only};
		/// std::span<BYTE> myView = myFile.view();
		/// ```
		[[nodiscard]] constexpr std::span<BYTE> view() const { return {reinterpret_cast<BYTE*>(_pMem), size()}; }

		/** Returns the underlying `File` object. */
		[[nodiscard]] constexpr const File& file() const { return _file; }

	private:
		File _file{};
		Access _access = Access::existing_read_only;
		HANDLE _hMap = nullptr;
		LPVOID _pMem = nullptr;
		size_t _sz = 0;
	};

}
