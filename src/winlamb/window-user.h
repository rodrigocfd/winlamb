#pragma once
#include "window-dialog.h"

namespace wl { class DropFiles; }

namespace wl {

	// Main application window.
	class WindowMain final {
	public:
		WindowMain() = delete;
		WindowMain(const WindowMain&) = delete;
		WindowMain(WindowMain&&) = delete;
		WindowMain& operator=(const WindowMain&) = delete;
		WindowMain& operator=(WindowMain&&) = delete;

		WindowMain(WORD dlgId, WORD iconId = 0, WORD accelTblId = 0)
			: _dlgMain{std::make_optional<_wl_internal::DialogMain>(dlgId, iconId, accelTblId)} { }

		[[nodiscard]] constexpr HWND hwnd() const { return _rawMain.has_value() ? _rawMain.value().hwnd() : _dlgMain.value().hwnd(); }

		[[nodiscard]] std::wstring title() const;
		void set_title(WStrPtr title) const;

		// Allows message events to be added.
		// The events must be added before the dialog is created.
		[[nodiscard]] _wl_internal::EventsUser& on() { return wnd_msg().on(); }

		// Creates a new, detached thread and runs the function. Catches uncaught exceptions.
		void thread_detach(std::function<void()> cb) const { wnd_msg().thread_detach(cb); }

		// Blocks the current thread and runs the function in the original UI thread. Catches uncaught exceptions.
		void thread_ui(std::function<void()> cb) const { wnd_msg().thread_ui(cb); }

		// Runs the main dialog, blocking until the dialog is closed.
		// Prefer using the RUN_MAIN macro instead of calling this method directly.
		int run(HINSTANCE hInst, int cmdShow);

	private:
		[[nodiscard]] const _wl_internal::WindowMsg& wnd_msg() const;
		[[nodiscard]] _wl_internal::WindowMsg& wnd_msg();

		std::optional<_wl_internal::DialogMain> _rawMain{}; // either one
		std::optional<_wl_internal::DialogMain> _dlgMain{};

		friend _wl_internal::NativeCtrl;
		friend _wl_internal::EventsNativeCtrl;
		friend DropFiles;
	};

}

namespace wl {

	// Modal window.
	class WindowModal final {
	public:
		WindowModal() = delete;
		WindowModal(const WindowModal&) = delete;
		WindowModal(WindowModal&&) = delete;
		WindowModal& operator=(const WindowModal&) = delete;
		WindowModal& operator=(WindowModal&&) = delete;

		WindowModal(WORD dlgId)
			: _dlgModal{std::make_optional<_wl_internal::DialogModal>(dlgId)} { }

		// Displays the modal dialog, blocking until the modal is closed.
		void show(const wl::WindowMain &owner);

	private:
		[[nodiscard]] const _wl_internal::WindowMsg& wnd_msg() const;
		[[nodiscard]] _wl_internal::WindowMsg& wnd_msg();

		std::optional<_wl_internal::DialogModal> _rawModal{}; // either one
		std::optional<_wl_internal::DialogModal> _dlgModal{};

		friend _wl_internal::NativeCtrl;
		friend _wl_internal::EventsNativeCtrl;
		friend DropFiles;
	};

}
