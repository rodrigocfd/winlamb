# WinLamb

A lightweight modern C++ library for Win32, using C++11 lambdas to handle Windows messages.

### Introductory article

[CodeProject - WinLamb: using C++11 lambdas to handle Win32 messages](https://www.codeproject.com/Articles/1184175/WinLamb-using-Cplusplus-lambdas-to-handle-Win-mess) is a comprehensive article explaining all usage of WinLamb library.

### Example

This is a simple Win32 program written with WinLamb. Each window has a class, and messages are handled with C++11 lambdas. There's no need to write a message loop or window registering.

````cpp
// Declaration: SimpleMainWindow.h

#include "winlamb/window_main.h"

class SimpleMainWindow : public wl::window_main
{
public:
  SimpleMainWindow();
};
````

````cpp
// Implementation: SimpleMainWindow.cpp

#include "SimpleMainWindow.h"
RUN(SimpleMainWindow);

SimpleMainWindow::SimpleMainWindow()
{
  setup.wndClassEx.lpszClassName = L"SOME_CLASS_NAME";
  setup.title = L"This is my window";
  setup.style |= WS_MINIMIZEBOX;

  on_message(WM_CREATE, [&](wl::params p)->LRESULT
  {
    SetWindowText(hwnd(), L"A new title for the window");
    return 0;
  });

  on_message(WM_LBUTTONDOWN, [&](wl::params p)->LRESULT
  {
    SetWindowText(hwnd(), L"Window clicked!");
    return 0;
  });
}
````

### More

This project contains only the core files of the library, the mininum necessary infrastructure. If you need extended funcionalities like native widget wrappers, take a look at addendum project [WinLamb More](https://github.com/rodrigocfd/winlamb-more).

### License

Licensed under [MIT license](https://opensource.org/licenses/MIT), see license.txt for details.
