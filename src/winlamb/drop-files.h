#pragma once
#include <functional>
#include <optional>
#include <vector>
#include "lib-include-win.h"
#include <oleidl.h>
#include <shellapi.h>
#include "window-user.h"

namespace wl {

	/// @brief Implements [`IDropTarget`] COM interface, allowing file drag & drop on the window.
	///
	/// Calls [`RegisterDragDrop`], calls [`RevokeDragDrop`], and extracts the dropped files automatically.
	///
	/// Example, .h and .cpp files:
	///
	/// ```cpp
	/// class MyMain final : wl::NonMovable {
	/// public:
	///     MyMain();
	///     wl::WindowMain wnd{DLG_MAIN, ICO_MAIN};
	///     wl::DropFiles dropFiles{wnd};
	/// };
	/// ```
	///
	/// ```cpp
	/// RUN_MAIN(MyMain, wnd)
	///
	/// MyMain::MyMain() {
	///     // ...
	///
	///     dropFiles.on_drop([](const std::vector<std::wstring> &files) {
	///         // ...
	///     });
	/// }
	/// ```
	///
	/// [`IDropTarget`]: https://learn.microsoft.com/en-us/windows/win32/api/oleidl/nn-oleidl-idroptarget
	/// [`RegisterDragDrop`]: https://learn.microsoft.com/en-us/windows/win32/api/ole2/nf-ole2-registerdragdrop
	/// [`RevokeDragDrop`]: https://learn.microsoft.com/en-us/windows/win32/api/ole2/nf-ole2-revokedragdrop
	class DropFiles final : public IDropTarget, NonMovable {
	public:
		explicit DropFiles(WindowParent &owner);

		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject) override;
		ULONG STDMETHODCALLTYPE AddRef() override;
		ULONG STDMETHODCALLTYPE Release() override;

		HRESULT STDMETHODCALLTYPE DragEnter(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect) override;
		HRESULT STDMETHODCALLTYPE DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect) override;
		HRESULT STDMETHODCALLTYPE DragLeave() override;
		HRESULT STDMETHODCALLTYPE Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect) override;

		/// Defines a callback to be called when files are dropped on the window.
		///
		/// Receives a vector with the full path of each file being dropped.
		void on_drop(std::function<void(const std::vector<std::wstring>&)> cb) { _cb = std::make_optional(cb); }

	private:
		std::vector<std::wstring> get_dropped(HDROP hDrop) const;

		LONG _refCount = 1;
		std::optional<std::function<void(const std::vector<std::wstring>&)>> _cb{};
	};

}
