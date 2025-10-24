#pragma once
#include "window-dialog.h"
#include "window-raw.h"

namespace wl {

	class DropFiles;

	// Pure abstract class; implemented by any window which can contain child controls.
	class WindowParent {
	public:
		[[nodiscard]] virtual constexpr HWND hwnd() const = 0;
	private:
		[[nodiscard]] virtual const _wl_internal::WindowMsg& wnd_msg() const = 0;
		[[nodiscard]] virtual _wl_internal::WindowMsg& wnd_msg() = 0;
		friend _wl_internal::NativeCtrl; // so NativeCtrl can be constructed, then expose parent events to controls ctor
		friend _wl_internal::EventsNativeCtrl; // so events can be added to parent
		friend DropFiles;
	};

}

namespace wl {

	// Main application window.
	class WindowMain final : public WindowParent {
	public:
		DEL_COPY_MOVE(WindowMain);
		WindowMain() = delete;

		// Constructs the main window programmatically.
		WindowMain(opts::Main opts)
			: _rawMain{std::make_optional<_wl_internal::RawMain>(opts)} { }

		// Constructs the main window from a dialog resource.
		WindowMain(WORD dlgId, WORD iconId = 0, WORD accelTblId = 0)
			: _dlgMain{std::make_optional<_wl_internal::DialogMain>(dlgId, iconId, accelTblId)} { }

		[[nodiscard]] constexpr HWND hwnd() const override { return _rawMain.has_value() ? _rawMain.value().hwnd() : _dlgMain.value().hwnd(); }
		[[nodiscard]] std::wstring title() const           { return wnd_msg()._wnd.text(); }
		void set_title(WStrPtr title) const                { wnd_msg()._wnd.set_text(title); }

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
		[[nodiscard]] const _wl_internal::WindowMsg& wnd_msg() const override;
		[[nodiscard]] _wl_internal::WindowMsg& wnd_msg() override;

		std::optional<_wl_internal::RawMain> _rawMain{}; // either one
		std::optional<_wl_internal::DialogMain> _dlgMain{};
	};

}

namespace wl {

	// Modal window.
	class WindowModal final : public WindowParent {
	public:
		DEL_COPY_MOVE(WindowModal);
		WindowModal() = delete;

		// Constructs the modal window programmatically.
		WindowModal(WindowParent &parent, opts::Modal opts)
			: _rawModal{std::make_optional<_wl_internal::RawModal>(parent, opts)} { }

		// Constructs the modal window from a dialog resource.
		WindowModal(const WindowParent &parent, WORD dlgId)
			: _dlgModal{std::make_optional<_wl_internal::DialogModal>(parent, dlgId)} { }

		[[nodiscard]] constexpr HWND hwnd() const { return _rawModal.has_value() ? _rawModal.value().hwnd() : _dlgModal.value().hwnd(); }
		[[nodiscard]] std::wstring title() const  { return wnd_msg()._wnd.text(); }
		void set_title(WStrPtr title) const       { wnd_msg()._wnd.set_text(title); }

		// Allows message events to be added.
		// The events must be added before the dialog is created.
		[[nodiscard]] _wl_internal::EventsUser& on() { return wnd_msg().on(); }

		// Creates a new, detached thread and runs the function. Catches uncaught exceptions.
		void thread_detach(std::function<void()> cb) const { wnd_msg().thread_detach(cb); }

		// Blocks the current thread and runs the function in the original UI thread. Catches uncaught exceptions.
		void thread_ui(std::function<void()> cb) const { wnd_msg().thread_ui(cb); }

		// Displays the modal dialog, blocking until the modal is closed.
		void show();

	private:
		[[nodiscard]] const _wl_internal::WindowMsg& wnd_msg() const override;
		[[nodiscard]] _wl_internal::WindowMsg& wnd_msg() override;

		std::optional<_wl_internal::RawModal> _rawModal{}; // either one
		std::optional<_wl_internal::DialogModal> _dlgModal{};
	};

}
