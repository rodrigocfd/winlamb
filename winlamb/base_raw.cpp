
#include "base_raw.h"
using namespace _wli;

base_raw::~base_raw()
{
	if (hwnd() != nullptr) {
		SetWindowLongPtrW(hwnd(), GWLP_USERDATA, 0);
	}
}
