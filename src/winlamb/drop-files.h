#pragma once
#include <functional>
#include <optional>
#include <vector>
#include <Windows.h>
#include <oleidl.h>
#include "window-user.h"

namespace wl {

	// Implements IDropFiles COM interface, allowing file drag & dropping.
	class DropFiles final : public IDropTarget {
	public:
		DropFiles() = delete;
		DropFiles(const DropFiles&) = delete;
		DropFiles(DropFiles&&) = delete;
		DropFiles& operator=(const DropFiles&) = delete;
		DropFiles& operator=(DropFiles&&) = delete;

		explicit DropFiles(WindowParent &owner);

		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject) override;
		ULONG STDMETHODCALLTYPE AddRef() override;
		ULONG STDMETHODCALLTYPE Release() override;

		HRESULT STDMETHODCALLTYPE DragEnter(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect) override;
		HRESULT STDMETHODCALLTYPE DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect) override;
		HRESULT STDMETHODCALLTYPE DragLeave() override;
		HRESULT STDMETHODCALLTYPE Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect) override;

		void on_drop(std::function<void(const std::vector<std::wstring>&)> cb) { _cb = std::make_optional(cb); }

	private:
		std::vector<std::wstring> get_dropped(HDROP hDrop) const;

		LONG _refCount = 1;
		std::optional<std::function<void(const std::vector<std::wstring>&)>> _cb;
	};

}
