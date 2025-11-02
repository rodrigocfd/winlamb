# WinLamb

A lightweight modern C++20 library for the [Win32 API](https://en.wikipedia.org/wiki/Windows_API), using [lambdas](https://en.cppreference.com/w/cpp/language/lambda.html) to handle window messages.

Goals:

* simplify the process of writing native GUI Win32 applications;
* lay a scalable foundation for large, complex programs;
* support high-performance applications.

## Installation

WinLamb has no dependencies other than the Win32 itself, and won't require any special build steps.

Just copy all the files under your compilation tree and `#include <winlamb/lib.h>`, and that's it.

All WinLamb entities are enclosed in the `wl` namespace.

## Example

The example below is a full Win32 program consisting of a single window, managed by `wl::WindowMain`.

Note there's no need to write a [message loop](https://learn.microsoft.com/en-us/windows/win32/winmsg/using-messages-and-message-queues) or [register a window](https://learn.microsoft.com/en-us/windows/win32/winmsg/about-window-classes). The [`WM_CREATE`](https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-create) message is handled with a lambda, and the `RUN_MAIN` macro takes care of writing the [`WinMain`](https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point) entry point for you.

* Declaration: MyMain.h

```cpp
#include <winlamb/lib.h>

class MyMain final {
public:
    MyMain();
    wl::WindowMain wnd{};
};
```

* Implementation: MyMain.cpp

```cpp
RUN_MAIN(MyMain, wnd)

MyMain::MyMain() {
    wnd.setup().title = L"My main window";
    wnd.setup().size = wl::dpi::sz(500, 300);
    wnd.setup().style |= WS_SIZEBOX | WS_MAXIMIZEBOX;

    wnd.on().wm_create([this](wl::wm::Create p) -> int {
        wnd.set_title(L"A new title");
        return 0;
    });
}
```

## License

Licensed under [MIT license](https://opensource.org/licenses/MIT), see [LICENSE.md](LICENSE.md) for details.
