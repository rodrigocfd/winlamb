/** @brief WinLamb library namespace. */
namespace wl { }

/// @brief Native window and control events.
///
/// You don't instantiate these classes manually, they are exposed by native windows and controls.
namespace wl::events { }

/// @brief Options to create windows and controls programmatically with [`CreateWindowEx`].
///
/// [`CreateWindowEx`]: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
namespace wl::opts { }

#include "lib-include-win.h"

#include "com-ptr.h"
#include "ctl-list-view.h"
#include "drop-files.h"
#include "events.h"
#include "file.h"
#include "image-list.h"
#include "layout.h"
#include "path.h"
#include "runnable.h"
#include "str.h"
#include "vec.h"
#include "window-dlg.h"
#include "window-raw.h"
#include "window-user.h"
#include "window.h"
