#include "DlgMain.h"

// RUN_MAIN(DlgMain, wnd)
RUN_MAIN(RawMain, wnd)

DlgMain::DlgMain() {
	wnd.on().wm_init_dialog(std::bind(&DlgMain::on_init_dialog, this, std::placeholders::_1));
	wnd.on().wm_command(IDCANCEL, std::bind(&DlgMain::on_cancel, this));
	dropFiles.on_drop(std::bind(&DlgMain::on_drop_files, this, std::placeholders::_1));
	lstFiles.on().lvn_item_changed(std::bind(&DlgMain::on_lst_files_item_changed, this, std::placeholders::_1));
	wnd.on().wm_command(MNU_FILES_ABOUT, std::bind(&DlgMain::on_about, this));

	dropFiles.on_drop([](const std::vector<std::wstring> &files) {

	});

	// dlg.on().wm_notify(0x333, LVN_DELETEITEM, [this](wl::wm::Notify n) {
	// 	auto p = n.pHdr<NMLINK>();
	// 	return 0;
	// });
}

bool DlgMain::on_init_dialog(wl::wm::InitDialog) {
	lstFiles.set_extended_style(true, LVS_EX_FULLROWSELECT);
	lstFiles.cols.add(L"Faster", 300);
	lstFiles.cols.add(L"Rapids", 300);

	lstFiles.items.add(L"Line", {L"Killings"});
	lstFiles.items.add(L"Neither heaven", {L"nor space"});
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

wl::opts::Control ctlOpts{
	.pos    = wl::dpi::pt(420, 10),
	.size   = wl::dpi::sz(80, 80),
	.layout = wl::Lay::repos_none,
};

Contro::Contro(wl::WindowParent &parent)
	: wnd{parent, ctlOpts}
{
}

////////////////////////////////////////////////////////////////////////////////

wl::opts::Main wndOpts{
	.iconId = ICO_MAIN,
	.title  = L"Together",
	.size   = wl::dpi::sz(550, 300),
	.style  = wl::opts::Main{}.style | WS_SIZEBOX | WS_MAXIMIZEBOX,
};
wl::opts::ListView lstOpts{
	.pos    = wl::dpi::pt(10, 10),
	.size   = wl::dpi::sz(400, 200),
	.ctrlId = LST_FILES,
	.layout = wl::Lay::resize_resize,
};

RawMain::RawMain()
	: wnd{wndOpts}, lst{wnd, lstOpts}
{
	wnd.on().wm_create(std::bind(&RawMain::on_create, this, std::placeholders::_1));



}

#include "id3v2/tag.h"

int RawMain::on_create(wl::wm::Create) {
	lst.cols.add(L"First", 200);
	lst.cols.add(L"Second", 1).set_justif(HDF_CENTER).set_width_to_fill();
	lst.items.add(L"Bronco kid", {L"Surreal"});
	lst.items.add(L"Ground control", {L"to major tom"});

	// id3v2::Tag tag{L"D:\\Music\\mp3\\Eagles - 1976 - Hotel California\\01 Eagles - Hotel California.mp3"};
	id3v2::Tag tag{L"D:\\Music\\mp3\\Megadeth - 1988 - So Far, So Good\\01 Megadeth - Into the Lungs of Hell.mp3"};


	wnd.set_title(L"Shat out of hell");
	return 0;
}
