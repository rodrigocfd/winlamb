#include "DlgMain.h"

// RUN_MAIN(DlgMain, wnd)
RUN_MAIN(RawMain, wnd)

DlgMain::DlgMain() {
	wnd.on().wm_init_dialog(std::bind(&DlgMain::on_init_dialog, this, std::placeholders::_1));
	wnd.on().wm_command(IDCANCEL, std::bind(&DlgMain::on_cancel, this));
	dropFiles.on_drop(std::bind(&DlgMain::on_drop_files, this, std::placeholders::_1));
	lstFiles.on().lvn_item_changed(std::bind(&DlgMain::on_lst_files_item_changed, this, std::placeholders::_1));
	wnd.on().wm_command(MNU_FILES_ABOUT, std::bind(&DlgMain::on_about, this));

	// dlg.on().wm_notify(0x333, LVN_DELETEITEM, [this](wl::wm::Notify n) {
	// 	auto p = n.pHdr<NMLINK>();
	// 	return 0;
	// });
}

bool DlgMain::on_init_dialog(wl::wm::InitDialog) {
	lstFiles.set_extended_style(true, LVS_EX_FULLROWSELECT);
	lstFiles.image_list_16().add_shell_ext(L"doc");

	lstFiles.cols.add(L"Faster", 300);
	lstFiles.cols.add(L"Rapids", 300);

	lstFiles.items.add(L"Line", {L"Killings"}, 0);
	lstFiles.items.add(L"Neither heaven", {L"nor space"}, 0);
	return true;
}

void DlgMain::on_cancel() {
	// wl::ComPtr<IFileOpenDialog> ifod{CLSID_FileOpenDialog};

	// FILEOPENDIALOGOPTIONS defOpts;
	// ifod->GetOptions(&defOpts);
	// ifod->SetOptions(defOpts | FOS_FORCEFILESYSTEM | FOS_FILEMUSTEXIST | FOS_ALLOWMULTISELECT);

	// COMDLG_FILTERSPEC ftypes[] = {
	// 	{L"MP3 files", L"*.mp3"},
	// 	{L"All files", L"*.*"},
	// };
	// ifod->SetFileTypes(ARRAYSIZE(ftypes), ftypes);
	// ifod->SetFileTypeIndex(1);

	// HRESULT hrShow = ifod->Show(nullptr);
	// if (hrShow == HRESULT_FROM_WIN32(ERROR_CANCELLED)) {
	// 	OutputDebugStringW(L"Cancelled.\n");
	// } else if (SUCCEEDED(hrShow)) {
	// 	OutputDebugStringW(L"OKAY.\n");
	// } else  {
	// 	MessageBoxW(nullptr, L"File open failed.", L"Error", MB_ICONERROR);
	// }

	PostMessageW(wnd.hwnd(), WM_CLOSE, 0, 0);
}

void DlgMain::on_drop_files(const std::vector<std::wstring> files) {
	lstFiles.items.delete_all();
	for (auto &&f : files)
		lstFiles.items.add(f);
}

void DlgMain::on_lst_files_item_changed(NMLISTVIEW &p) {
	wnd.set_title(!lstFiles.items.selected_count()
		? L"NO SEL" : lstFiles.items[p.iItem].text());
}

void DlgMain::on_about() {
	// MessageBoxW(wnd.hwnd(), L"Hello", L"Title", MB_ICONINFORMATION);

	wl::WindowModal pop{wnd, DLG_MAIN};
	pop.on().wm_init_dialog([&pop](wl::wm::InitDialog) {
		SetWindowPos(pop.hwnd(), nullptr, 0, 0, 400, 200, SWP_NOZORDER | SWP_NOMOVE);
		return true;
	});
	pop.show();
}

////////////////////////////////////////////////////////////////////////////////

Contro::Contro(wl::WindowParent &parent)
	: wnd{parent}
{
	wnd.setup().windowExStyle |= WS_EX_CLIENTEDGE;
	wnd.setup().pos = wl::dpi::pt(420, 10);
	wnd.setup().size = wl::dpi::sz(80, 80);
	wnd.setup().layout = wl::Lay::move_hold;

	wnd.on().wm_paint(std::bind(&Contro::on_paint, this));


}

void Contro::on_paint() {
	PAINTSTRUCT ps{};
	HDC hdc = BeginPaint(wnd.hwnd(), &ps);
	LineTo(hdc, 60, 60);
	EndPaint(wnd.hwnd(), &ps);
}

////////////////////////////////////////////////////////////////////////////////

RawMain::RawMain() {
	wnd.setup().iconId = ICO_MAIN;
	wnd.setup().title = L"My main window";
	wnd.setup().size = wl::dpi::sz(550, 300);
	wnd.setup().style |= WS_SIZEBOX | WS_MAXIMIZEBOX;

	btn.setup().pos = wl::dpi::pt(10, 10);
	btn.setup().text = L"&Click me";

	chk.setup().pos = wl::dpi::pt(110, 10);
	chk.setup().text = L"&Check me";

	lv.setup().pos = wl::dpi::pt(10, 50);
	lv.setup().size = wl::dpi::sz(400, 200);
	lv.setup().layout = wl::Lay::resize_resize;
	lv.setup().contextMenuId = MNU_FILES;

	wnd.on().wm_create([this](wl::wm::Create p) -> int {
		lv.cols.add(L"First", wl::dpi::x(200));
		lv.cols.add(L"Second", 1).set_justif(HDF_CENTER).set_width_to_fill();
		lv.items.add(L"Bronco kid", {L"Surreal"});
		lv.items.add(L"Ground control", {L"to major tom"});
		return 0;
	});

	btn.on().bn_clicked([this]() -> void {
		MessageBoxW(wnd.hwnd(), L"Button clicked", L"Click", MB_ICONINFORMATION);
	});

	chk.on().bn_clicked([this]() -> void {
		std::wstring title = wl::str::fmt(L"Check box is %s", chk.is_checked() ? L"YES" : L"NO");
		wnd.set_title(title);
	});

}
