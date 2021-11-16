
#include <system_error>
#include "font.h"
using namespace wl;

font::~font()
{
	if (_hfont) {
		DeleteObject(_hfont);
		_hfont = nullptr;
	}
}

font::font(const LOGFONT& lf)
	: _hfont(CreateFontIndirectW(&lf))
{
	if (!_hfont) {
		throw std::system_error(GetLastError(), std::system_category(),
			"CreateFontIndirect failed");
	}
}

font& font::operator=(font&& other) noexcept
{
	this->~font();
	std::swap(_hfont, other._hfont);
	return *this;
}
