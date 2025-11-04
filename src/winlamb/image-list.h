#pragma once
#include "str.h"
#include <CommCtrl.h>

namespace wl {

	/** @brief Manages a `HIMAGELIST`. */
	class ImageList final : NonCopyable {
	public:
		~ImageList() { destroy(); }

		/// Default constructor.
		///
		/// Initializes the imag list handle to `nullptr`.
		constexpr ImageList() = default;

		/// Move-constructor.
		///
		/// Takes ownership of `other`, so no leaks happen.
		ImageList(ImageList &&other) noexcept { operator=(std::forward<ImageList>(other)); }

		/// Constructs `ImageList` by wrapping `hImageList`.
		///
		/// Ideally, you should never need this.
		constexpr explicit ImageList(HIMAGELIST hImageList) : _hImageList{hImageList} { }

		/// Calls [`ImageList_Create`] immediately to create the image list.
		///
		/// Example:
		///
		/// ```cpp
		/// wl::ImageList imageList{16, 16, ILC_COLOR32};
		/// ```
		///
		/// [`ImageList_Create`]: https://learn.microsoft.com/en-us/windows/win32/api/commctrl/nf-commctrl-imagelist_create
		ImageList(int cx, int cy, UINT ilcFlags, size_t initialCount = 0, size_t growCount = 0) { create(cx, cy, ilcFlags, initialCount, growCount); }

		/// Move-assignment operator.
		///
		/// Calls [`ImageList_Destroy`] on the current image list handle and takes ownership of `other`, so no leaks happen.
		///
		/// [`ImageList_Destroy`]: https://learn.microsoft.com/en-us/windows/win32/api/commctrl/nf-commctrl-imagelist_destroy
		ImageList& operator=(ImageList &&other) noexcept;

		/** Returns the wrapped image list handle. */
		[[nodiscard]] constexpr HIMAGELIST himagelist() const { return _hImageList; }

		/// Returns the wrapped `HIMAGELIST`, setting the current `HIMAGELIST` to `nullptr`, so that `close` won't be called.
		///
		/// It's your responsability to close the returned `HIMAGELIST`, or a resource leak will occur.
		///
		/// Example:
		///
		/// ```cpp
		/// wl::ImageList imageList{16, 16, ILC_COLOR32};
		/// HIMAGELIST pLeaked = imageList.leak();
		///
		/// wl::ImageList imageList2{pLeaked}; // take ownership again
		/// ```
		[[nodiscard]] HIMAGELIST leak();

		/// Calls [`ImageList_Destroy`] immediately.
		///
		/// [`ImageList_Destroy`]: https://learn.microsoft.com/en-us/windows/win32/api/commctrl/nf-commctrl-imagelist_destroy
		void destroy() noexcept;

		/// Calls [`ImageList_Create`] to create the image list.
		///
		/// Example:
		///
		/// ```cpp
		/// wl::ImageList imageList{};
		/// imageList.create(16, 16, ILC_COLOR32);
		/// ```
		///
		/// [`ImageList_Create`]: https://learn.microsoft.com/en-us/windows/win32/api/commctrl/nf-commctrl-imagelist_create
		void create(int cx, int cy, UINT ilcFlags, size_t initialCount = 0, size_t growCount = 0);

		/// Calls [`ImageList_Add`] to add a copy of `hBmp` into the image list.
		///
		/// [`ImageList_Add`]: https://learn.microsoft.com/en-us/windows/win32/api/commctrl/nf-commctrl-imagelist_add
		const ImageList& add_bitmap(HBITMAP hBmp, HBITMAP hBmpMask) const;

		/// Calls [`ImageList_AddMasked`] to add a copy of `hBmp` into the image list.
		///
		/// [`ImageList_AddMasked`]: https://learn.microsoft.com/en-us/windows/win32/api/commctrl/nf-commctrl-imagelist_addmasked
		const ImageList& add_bitmap(HBITMAP hBmp, COLORREF crMask) const;

		/// Calls [`ImageList_AddIcon`] to add a copy of `hIcon` into the image list.
		///
		/// You still need to free your `hIcon`.
		///
		/// [`ImageList_AddIcon`]: https://learn.microsoft.com/en-us/windows/win32/api/commctrl/nf-commctrl-imagelist_addicon
		const ImageList& add_icon(HICON hIcon) const;

		/// Calls [`LoadIcon`] and [`ImageList_AddIcon`] to load an icon from the resource into the image list.
		///
		/// [`LoadIcon`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-loadiconw
		/// [`ImageList_AddIcon`]: https://learn.microsoft.com/en-us/windows/win32/api/commctrl/nf-commctrl-imagelist_addicon
		const ImageList& add_resource(WORD iconId) const;

		/// Calls [`SHGetFileInfo`] and [`ImageList_AddIcon`] to load icons from the shell,
		/// used by Windows Explorer to represent the given file extensions, like "mp3".
		///
		/// Example:
		///
		/// ```cpp
		/// wl::ImageList imgList{16, 16, ILC_COLOR32};
		/// imgList.add_shell_ext(L"mp3");
		/// ```
		///
		/// [`SHGetFileInfo`]: https://learn.microsoft.com/en-us/windows/win32/api/shellapi/nf-shellapi-shgetfileinfow
		/// [`ImageList_AddIcon`]: https://learn.microsoft.com/en-us/windows/win32/api/commctrl/nf-commctrl-imagelist_addicon
		const ImageList& add_shell_ext(WStrPtr fileExt) const;

		/// Calls [`ImageList_GetImageCount`] to return the number of stored images.
		///
		/// [`ImageList_GetImageCount`]: https://learn.microsoft.com/en-us/windows/win32/api/commctrl/nf-commctrl-imagelist_getimagecount
		[[nodiscard]] size_t count() const;

		/// Calls [`ImageList_GetIconSize`] to return the size of the stored icons.
		///
		/// The icon sized is defined once, when the image list is created.
		///
		/// [`ImageList_GetIconSize`]: https://learn.microsoft.com/en-us/windows/win32/api/commctrl/nf-commctrl-imagelist_geticonsize
		[[nodiscard]] SIZE icon_size() const;

		/// Calls [`ImageList_Remove`] to remove the image at `index`.
		///
		/// [`ImageList_Remove`]: https://learn.microsoft.com/en-us/windows/win32/api/commctrl/nf-commctrl-imagelist_remove
		const ImageList& remove(size_t index) const;

	private:
		HIMAGELIST _hImageList = nullptr;
	};

}
