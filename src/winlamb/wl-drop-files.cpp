#include "drop-files.h"
using namespace wl;

DropFiles::DropFiles(WindowMain& owner) {
	if (owner.wnd_msg()._isDlg) {
		owner.wnd_msg()._preEvents.wm_init_dialog([this, pOwner = &owner](wm::InitDialog) {
			RegisterDragDrop(pOwner->hwnd(), this);
			return false; // ignored
		});
	} else {
		owner.wnd_msg()._preEvents.wm_create([this, pOwner = &owner](wm::Create) {
			RegisterDragDrop(pOwner->hwnd(), this);
			return false; // ignored
		});
	}

	owner.wnd_msg()._postEvents.wm_destroy([pOwner = &owner]() {
		RevokeDragDrop(pOwner->hwnd());
	});
}

HRESULT STDMETHODCALLTYPE DropFiles::QueryInterface(REFIID riid, void **ppvObject) {
	if (riid == IID_IDropTarget || riid == IID_IUnknown) {
		AddRef();
		*ppvObject = this;
		return S_OK;
	} else {
		*ppvObject = nullptr;
		return E_NOINTERFACE;
	}
}

ULONG STDMETHODCALLTYPE DropFiles::AddRef() {
	return InterlockedIncrement(&_refCount);
}

ULONG STDMETHODCALLTYPE DropFiles::Release() {
	ULONG c = InterlockedDecrement(&_refCount);
	if (!_refCount) delete this;
	return c;
}

HRESULT STDMETHODCALLTYPE DropFiles::DragEnter(
	IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
	*pdwEffect &= DROPEFFECT_COPY;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DropFiles::DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect) {
	*pdwEffect &= DROPEFFECT_COPY;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DropFiles::DragLeave() {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DropFiles::Drop(
	IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
	if (_cb.has_value()) {
		FORMATETC fetc{
			.cfFormat = CF_HDROP,
			.ptd = nullptr,
			.dwAspect = DVASPECT_CONTENT,
			.lindex = -1,
			.tymed = TYMED_HGLOBAL,
		};
		STGMEDIUM medium{};

		if (HRESULT hr = pDataObj->GetData(&fetc, &medium); FAILED(hr)) {
			*pdwEffect = DROPEFFECT_NONE;
			return hr;
		}

		HGLOBAL hFiles = medium.hGlobal;
		auto hDrop = reinterpret_cast<HDROP>(GlobalLock(hFiles));
		auto files = get_dropped(hDrop);
		GlobalUnlock(hFiles);
		ReleaseStgMedium(&medium);

		_cb.value()(files);
	}

	*pdwEffect &= DROPEFFECT_COPY;
	return S_OK;
}

std::vector<std::wstring> DropFiles::get_dropped(HDROP hDrop) const {
	UINT count = DragQueryFileW(hDrop, 0xffff'ffff, nullptr, 0);
	std::vector<std::wstring> paths;
	paths.reserve(count);

	for (UINT i = 0; i < count; ++i) {
		WCHAR buf[MAX_PATH + 1] = {L'\0'};
		DragQueryFileW(hDrop, i, buf, MAX_PATH + 1);
		paths.emplace_back(buf);
	}

	//DragFinish(hDrop); // will crash ReleaseStgMedium()
	return paths;
}
