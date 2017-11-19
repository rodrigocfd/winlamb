# WinLamb

A lightweight modern C++ library for [Win32 API](https://en.wikipedia.org/wiki/Windows_API), using [C++11 lambdas](https://www.cprogramming.com/c++11/c++11-lambda-closures.html) to handle Windows messages.

### Overview

As far as I can remember, around 2002 I started wrapping all my Win32 routines in classes, to make them reusable to myself, to save my time. Through all these years it took the form of a real library, a thin abstraction layer over raw Win32. People who saw it often commented that it was good, so in 2017 I decided to publish it on GitHub.

Then I wrote [CodeProject - WinLamb: using C++11 lambdas to handle Win32 messages](https://www.codeproject.com/Articles/1184175/WinLamb-using-Cplusplus-lambdas-to-handle-Win-mess), a comprehensive article explaining WinLamb's message handling model, with [dialogs](https://msdn.microsoft.com/en-us/library/windows/desktop/ms632588(v=vs.85).aspx) and also [ordinary windows](https://msdn.microsoft.com/en-us/library/windows/desktop/ms632595(v=vs.85).aspx). Actually, features from C++14 and C++17 are used as well, as much as my compiler (Visual C++) allows it.

Beyond dialog/window message handling, WinLamb also has wrappers for most native Windows controls ([textbox](https://msdn.microsoft.com/en-us/library/windows/desktop/bb775458(v=vs.85).aspx), [listview](https://msdn.microsoft.com/en-us/library/windows/desktop/bb774737(v=vs.85).aspx), etc.), along with other utility classes (strings, file I/O, COM wrappers, etc.) which play nice together. These controls and utilities, however, are _not_ mandatory: you can use your own classes upon the basic dialog/window infrastructure.

WinLamb by no means covers the whole Win32 API, simply because it's too huge. It just wraps some things. New features are constantly being added, though.

### Setup

WinLamb is a header-only library. You can clone the repository or simply download the files; once referenced in your source code, it should work right away.

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
RUN(SimpleMainWindow); // wraps WinMain call

SimpleMainWindow::SimpleMainWindow()
{
  setup.wndClassEx.lpszClassName = L"SOME_CLASS_NAME";
  setup.title = L"This is my window";
  setup.style |= WS_MINIMIZEBOX;

  on_message(WM_CREATE, [&](wl::params p)->LRESULT
  {
    set_text(L"A new title for the window");
    return 0;
  });

  on_message(WM_LBUTTONDOWN, [&](wl::params p)->LRESULT
  {
    set_text(L"Window clicked!");
    return 0;
  });
}
````

Full real-world projects can be seen browsing [winlamb tag](https://github.com/topics/winlamb).

### License

Licensed under [MIT license](https://opensource.org/licenses/MIT), see LICENSE.txt for details.
