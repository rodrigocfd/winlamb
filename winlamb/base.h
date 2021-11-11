
#pragma once
#include <Windows.h>

namespace _wli { class base_parent; } // friend forward declaration

namespace _wli {

class base {
private:
	friend base_parent;
	HWND _hwnd = nullptr;

public:
	virtual ~base() = 0;

	// Returns the underlying handle to the window.
	// Note that this handle is initially NULL, receiving its value upon creation.
	[[nodiscard]] constexpr HWND hwnd() const { return _hwnd; }
};

}
