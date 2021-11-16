
#pragma once
#include "font.h"

namespace _wli {

void first_calls();
void lippincott();
void multiply_dpi(POINT* pos, SIZE* sz);
const wl::font& ui_font();
int run_main_loop(HWND hwnd, HACCEL haccel);

}
