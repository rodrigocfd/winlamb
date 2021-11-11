
#pragma once
#include <Windows.h>

namespace wl {

// Manages an HFONT resource, which is automatically destroyed.
class font final {
private:
	HFONT _hfont = nullptr;

public:
	~font();
	font() = default;
	explicit font(const LOGFONT& lf);
	font(font&& other) noexcept : _hfont(other._hfont) { other._hfont = nullptr; }

	font& operator=(font&& other) noexcept;

	[[nodiscard]] constexpr HFONT hfont() const { return _hfont; }
};

}
