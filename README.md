# WinLamb

A lightweight modern C++ library for [Win32 API](https://en.wikipedia.org/wiki/Windows_API), using [C++11 lambdas](https://www.cprogramming.com/c++11/c++11-lambda-closures.html) to handle Windows messages.

## 1. Overview

As far as I can remember, around 2002 I started wrapping all my Win32 routines in classes, to make them reusable to myself, to save my time. Through all these years it took the form of a real library, a thin abstraction layer over raw Win32. People who saw it often commented that it was good, so in 2017 I decided to publish it on GitHub.

Then I wrote [CodeProject - WinLamb: using C++11 lambdas to handle Win32 messages](https://www.codeproject.com/Articles/1184175/WinLamb-using-Cplusplus-lambdas-to-handle-Win-mess), a comprehensive article explaining WinLamb's message handling model, with [dialogs](https://msdn.microsoft.com/en-us/library/windows/desktop/ms632588(v=vs.85).aspx) and also [ordinary windows](https://msdn.microsoft.com/en-us/library/windows/desktop/ms632595(v=vs.85).aspx). Actually, features from C++14 and C++17 are used as well, as much as my compiler (Visual C++) allows it.

Beyond dialog/window message handling, WinLamb also has wrappers for most native Windows controls ([textbox](https://msdn.microsoft.com/en-us/library/windows/desktop/bb775458(v=vs.85).aspx), [listview](https://msdn.microsoft.com/en-us/library/windows/desktop/bb774737(v=vs.85).aspx), etc.), along with other utility classes (strings, file I/O, COM wrappers, etc.) which play nice together. These controls and utilities, however, are _not_ mandatory: you can use your own classes upon the basic dialog/window infrastructure.

WinLamb by no means covers the whole Win32 API, simply because it's too huge. It just wraps some things. New features are constantly being added, though.

## 2. Setup

WinLamb is a header-only library. You can clone the repository or simply download the files; once referenced in your source code, it should work right away.

It has been tested with Visual C++ 2017.

### 2.1. Windows 10 manifest file

There's an included `win10.exe.manifest` file, which you can [add to your Visual Studio project](https://stackoverflow.com/a/18115255/6923555). This manifest includes Common Controls and gives you [Windows 10 support](https://msdn.microsoft.com/en-us/library/windows/desktop/dn481241(v=vs.85).aspx).

## 3. Example

This is a simple Win32 program written with WinLamb. Each window has a class, and messages are handled with C++11 lambdas. There's no need to write a message loop or window registering.

*Declaration:* My_Window.h

````cpp
#include "winlamb/window_main.h"

class My_Window : public wl::window_main {
public:
    My_Window();
};
````

*Implementation:* My_Window.cpp

````cpp
#include "My_Window.h"

RUN(My_Window) // optional, generate WinMain call and instantiate My_Window

My_Window::My_Window()
{
    setup.wndClassEx.lpszClassName = L"SOME_CLASS_NAME"; // class name to be registered
    setup.title = L"This is my window";
    setup.style |= WS_MINIMIZEBOX;

    on_message(WM_CREATE, [this](wl::wm::create p)->LRESULT
    {
        set_text(L"A new title for the window");
        return 0;
    });

    on_message(WM_LBUTTONDOWN, [](wl::wm::lbuttondown p)->LRESULT
    {
        bool isCtrlDown = p.has_ctrl();
        long xPos = p.pos().x;
        return 0;
    });
}
````

### 3.1. Project examples

Full real-world projects can be seen browsing [winlamb topic](https://github.com/topics/winlamb).

## 4. License

Licensed under [MIT license](https://opensource.org/licenses/MIT), see LICENSE.txt for details.
