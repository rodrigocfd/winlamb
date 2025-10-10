#pragma once
#include "str.h"

namespace wl {

	// Manages a file HANDLE.
	class File final {
	public:
		// Requested access to open/create a file.
		enum class Access { ExistingReadOnly, ExistingRW, OpenOrCreateRW, CreateRW };

		// Returned by times().
		struct Times final {
			SYSTEMTIME creation{}, lastAccess{}, lastWrite{};
		};

		~File() { close(); }

		constexpr File() = default;
		File(const File&) = delete;
		File(File&& other) noexcept { operator=(std::forward<File>(other)); }
		File& operator=(const File&) = delete;
		File& operator=(File&& other) noexcept;

		constexpr explicit File(HANDLE hFile) : _hFile{hFile} { }
		File(WStrPtr path, Access access) { open(path, access); }

		[[nodiscard]] constexpr HANDLE hFile() const { return _hFile; }
		void close() noexcept;
		File& open(WStrPtr path, Access access);
		[[nodiscard]] size_t ptr_offset() const;
		[[nodiscard]] std::vector<BYTE> read(size_t numBytes = 0) const;
		const File& read_buf(std::vector<BYTE>& buf) const;
		const File& set_ptr_offset(size_t offset) const;
		[[nodiscard]] size_t size() const;
		[[nodiscard]] Times times() const;
		const File& truncate() const;
		const File& write(std::span<BYTE> data) const;

		// Static method. Erases the file and writes data immediately.
		static void erase_and_write(WStrPtr path, std::span<BYTE> data);

	private:
		HANDLE _hFile = nullptr;
	};

}

namespace wl {

	// Manages a memory-mapped file.
	class FileMapped final {
	public:
		enum class Access { ExistingReadOnly, ExistingRW };

		~FileMapped() { close(); }

		constexpr FileMapped() = default;
		FileMapped(const FileMapped&) = delete;
		FileMapped(FileMapped&& other) noexcept { operator=(std::forward<FileMapped>(other)); }
		FileMapped& operator=(const FileMapped&) = delete;
		FileMapped& operator=(FileMapped&& other) noexcept;

		FileMapped(WStrPtr path, Access access) { open(path, access); }

		void close() noexcept;
		FileMapped& open(WStrPtr path, Access access);
		[[nodiscard]] constexpr size_t size() const { return _sz; }
		[[nodiscard]] constexpr const std::span<BYTE> view() const { return {reinterpret_cast<BYTE*>(_pMem), size()}; }
		[[nodiscard]] std::span<BYTE> view();

		[[nodiscard]] constexpr const File& file() const { return _file; }
		[[nodiscard]] const BYTE& operator[](size_t index) const;
		[[nodiscard]] BYTE& operator[](size_t index);

		// Static method. Reads all the contents of the file immediately.
		static [[nodiscard]] std::vector<BYTE> read_all(WStrPtr path);

	private:
		File _file{};
		Access _access = Access::ExistingReadOnly;
		HANDLE _hMap = nullptr;
		LPVOID _pMem = nullptr;
		size_t _sz = 0;
	};

}
