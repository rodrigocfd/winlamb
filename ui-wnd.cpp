#include <algorithm>
#include <system_error>
#include "ui-wnd.hpp"
#include "aux-com.hpp"
#include <Uxtheme.h>
#include <vsstyle.h>
#include <shellapi.h>
#include <ShlObj_core.h>
using namespace _wl_internal;
using namespace wl;

int WindowMain::run(HINSTANCE hInst, int cmdShow) {
	return _rawOrDlg.raw.has_value()
		? _rawOrDlg.raw.value().run(hInst, cmdShow)
		: _rawOrDlg.dlg.value().run(hInst, cmdShow);
}

const WindowMain& WindowMain::set_title(WStrView newTitle) const {
	set_wnd_text(hwnd(), newTitle);
	return *this;
}

////////////////////////////////////////////////////////////////////////////////

void WindowModal::show() {
	return _rawOrDlg.raw.has_value()
		? _rawOrDlg.raw.value().show()
		: _rawOrDlg.dlg.value().show();
}

const WindowModal& WindowModal::set_title(WStrView newTitle) const {
	_wl_internal::set_wnd_text(hwnd(), newTitle);
	return *this;
}

////////////////////////////////////////////////////////////////////////////////

WindowControl::WindowControl(IWindowParent &parent, ControlOpts creationOpts)
	: _rawOrDlg{.raw = std::make_optional<RawControl>(parent.base(), creationOpts)}
{
	on().wm_nc_paint(std::bind(&WindowControl::paint_custom_border, this, std::placeholders::_1));
}

WindowControl::WindowControl(IWindowParent &parent, WORD dlgId, WORD ctrlId, POINT pos, Lay layout)
	: _rawOrDlg{.dlg = std::make_optional<DlgControl>(parent.base(), dlgId, ctrlId, pos, layout)}
{
	on().wm_nc_paint(std::bind(&WindowControl::paint_custom_border, this, std::placeholders::_1));
}

WORD WindowControl::ctrl_id() const {
	return GetDlgCtrlID(hwnd());
}

void WindowControl::paint_custom_border(wm::NcPaint p) const {
	DefWindowProcW(hwnd(), WM_NCPAINT, p.wparam(), p.lparam()); // make the system draw the scrollbar for us

	DWORD exStyle = static_cast<DWORD>(GetWindowLongPtrW(hwnd(), GWL_EXSTYLE));
	bool hasBorderStyle = (exStyle & WS_EX_CLIENTEDGE) != 0;
	if (!hasBorderStyle || !IsThemeActive() || !IsAppThemed())
		return; // no theme borders to be painted

	RECT rc{};
	GetWindowRect(hwnd(), &rc); // control window outmost coordinates, including margins
	screen_to_client_rc(hwnd(), &rc);
	OffsetRect(&rc, 2, 2); // because it comes up anchored at -2,-2

	struct Handles final {
		~Handles() {
			if (hTheme) CloseThemeData(hTheme); // make sure resources will be freed
			if (hRgnClip) DeleteObject(hRgnClip);
			if (hRgnHole) DeleteObject(hRgnHole);
			if (hdc) ReleaseDC(hWnd, hdc);
		}
		HWND hWnd;
		HDC hdc = nullptr;
		HRGN hRgnHole = nullptr;
		HRGN hRgnClip = nullptr;
		HTHEME hTheme = nullptr;
	} handles{.hWnd = hwnd()};

	handles.hdc = GetWindowDC(hwnd());

	// The HRGN which comes in WM_NCPAINT seems to be invalid, so we carve our own.
	RECT rcHole = rc;
	InflateRect(&rcHole, -2, -2);
	handles.hRgnHole = CreateRectRgnIndirect(&rcHole);
	handles.hRgnClip = CreateRectRgnIndirect(&rc);
	CombineRgn(handles.hRgnClip, handles.hRgnClip, handles.hRgnHole, RGN_DIFF);
	SelectClipRgn(handles.hdc, handles.hRgnClip);

	handles.hTheme = OpenThemeData(hwnd(), L"LISTVIEW");
	DrawThemeBackground(handles.hTheme, handles.hdc, LVP_LISTDETAIL, 0, &rc, nullptr);
}

////////////////////////////////////////////////////////////////////////////////

