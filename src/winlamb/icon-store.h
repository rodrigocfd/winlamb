#pragma once
#include <vector>
#include "lib-include-win.h"
#include <CommCtrl.h>
#include "str.h"

namespace wl {

	/** @brief Pure abstract class; implemented by icon stores. */
	class IconStore {
	public:
		/** Adds the `HICON` to the store, taking ownership of it. */
		virtual void add_icon(HICON hIcon) = 0;

		/// Calls [`LoadImage`] to load an icon from the resource.
		///
		/// [`LoadImage`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-loadimagew
		virtual void add_resource(WORD iconId) = 0;

		/// Calls [`SHGetFileInfo`] to load icons from the shell.
		/// These are the icons used by Windows Explorer to represent the given file extensions, like "mp3".
		///
		/// [`SHGetFileInfo`]: https://learn.microsoft.com/en-us/windows/win32/api/shellapi/nf-shellapi-shgetfileinfow
		virtual void add_shell_ext(WStrView fileExt) = 0;
	};

}

namespace _wl_internal {

	/** Manages a `HIMAGELIST`. */
	class ImageList final : public wl::IconStore {
	public:
		~ImageList();
		constexpr explicit ImageList(SIZE szIcon) : _szIcon{szIcon} { }

		void create(UINT ilcFlags = ILC_COLOR32);
		[[nodiscard]] size_t count() const;
		constexpr HIMAGELIST himagelist() const { return _hImageList; }

		void add_icon(HICON hIcon) override;
		void add_resource(WORD iconId) override;
		void add_shell_ext(wl::WStrView fileExt) override;

	private:
		HIMAGELIST _hImageList = nullptr;
		SIZE _szIcon;
	};

	/** Stores `HICON` handles. */
	class HIconStore final : public wl::IconStore {
	public:
		~HIconStore();
		constexpr explicit HIconStore(SIZE szIcon) : _szIcon{szIcon} { }

		[[nodiscard]] constexpr size_t count() const { return _hIcons.size(); }
		[[nodiscard]] constexpr HICON get(size_t index) const { return _hIcons[index]; }

		void add_icon(HICON hIcon) override;
		void add_resource(WORD iconId) override;
		void add_shell_ext(wl::WStrView fileExt) override;

	private:
		std::vector<HICON> _hIcons{};
		SIZE _szIcon;
	};

}
