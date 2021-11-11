
#pragma once
#include "events.h"
#include "resizer_children.h"

namespace _wli { class window_dlg; } // friend forward declaration

namespace _wli {

// Base to window_dlg and window_raw.
class window_base {
public:
	enum class creation_type { raw, dialog };

private:
	friend window_dlg;
	HWND _hwnd = nullptr;
	events _events_internal, _events;
	creation_type _creation_type;
	resizer_children _resizer;

public:
	virtual ~window_base() { }
	explicit window_base(creation_type creation_ty);

	// Returns the underlying handle to the window.
	// Note that this handle is initially NULL, receiving its value upon creation.
	[[nodiscard]] constexpr HWND hwnd() const { return _hwnd; }

	// Exposes the window events that can be handled.
	// Each event corresponds to a standard window message.
	[[nodiscard]] events& on();

	// Runs a lambda asynchronously in a new, detached thread.
	// This method is analog to std::thread([](){ ... }).detach(), but exception-safe.
	// This is useful if you want to perform a lengthy processing which could freeze the UI.
	// In order to update the UI, call run_ui_thread(), or it may deadlock.
	void run_detached_thread(std::function<void()> fun) const;

	// Runs a lambda synchronously in the UI thread.
	// This is useful if you are in a detached thread and need to update the UI.
	void run_ui_thread(std::function<void()> fun) const;
};

}
