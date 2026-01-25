# WinLamb

A lightweight modern C++20 library for the [Win32 API](https://en.wikipedia.org/wiki/Windows_API), using [lambdas](https://en.cppreference.com/w/cpp/language/lambda.html) to handle window messages.

Goals:

* simplify the process of writing native GUI Win32 applications;
* lay a scalable foundation for large, complex programs;
* support high-performance applications;
* build windows in two possible ways:
    * programmatically; or
    * loading dialog resources.

All WinLamb entities are enclosed in the `wl` namespace.

## Installation

WinLamb has no dependencies other than the Win32 itself, and won't require any special build steps.

Just copy all source files under your compilation tree, and `#include <winlamb/winlamb.hpp>`. That's all.

## Classes

Container classes represent windows which can host child controls:

| Container class | Description |
| -- | -- |
| `wl::WindowMain` | Main window application. Usually, this is where your program starts. |
| `wl::WindowModal` | A modal popup window. |
| `wl::WindowControl` | A custom child control. |

There are a few pure abstract classes (interfaces) which are implemented by windows and controls:

| Interface | Description |
| -- | -- |
| `wl::Window` | Implemented by all windows. |
| `wl::WindowParent` | Implemented by all windows which can host child controls. |
| `wl::WindowChild` | Implemented by all child controls. |

Native controls are the Win32 built-in widgets:

| Native control | Description |
| -- | -- |
| `wl::Button` | [Button](https://learn.microsoft.com/en-us/windows/win32/controls/button-types-and-styles#push-buttons) control. |
| `wl::CheckBox` | [CheckBox](https://learn.microsoft.com/en-us/windows/win32/controls/button-types-and-styles#check-boxes) control. |
| `wl::ComboBox` | [ComboBox](https://learn.microsoft.com/en-us/windows/win32/controls/about-combo-boxes) control (dropdown). |
| `wl::DateTimePicker` | [DateTimePicker](https://learn.microsoft.com/en-us/windows/win32/controls/date-and-time-picker-controls) control. |
| `wl::Edit` | [Edit](https://learn.microsoft.com/en-us/windows/win32/controls/about-edit-controls) control (textbox). |
| `wl::ListView` | [ListView](https://learn.microsoft.com/en-us/windows/win32/controls/list-view-controls-overview) control. |
| `wl::MonthCalendar` | [MonthCalendar](https://learn.microsoft.com/en-us/windows/win32/controls/month-calendar-controls) control. |
| `wl::RadioGroup` | Manages a group of `wl::RadioButton`, the [RadioButton](https://learn.microsoft.com/en-us/windows/win32/controls/button-types-and-styles#radio-buttons) control. |
| `wl::Static` | [Static](https://learn.microsoft.com/en-us/windows/win32/controls/about-static-controls) control (label). |
| `wl::StatusBar` | [StatusBar](https://learn.microsoft.com/en-us/windows/win32/controls/status-bars) control. |
| `wl::Tab` | [Tab](https://learn.microsoft.com/en-us/windows/win32/controls/tab-controls) control. |
| `wl::Trackbar` | [Trackbar](https://learn.microsoft.com/en-us/windows/win32/controls/trackbar-controls) control. |
| `wl::TreeView` | [TreeView](https://learn.microsoft.com/en-us/windows/win32/controls/tree-view-controls) control. |

A few utility entities are included for convenience:

| Utility | Description |
| -- | -- |
| `wl::ComPtr` | Templated [COM](https://learn.microsoft.com/en-us/windows/win32/com/component-object-model--com--portal) smart pointer. |
| `wl::DropFiles` | Implements [`IDropTarget`](https://learn.microsoft.com/en-us/windows/win32/api/oleidl/nn-oleidl-idroptarget) COM interface, allowing file drag & drop on the window. |
| `wl::Encoding` | Character encoding information. |
| `wl::File` | Manages a file `HANDLE`. |
| `wl::FileMapped` | Manages a memory-mapped file. |
| `wl::WStrView` | Wraps a pointer to a null-terminated string. |
| `wl::dpi` | Adjusts pixel values according to the current system [DPI](https://learn.microsoft.com/en-us/windows/win32/hidpi/high-dpi-desktop-application-development-on-windows). |
| `wl::path` | Filepath utilities. |
| `wl::str` | [UTF-16](https://learn.microsoft.com/en-us/windows/win32/intl/unicode-in-the-windows-api) wide string utilities. |
| `wl::vec` | Utilities for [`vector`](https://en.cppreference.com/w/cpp/container/vector.html), [`span`](https://en.cppreference.com/w/cpp/container/span.html) and [`array`](https://en.cppreference.com/w/cpp/container/array.html). |

## Example

The example below is a full native Win32 program consisting of a single window, managed by `wl::WindowMain`. Note there's no need to write a [message loop](https://learn.microsoft.com/en-us/windows/win32/winmsg/using-messages-and-message-queues) or [register a window](https://learn.microsoft.com/en-us/windows/win32/winmsg/about-window-classes).

The whole code, .h and .cpp files:

```cpp
// -- MyMain.h declaration file --

#include <winlamb/lib.h>

class MyMain final {
public:
    MyMain();
    wl::WindowMain wnd{wl::MainOpts{ // manages our main window
        .size = wl::dpi::sz(500, 300),
        .style = wl::MainOpts{}.style | WS_SIZEBOX | WS_MAXIMIZEBOX,
        .title = L"My main window",
    }};
    wl::Button btn{wnd, wl::ButtonOpts{ // this button is a child control of our main window
        .pos = wl::dpi::pt(10, 10),
        .text = L"&Click me",
    }};
};
```

```cpp
// -- MyMain.cpp implementation file --

RUN_MAIN(MyMain, wnd) // will generate WinMain() entry point

MyMain::MyMain() {
    wnd.on().wm_create([this](wl::wm::Create p) -> int { // WM_CREATE event
        wnd.set_title(L"A new title");
        return 0;
    });

    btn.on().bn_clicked([this]() -> void { // WM_COMMAND event for BN_CLICKED
       MessageBoxW(wnd.hwnd(), L"Button clicked", L"Hello", MB_ICONINFORMATION);
    });
}
```

## License

Licensed under [MIT license](https://opensource.org/licenses/MIT), see [LICENSE.md](LICENSE.md) for details.
