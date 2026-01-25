#include "DlgMain.h"
#include "id3v2/tag.h"

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
	lstFiles.icons_16().add_shell_ext(L"doc");
	lstFiles.cols.add(L"Path", 400);
	lstFiles.cols.add(L"Padding", 80);
	return true;
}

void DlgMain::on_cancel() {
	// wl::ComPtr<IFileOpenDialog> ifod{};
	// ifod.co_create_instance(CLSID_FileOpenDialog);

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
	for (auto &&f : files) {
		auto tag = id3v2::Tag{f};
		lstFiles.items.add(f, {wl::str::fmt(L"%d", tag.padding())});
	}
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

Contro::Contro(wl::WindowParent &parent, POINT pos, SIZE size)
	: wnd{parent, wl::ControlOpts{
		.layout = wl::Lay::move_hold,
		.pos = pos,
		.size = size,
		.styleEx = wl::ControlOpts{}.styleEx | WS_EX_CLIENTEDGE,
	}}
{
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
	wnd.on().wm_create([this](wl::wm::Create p) -> int {
		lv.cols[1].set_justif(HDF_CENTER).set_width_to_fill();
		lv.items.add(L"Bronco kid", {L"Surreal"});
		lv.items.add(L"Ground control", {L"to major tom"});

		sb.icons().add_shell_ext(L"xlsx");

		tv.items.add_root(L"Xabregas").add_child(L"Donegas");
		tv.items.add_root(L"Dumpster fire");
		return 0;
	});

	btn.on().bn_clicked([this]() -> void {
		wl::WindowModal modal{wnd, wl::ModalOpts{
			.size = wl::dpi::sz(200, 200),
			.title = L"My modal",
		}};
		modal.show();
	});

	chk.on().bn_clicked([this]() -> void {
		std::wstring title = wl::str::fmt(L"Check box is %s", chk.is_checked() ? L"YES" : L"NO");
		wnd.set_title(title);
	});

	cmb.on().cbn_sel_change([this]() -> void {
		std::optional<std::wstring> selText = cmb.items.selected_text();
		if (selText.has_value()) {
			std::wstring title = wl::str::fmt(L"Selected: %s", selText.value().c_str());
			wnd.set_title(title);
		} else {
			wnd.set_title(L"No selection");
		}
	});

	dtp.on().dtn_date_time_change([this](NMDATETIMECHANGE &p) -> void {
		std::wstring title = wl::str::fmt(L"%d-%d-%d", p.st.wYear, p.st.wMonth, p.st.wDay);
		wnd.set_title(title);
	});

	txt.on().en_change([this]() -> void {
		std::wstring s = txt.text();
		wnd.set_title(s);
	});

	mcal.on().mcn_sel_change([this](NMSELCHANGE &p) -> void {
		auto s = wl::str::fmt(L"%d %d %d", p.stSelStart.wYear, p.stSelStart.wMonth, p.stSelStart.wDay);
		wnd.set_title(s);
	});

	lbl.on().stn_clicked([this]() -> void {
		wnd.set_title(L"Label clicked");
	});

	sb.on().nm_click([this](NMMOUSE &p) -> bool {
		MessageBoxW(wnd.hwnd(), L"Status bar clicked", L"Click", MB_ICONINFORMATION);
		return true;
	});

	rads.on().bn_clicked([this](int i) -> void {
		auto s = wl::str::fmt(L"Radio: %s", rads.radios[i].text().c_str());
		wnd.set_title(s);
	});

	tv.on().tvn_sel_changed([this](NMTREEVIEW&) -> void {
		wl::TreeView::Item sel = tv.items.selected();
		wnd.set_title(sel.hitem() ? sel.text() : L"No tree sel");
	});

	trb.on().wm_h_scroll([this](wl::wm::HScroll) -> void {
		auto s = wl::str::fmt(L"Track bar at %d", trb.pos());
		wnd.set_title(s);
	});

}
