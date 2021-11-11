
#pragma once
#include "base.h"
#include "events.h"
#include "resizer_children.h"

namespace _wli { class base_dlg; } // friend forward declaration
namespace _wli { class base_raw; }

namespace _wli {

// Base to all parent windows.
class base_parent : public base {
public:
	enum class creation_type { raw, dialog };

private:
	friend base_dlg;
	friend base_raw;
	events _events_internal, _events;
	creation_type _creation_type;
	resizer_children _resizer;

public:
	virtual ~base_parent() = 0;
	explicit base_parent(creation_type creation_ty);

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

private:
	void set_hwnd(HWND h) { _hwnd = h; }
};

}
