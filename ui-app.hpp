#pragma once
#include "aux-wrap.hpp"
#include <CommCtrl.h>

namespace _wl_internal {

	/** Initializes the GUI environment. */
	struct GuiApp final : private wl::NoCopyNoMove {
		~GuiApp();
		GuiApp();

		static int logPixelsX, logPixelsY;
		static HFONT hUiFont;
	};

	void apply_ui_font(HWND hWnd);
	[[nodiscard]] WORD valid_ctrl_id(WORD ctrlId);
	[[nodiscard]] HINSTANCE wnd_hinst(HWND hWnd);
	[[nodiscard]] std::wstring wnd_text(HWND hWnd);
	void set_wnd_text(HWND hWnd, wl::WStrView text);
	void focus(HWND hCtrl);
	void screen_to_client_rc(HWND hWnd, RECT *pRc);
	[[nodiscard]] SIZE calc_text_bound_box(wl::WStrView text);
	[[nodiscard]] SIZE calc_text_bound_box_with_check(wl::WStrView text);

	/** Validates event access. */
	template<typename E>
	constexpr E& valid_event(HWND hWnd, E &events) {
		#ifdef _DEBUG
		if (hWnd)
			throw "Cannot add events after the window or control is created.";
		#endif
		return events;
	}

	/** An array of non-movable objects. */
	template<typename T>
	class NonMovableArray final : private wl::NoCopyNoMove {
	public:
		~NonMovableArray() {
			for (size_t i = 0; i < _sz; ++i) _ptr[i].~T();
			StorageT *pMem = reinterpret_cast<StorageT*>(_ptr);
			delete[] pMem;
		}
		explicit NonMovableArray(size_t numElems) : _sz{numElems} {
			StorageT *pMem = new StorageT[numElems]; // don't forget to call placement new on each element!
			_ptr = reinterpret_cast<T*>(pMem);
		}
		[[nodiscard]] constexpr bool empty() const { return !size(); }
		[[nodiscard]] constexpr size_t size() const { return _sz; }
		[[nodiscard]] constexpr const T& operator[](size_t index) const { return _ptr[index]; }
		[[nodiscard]] constexpr T& operator[](size_t index) { return _ptr[index]; }
	private:
		using StorageT = std::aligned_storage_t<sizeof(T), alignof(T)>;
		size_t _sz = 0;
		T *_ptr = nullptr;
	};

}

/** @brief Implements WinMain(), instantiates the main class, and calls run(). */
#define RUN_MAIN(usermainclass, mainwndobj) \
	int APIENTRY wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int cmdShow) { \
		int ret = -1; \
		{ \
			_wl_internal::GuiApp guiApp{}; \
			usermainclass w; \
			ret = w.mainwndobj.run(hInst, cmdShow); \
		} \
		if (_CrtDumpMemoryLeaks()) [[unlikely]] { \
			MessageBoxW(nullptr, L"A memory leak was found.", L"Memory leak", MB_ICONERROR); \
		} \
		return ret; \
	}

////////////////////////////////////////////////////////////////////////////////

/// @brief Adjusts pixel values according to the current [system DPI].
///
/// These functions should be used every time you use pixels on the screen.
///
/// [system DPI]: https://learn.microsoft.com/en-us/windows/win32/hidpi/high-dpi-desktop-application-development-on-windows
namespace wl::dpi {

	/** Returns the value adjusted according to the current horizontal system DPI. */
	[[nodiscard]] int x(int xVal);

	/** Returns the value adjusted according to the current vertical system DPI. */
	[[nodiscard]] int y(int yVal);

	/// Returns a [`POINT`] with values adjusted according to the current
	/// horizontal and vertical system DPI.
	///
	/// [`POINT`]: https://learn.microsoft.com/en-us/windows/win32/api/windef/ns-windef-point
	[[nodiscard]] POINT pt(int xVal, int yVal);

	/// Returns a [`POINT`] with values adjusted according to the current
	/// horizontal and vertical system DPI.
	///
	/// [`POINT`]: https://learn.microsoft.com/en-us/windows/win32/api/windef/ns-windef-point
	[[nodiscard]] POINT pt(POINT value);

	/// Returns a [`SIZE`] with values adjusted according to the current
	/// horizontal and vertical system DPI.
	///
	/// [`SIZE`]: https://learn.microsoft.com/en-us/windows/win32/api/windef/ns-windef-size
	[[nodiscard]] SIZE sz(int xVal, int yVal);

	/// Returns a [`SIZE`] with values adjusted according to the current
	/// horizontal and vertical system DPI.
	///
	/// [`SIZE`]: https://learn.microsoft.com/en-us/windows/win32/api/windef/ns-windef-size
	[[nodiscard]] SIZE sz(SIZE value);

}

namespace wl {

	/// @brief Pure abstract class; implemented by icon stores.
	///
	/// Controls which make use of icons will have a method which expose this
	/// abstract class, so icons can be added in various ways.
	class IStoreIcon {
	public:
		/** Adds the `HICON` to the store, taking ownership of it. */
		virtual void add_icon(HICON hIcon) = 0;

		/// Calls [`LoadImage`] to load an icon from the resource.
		///
		/// [`LoadImage`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-loadimagew
		virtual void add_resource(WORD iconId) = 0;

		/// Calls [`SHGetFileInfo`] to load icons from the shell.
		///
		/// These are the icons used by Windows Explorer to represent the given
		/// file extensions, like "mp3".
		///
		/// [`SHGetFileInfo`]: https://learn.microsoft.com/en-us/windows/win32/api/shellapi/nf-shellapi-shgetfileinfow
		virtual void add_shell_ext(WStrView fileExt) = 0;
	};

	/// @brief Specifies the horizontal and vertical behavior for a control when
	/// the parent window is resized.
	enum class Lay : BYTE {
		/** When parent is resized, nothing happens. */
		hold_hold,
		/// When parent resizes:
		/// - horizontal: nothing happens;
		/// - vertical: control moves anchored at bottom.
		hold_move,
		/// When parent resizes:
		/// - horizontal: nothing happens;
		/// - vertical: control is resized together.
		hold_resize,
		/// When parent resizes:
		/// - horizontal: control moves anchored at right;
		/// - vertical: nothing happens.
		move_hold,
		/// When parent resizes:
		/// - horizontal: control moves anchored at right;
		/// - vertical: control moves anchored at bottom.
		move_move,
		/// When parent resizes:
		/// - horizontal: control moves anchored at right;
		/// - vertical: control is resized together.
		move_resize,
		/// When parent resizes:
		/// - horizontal: control is resized together;
		/// - vertical: nothing happens.
		resize_hold,
		/// When parent resizes:
		/// - horizontal: control is resized together;
		/// - vertical: control moves anchored at bottom.
		resize_move,
		/// When parent resizes:
		/// - horizontal: control is resized together;
		/// - vertical: control is resized together.
		resize_resize,
	};

}

namespace _wl_internal {

	/** Manages a `HIMAGELIST`. */
	class ImageList final : public wl::IStoreIcon {
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
	class HIconStore final : public wl::IStoreIcon {
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

	/// Rearranges position and size of each control when the parent resizes,
	/// according to `Lay` flags.
	class Layout final {
	public:
		struct Ctrl final {
			HWND hCtrl;
			wl::Lay lay;
			RECT rcOrig;
		};

		void add(HWND hCtrl, wl::Lay layout);
		void rearrange(WPARAM wp, LPARAM lp); // to be called internally within WM_SIZE

		std::vector<Ctrl> _ctrls{};
		SIZE _szOrig{}; // original parent client area
	};

}
