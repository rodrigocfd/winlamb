# Includes

```mermaid
flowchart BT
    aux-path.hpp --> aux-str.hpp
    aux-wrap.hpp --> aux-str.hpp
    ui-app.hpp --> aux-wrap.hpp
    ui-base.hpp --> ui-app.hpp
    ui-rawdlg.hpp --> ui-base.hpp
    ui-wnd.hpp --> ui-rawdlg.hpp
    ui-wndctrl.hpp --> ui-wnd.hpp
```
