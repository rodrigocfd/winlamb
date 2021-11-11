
#pragma once
#include "font.h"

namespace _wli {

void first_calls();
void lippincott();
const wl::font& ui_font();
int run_main_loop(HWND hwnd, HACCEL haccel);

}
