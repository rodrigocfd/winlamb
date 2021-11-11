# WinLamb

A lightweight modern C++20 library for the [Windows API](https://en.wikipedia.org/wiki/Windows_API), using [lambdas](https://www.cprogramming.com/c++11/c++11-lambda-closures.html) to handle Windows messages.

WinLamb allows you to build native Win32 applications, quickly and scalably.

## 1. Usage

The `winlamb` directory contains all the *.h and *.cpp and sources of the library. There are no special requirements to build the *.cpp files – just add them to your build process.

WinLamb has no dependencies other than the Windows API itself.

## 2. Examples

### 2.1. Programmatically creating a window

This is a simple native Windows program written with WinLamb. Each window has a class, and there's no need to write a message loop or window registering.

* Declaration: `MyWindow.h`

```cpp
#include <winlamb/window_main.h>

class MyWindow final : public wl::window_main {
public:
    MyWindow();
};
```

* Implementation: `MyWindow.cpp`

```cpp
#include "MyWindow.h"

RUN(MyWindow) // optional, generate WinMain

MyWindow::MyWindow()
    : window_main{window_main::opts{ // initialization options

    }}
{
    on().create([this]() -> LRESULT { // WM_CREATE handling
        return 0;
    });
}
```

### 2.2. Loading a dialog resource window

Dialog resources are great because you can design them with a WYSIWYG resource editor. WinLamb offers support to create windows directly from resources:

* Declaration: `MyWindow.h`

```cpp
#include <winlamb/dialog_main.h>

class MyWindow final : public wl::dialog_main {
public:
    MyWindow();
};
```

* Implementation: `MyWindow.cpp`

```cpp
#include "MyWindow.h"

RUN(MyWindow) // optional, generate WinMain

MyWindow::MyWindow()
    : dialog_main{dialog_main::opts{ // initialization options
        dialog_id = IDD_DIALOG1,
    }}
{
    on().init_dialog([this]() -> bool { // WM_INITDIALOG handling
        return true;
    });
}
```

## 3. C++11 version

In case you need, the old C++11 version of WinLamb is archived in the [cpp11 branch](https://github.com/rodrigocfd/winlamb/tree/cpp20). It's in maintenance mode now – no new features won't be added.

## 4. License

Licensed under [MIT License](https://opensource.org/licenses/MIT), see [LICENSE.md](LICENSE.md) for details.