DropFiles::DropFiles(IWindowParent &owner) {
	owner.base()._preEvents.wm_create_or_init_dialog([this, pOwner = &owner]() -> void {
		RegisterDragDrop(pOwner->hwnd(), this);
	});

	owner.base()._postEvents.wm(WM_DESTROY, [pOwner = &owner](wm::Msg) -> void {
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
	if (_cb.has_value()) { // do we have an user callback?
		FORMATETC fetc{
			.cfFormat = CF_HDROP,
			.ptd = nullptr,
			.dwAspect = DVASPECT_CONTENT,
			.lindex = -1,
			.tymed = TYMED_HGLOBAL,
		};
		STGMEDIUM medium{};

		if (HRESULT hr = pDataObj->GetData(&fetc, &medium); FAILED(hr)) [[unlikely]] {
			*pdwEffect = DROPEFFECT_NONE;
			return hr;
		}

		HGLOBAL hFiles = medium.hGlobal;
		HDROP hDrop = reinterpret_cast<HDROP>(GlobalLock(hFiles));
		std::vector<std::wstring> files = get_dropped(hDrop);
		GlobalUnlock(hFiles);
		ReleaseStgMedium(&medium);

		_cb.value()(files); // invoke user callback
	}

	*pdwEffect &= DROPEFFECT_COPY;
	return S_OK;
}

std::vector<std::wstring> DropFiles::get_dropped(HANDLE hDrop) const {
	UINT count = DragQueryFileW(reinterpret_cast<HDROP>(hDrop), 0xffff'ffff, nullptr, 0);
	std::vector<std::wstring> paths;
	paths.reserve(count);

	for (UINT i = 0; i < count; ++i) {
		wchar_t buf[MAX_PATH + 1] = {L'\0'};
		DragQueryFileW(reinterpret_cast<HDROP>(hDrop), i, buf, MAX_PATH + 1);
		paths.emplace_back(buf);
	}

	//DragFinish(hDrop); // will crash ReleaseStgMedium()
	return paths;
}

////////////////////////////////////////////////////////////////////////////////

static bool msg_box_build(const IWindowParent &parent, WStrView title, WStrView caption, WStrView body,
	LPWSTR icon, bool isAsk, WStrView okText)
{
	TASKDIALOG_BUTTON customBtns[] = {
		{.nButtonID = IDOK, .pszButtonText = okText.c_str()},
		{.nButtonID = IDCANCEL, .pszButtonText = L"&Cancel"},
	};
	UINT numBtns = isAsk ? 2 : 1;

	TASKDIALOGCONFIG tdc{
		.cbSize = sizeof(TASKDIALOGCONFIG),
		.hwndParent = parent.hwnd(),
		.dwFlags = TDF_ALLOW_DIALOG_CANCELLATION | TDF_POSITION_RELATIVE_TO_WINDOW,
		.pszWindowTitle = title.c_str(),
		.pszMainIcon = icon,
		.pszMainInstruction = caption.c_str(),
		.pszContent = body.c_str(),
		.cButtons = numBtns,
		.pButtons = customBtns,
	};

	int pnButton = 0, pnRadioButton = 0;
	BOOL pfChecked = FALSE;
	HRESULT hr = TaskDialogIndirect(&tdc, &pnButton, &pnRadioButton, &pfChecked);
	#ifdef _DEBUG
	if (FAILED(hr))
		throw std::system_error(hr, std::system_category(), "TaskDialogIndirect failed");
	#endif

	return pnButton == IDOK;
}

void wl::msg_box::info(const IWindowParent &parent, WStrView body) {
	msg_box_build(parent, wnd_text(parent.hwnd()), nullptr, body, TD_INFORMATION_ICON, false, L"&OK");
}

void wl::msg_box::info(const IWindowParent &parent, WStrView title, WStrView body) {
	msg_box_build(parent, title, nullptr, body, TD_INFORMATION_ICON, false, L"&OK");
}

void wl::msg_box::info(const IWindowParent &parent, WStrView title, WStrView caption, WStrView body) {
	msg_box_build(parent, title, caption, body, TD_INFORMATION_ICON, false, L"&OK");
}

void wl::msg_box::warn(const IWindowParent &parent, WStrView body) {
	msg_box_build(parent, wnd_text(parent.hwnd()), nullptr, body, TD_WARNING_ICON, false, L"&OK");
}

void wl::msg_box::warn(const IWindowParent &parent, WStrView title, WStrView body) {
	msg_box_build(parent, title, nullptr, body, TD_WARNING_ICON, false, L"&OK");
}

void wl::msg_box::warn(const IWindowParent &parent, WStrView title, WStrView caption, WStrView body) {
	msg_box_build(parent, title, caption, body, TD_WARNING_ICON, false, L"&OK");
}

void wl::msg_box::err(const IWindowParent &parent, WStrView body) {
	msg_box_build(parent, wnd_text(parent.hwnd()), nullptr, body, TD_ERROR_ICON, false, L"&OK");
}

void wl::msg_box::err(const IWindowParent &parent, WStrView title, WStrView body) {
	msg_box_build(parent, title, nullptr, body, TD_ERROR_ICON, false, L"&OK");
}

void wl::msg_box::err(const IWindowParent &parent, WStrView title, WStrView caption, WStrView body) {
	msg_box_build(parent, title, caption, body, TD_ERROR_ICON, false, L"&OK");
}

bool wl::msg_box::ask(const IWindowParent &parent, WStrView body) {
	return msg_box_build(parent, wnd_text(parent.hwnd()), nullptr, body, TD_WARNING_ICON, true, L"&OK");
}

bool wl::msg_box::ask(const IWindowParent &parent, WStrView title, WStrView body) {
	return msg_box_build(parent, title, nullptr, body, TD_WARNING_ICON, true, L"&OK");
}

bool wl::msg_box::ask(const IWindowParent &parent, WStrView title, WStrView caption, WStrView body, WStrView okText) {
	return msg_box_build(parent, title, caption, body, TD_WARNING_ICON, true, okText);
}

[[nodiscard]] static std::wstring shellitem_dn(const ComPtr<IShellItem> &shi) {
	wchar_t *pRecv = nullptr;
	shi->GetDisplayName(SIGDN_FILESYSPATH, &pRecv);
	std::wstring recv{pRecv};
	CoTaskMemFree(pRecv);
	return recv;
}

static void fd_set_opts(IFileDialog *pFd, FILEOPENDIALOGOPTIONS opts) {
	FILEOPENDIALOGOPTIONS defOpts;
	pFd->GetOptions(&defOpts);
	pFd->SetOptions(defOpts | FOS_FORCEFILESYSTEM | FOS_FILEMUSTEXIST | opts);
}

static void fd_set_types(IFileDialog *pFd, std::initializer_list<std::pair<WStrView, WStrView>> fileTypes) {
	std::vector<COMDLG_FILTERSPEC> rawTypes;
	rawTypes.reserve(fileTypes.size());
	for (auto &&fileType : fileTypes)
		rawTypes.emplace_back(fileType.first.c_str(), fileType.second.c_str());
	pFd->SetFileTypes(static_cast<UINT>(rawTypes.size()), rawTypes.data());
	pFd->SetFileTypeIndex(1); // 1-based
}

[[nodiscard]] static bool fd_open(IFileDialog *pFd, const IWindowParent &parent) {
	HRESULT hr = pFd->Show(parent.hwnd());
	if (hr == HRESULT_FROM_WIN32(ERROR_CANCELLED)) {
		return false;
	} else if (SUCCEEDED(hr)) {
		return true;
	} else {
		throw std::system_error(hr, std::system_category(), "IModalWindow::Show failed");
	}
}

[[nodiscard]] static std::wstring fd_get_file(IFileDialog *pFd) {
	ComPtr<IShellItem> shi{};
	pFd->GetResult(shi.pptr());
	return shellitem_dn(shi);
}

std::optional<std::wstring> wl::msg_box::open_file(const IWindowParent &parent,
	std::initializer_list<std::pair<WStrView, WStrView>> fileTypes)
{
	ComPtr<IFileOpenDialog> ifod{};
	ifod.co_create_instance(CLSID_FileOpenDialog);
	fd_set_types(ifod.ptr(), fileTypes);
	return fd_open(ifod.ptr(), parent) ? std::make_optional(fd_get_file(ifod.ptr())) : std::nullopt;
}

std::vector<std::wstring> wl::msg_box::open_files(const IWindowParent &parent,
	std::initializer_list<std::pair<WStrView, WStrView>> fileTypes)
{
	ComPtr<IFileOpenDialog> ifod{};
	ifod.co_create_instance(CLSID_FileOpenDialog);
	fd_set_opts(ifod.ptr(), FOS_ALLOWMULTISELECT);
	fd_set_types(ifod.ptr(), fileTypes);
	if (fd_open(ifod.ptr(), parent)) {
		ComPtr<IShellItemArray> sharr{};
		ifod->GetResults(sharr.pptr());

		DWORD count = 0;
		sharr->GetCount(&count);
		std::vector<std::wstring> strs{};
		strs.reserve(count);

		for (DWORD i = 0; i < count; ++i) {
			ComPtr<IShellItem> shi{};
			sharr->GetItemAt(i, shi.pptr());
			strs.emplace_back(shellitem_dn(shi));
		}
		std::sort(strs.begin(), strs.end(), [](const auto &a, const auto &b) -> bool {
			return str::cmp_i(a, b) < 1;
		});
		return strs;
	}
	return {}; // user cancelled
}

std::optional<std::wstring> wl::msg_box::open_folder(const IWindowParent &parent) {
	ComPtr<IFileOpenDialog> ifod{};
	ifod.co_create_instance(CLSID_FileOpenDialog);
	fd_set_opts(ifod.ptr(), FOS_PICKFOLDERS);
	return fd_open(ifod.ptr(), parent) ? std::make_optional(fd_get_file(ifod.ptr())) : std::nullopt;
}

std::optional<std::wstring> wl::msg_box::save_file(const IWindowParent &parent,
	std::initializer_list<std::pair<WStrView, WStrView>> fileTypes)
{
	ComPtr<IFileSaveDialog> ifsd{};
	ifsd.co_create_instance(CLSID_FileSaveDialog);
	fd_set_types(ifsd.ptr(), fileTypes);
	return fd_open(ifsd.ptr(), parent) ? std::make_optional(fd_get_file(ifsd.ptr())) : std::nullopt;
}
