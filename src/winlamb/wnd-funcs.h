#pragma once
#include <stdexcept>
#include <vector>
#include "lib-include-win.h"
#include <CommCtrl.h>
#include "str.h"

namespace _wl_internal {

	[[nodiscard]] HFONT ui_font();
	void apply_ui_font(HWND hWnd);
	void delete_ui_font();

	[[nodiscard]] WORD valid_ctrl_id(WORD ctrlId);
	[[nodiscard]] HINSTANCE wnd_hinst(HWND hWnd);
	[[nodiscard]] std::wstring wnd_text(HWND hWnd);
	void set_wnd_text(HWND hWnd, wl::WStrView text);
	[[nodiscard]] SIZE calc_text_bound_box(wl::WStrView text);
	[[nodiscard]] SIZE calc_text_bound_box_with_check(wl::WStrView text);

	/** Validates opts access. */
	template<typename O>
	constexpr O& valid_opts(HWND hWnd, O &opts) {
		#ifdef _DEBUG
		if (hWnd)
			throw "Cannot change setup options after the window or control is created.";
		#endif
		return opts;
	}

	/** Validates event access. */
	template<typename E>
	constexpr E& valid_event(HWND hWnd, E &events) {
		#ifdef _DEBUG
		if (hWnd)
			throw "Cannot add events after the window or control is created.";
		#endif
		return events;
	}

}

////////////////////////////////////////////////////////////////////////////////

namespace wl {

	/// @brief Pure abstract class; implemented by icon stores.
	///
	/// Controls which make use of icons will have a method which expose this abstract class,
	/// so icons can be added in various ways.
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

		[[nodiscard]] constexpr HICON operator[](size_t index) const { return _hIcons[index]; }
		[[nodiscard]] constexpr size_t count() const                 { return _hIcons.size(); }

		void add_icon(HICON hIcon) override;
		void add_resource(WORD iconId) override;
		void add_shell_ext(wl::WStrView fileExt) override;

	private:
		std::vector<HICON> _hIcons{};
		SIZE _szIcon;
	};

}
