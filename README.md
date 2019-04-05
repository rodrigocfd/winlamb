# WinLamb

A lightweight modern C++ library for [Win32 API](https://en.wikipedia.org/wiki/Windows_API), using [C++11 lambdas](https://www.cprogramming.com/c++11/c++11-lambda-closures.html) to handle Windows messages.

1. [Overview](#1-overview)
2. [Setup](#2-setup)
3. [Example](#3-example)
4. [Classes summary](#4-classes-summary)
5. [License](#5-license)

## 1. Overview

As far as I can remember, around 2002 I started wrapping all my Win32 routines in classes, to make them reusable to myself, to save my time. Through all these years it took the form of a real library, a thin abstraction layer over raw Win32. People who saw it often commented that it was good, so in 2017 I decided to publish it on GitHub.

Then I wrote [CodeProject - WinLamb: using C++11 lambdas to handle Win32 messages](https://www.codeproject.com/Articles/1184175/WinLamb-using-Cplusplus-lambdas-to-handle-Win-mess), a comprehensive article explaining WinLamb's message handling model, with [dialogs](https://docs.microsoft.com/pt-br/windows/desktop/dlgbox/dialog-boxes) and also [ordinary windows](https://docs.microsoft.com/pt-br/windows/desktop/winmsg/windows). Actually, features from C++14 and C++17 are used as well, as much as my compiler (Visual C++) allows it.

Beyond dialog/window message handling, WinLamb also has wrappers for most native Windows controls ([textbox](https://docs.microsoft.com/pt-br/windows/desktop/Controls/edit-controls), [listview](https://docs.microsoft.com/pt-br/windows/desktop/Controls/list-view-control-reference), etc.), along with other utility classes (strings, file I/O, COM wrappers, etc.) which play nice together. These controls and utilities, however, are _not_ mandatory: you can use your own classes upon the basic dialog/window infrastructure.

WinLamb by no means covers the whole Win32 API, simply because it's too huge. It just wraps some things. New features are constantly being added, though.

## 2. Setup

WinLamb is a header-only library. You can clone the repository or simply [download](https://github.com/rodrigocfd/winlamb/archive/master.zip) the files; once referenced in your source code, it should work right away.

It has been tested with Visual C++ 2017.

### 2.1. Windows 10 manifest file

There's an included `win10.exe.manifest` file, which you can [add to your Visual Studio project](https://stackoverflow.com/a/18115255/6923555). This manifest includes Common Controls and gives you [Windows 10 support](https://docs.microsoft.com/pt-br/windows/desktop/SysInfo/targeting-your-application-at-windows-8-1).

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
    setup.style |= wl::ws::MINIMIZEBOX;

    on_message(WM_CREATE, [this](wl::wm::create p) -> LRESULT
    {
        set_text(L"A new title for the window");
        return 0;
    });

    on_message(WM_LBUTTONDOWN, [](wl::wm::lbuttondown p) -> LRESULT
    {
        bool isCtrlDown = p.has_ctrl();
        long xPos = p.pos().x;
        return 0;
    });
}
````

### 3.1. Project examples

I've written the following examples showcasing different things:

* [Click lines](https://github.com/rodrigocfd/click-lines)
* [FLAC/LAME frontend](https://github.com/rodrigocfd/flac-lame-frontend)
* [Chromium Peeker](https://github.com/rodrigocfd/chromium-peeker)

More projects can be seen browsing [winlamb topic](https://github.com/topics/winlamb).

## 4. Classes summary

Each file is named after the class it contains; for example, file "button.h" contains `button` class.

To create your windows, you inherit from these classes below. See the [article](https://www.codeproject.com/Articles/1184175/WinLamb-using-Cplusplus-lambdas-to-handle-Win-mess) and the [examples](#3-example) to learn how to use them:

| Class | Description |
| :--- | :--- |
| [`dialog_control`](dialog_control.h) | Inherit from this class to have a dialog to be used as a control within a parent window. |
| [`dialog_main`](dialog_main.h) | Inherit from this class to have a dialog as the main window for your application. |
| [`dialog_modal`](dialog_modal.h) | Inherit from this class to have a modal dialog popup. |
| [`dialog_modeless`](dialog_modeless.h) | Inherit from this class to have a dialog modeless popup. |
| [`window_control`](window_control.h) | Inherit from this class to have an user-custom window control. |
| [`window_main`](window_main.h) | Inherit from this class to have an ordinary main window for your application. |

Wrappers and utility classes:

| Class | Description |
| :--- |:--- |
| [`button`](button.h) | Wrapper to native button control. |
| [`checkbox`](checkbox.h) | Wrapper to native checkbox control. |
| [`com::bstr`](internals/com_bstr.h) | Wrapper to BSTR string, used with COM. |
| [`com::lib`](internals/com_lib.h) | Smart class to automate CoInitialize and CoUninitialize calls. |
| [`com::ptr`](internals/com_ptr.h) | Wrapper to a COM pointer. |
| [`com::variant`](internals/com_variant.h) | Wrapper to VARIANT object, used with COM. |
| [`combobox`](combobox.h) | Wrapper to native combobox control. |
| [`datetime`](datetime.h) | Wrapper to SYSTEMTIME structure. |
| [`datetime_picker`](datetime_picker.h) | Wrapper to datetime picker control from Common Controls library. |
| [`gdi::dc`](device_context.h) | Wrapper to device context. |
| [`gdi::dc_painter`](device_context.h) | Wrapper to device context which calls BeginPaint/EndPaint automatically. |
| [`gdi::dc_painter_buffered`](device_context.h) | Wrapper to device context which calls BeginPaint/EndPaint automatically with double-buffer. |
| [`download`](download.h) | Automates internet download operations. |
| [`executable`](executable.h) | Utilities to executable-related stuff. |
| [`file`](file.h) | Wrapper to a low-level HANDLE of a file. |
| [`file_ini`](file_ini.h) | Wrapper to INI file. |
| [`file_mapped`](file_mapped.h) | Wrapper to a memory-mapped file. |
| [`font`](font.h) | Wrapper to HFONT handle. |
| [`icon`](icon.h) | Wrapper to HICON handle. |
| [`image_list`](image_list.h) | Wrapper to image list object from Common Controls library. |
| [`insert_order_map`](insert_order_map.h) | Vector-based associative container which keeps the insertion order. |
| [`label`](label.h) | Wrapper to native static text control. |
| [`list_view`](list_view.h) | Wrapper to listview control from Common Controls library. |
| [`menu`](menu.h) | Wrapper to HMENU handle. |
| [`path`](path.h) | Utilities to file path operations with std::wstring. |
| [`progress_taskbar`](progress_taskbar.h) | Allows to show a progress bar in the taskbar button of the window, in green, yellow or red. |
| [`progressbar`](progressbar.h) | Wrapper to progressbar control from Common Controls library. |
| [`radio`](radio.h) | Wrapper to native radio button control. |
| [`radio_group`](radio_group.h) | Automates a group of native radio buttons. |
| [`resizer`](resizer.h) | Allows the resizing of multiple controls when the parent window is resized. |
| [`scrollinfo`](scrollinfo.h) | Automates SCROLLINFO operations. |
| [`statusbar`](statusbar.h) | Wrapper to status control from Common Controls library. |
| [`str`](str.h) | Utilities to std::wstring. |
| [`subclass`](subclass.h) | Manages window subclassing for a window. |
| [`sysdlg`](sysdlg.h) | Wrappers system dialogs calls. |
| [`syspath`](syspath.h) | Retrieves system paths. |
| [`textbox`](textbox.h) | Wrapper to native edit box control. |
| [`treeview`](treeview.h) | Wrapper to treeview control from Common Controls library. |
| [`vec`](vec.h) | Utilities to std::vector. |
| [`version`](version.h) | Parses version information from an EXE or DLL. |
| [`wnd`](wnd.h) | Simple HWND wrapper, base to all dialog and window classes. |
| [`xml`](xml.h) | XML wrapper class to MSXML2 Windows library. |
| [`zip`](zip.h) | Utilities to work with zipped files. |

## 5. License

Licensed under [MIT license](https://opensource.org/licenses/MIT), see [LICENSE.txt](LICENSE.txt) for details.
