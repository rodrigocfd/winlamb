#pragma once
#include "lib-include-win.h"
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

		/// Calls [`CreateFile`] immediately to open the file pointed by `filePath`, according to the given `access`.
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
		/// Calls [`CloseHandle`] on the current file handle and takes ownership of `other`, so no leaks happen.
		///
		/// [`CloseHandle`]: https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle
		File& operator=(File &&other) noexcept;

		/** Returns the wrapped file handle. */
		[[nodiscard]] constexpr HANDLE hfile() const { return _hFile; }

		/// Returns the wrapped `HANDLE`, setting the current `HANDLE` to `nullptr`, so that `close` won't be called.
		///
		/// It's your responsability to close the returned `HANDLE`, or a resource leak will occur.
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

		/// Calls [`CreateFile`] to open the file pointed by `filePath`, according to the given `access`.
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

		/// Calls [`ReadFile`] and returns a [`std::vector`] with the file contents, up to `numBytes`.
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

		/// Calls [`SetFilePointerEx`] to rewind the current file pointer, then [`ReadFile`] to return a [`std::vector`] with the entire file.
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

		/// Calls [`ReadFile`] and copies the file contents into `buf`, up to `buf.size()`.
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
		const File& write_from_ptr(const BYTE *p, size_t n) const;
		HANDLE _hFile = nullptr;
	};

	/// @brief Manages a memory-mapped file.
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
		FileMapped(WStrView filePath, Access access) { open(filePath, access); }

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

}
