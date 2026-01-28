# Includes

```mermaid
flowchart BT
    ui-app.hpp --> aux-str.hpp
    ui-base.hpp --> ui-app.hpp
    ui-rawdlg.hpp --> ui-base.hpp
    ui-wnd.hpp --> ui-rawdlg.hpp
    ui-wndctrl.hpp --> ui-wnd.hpp
```
