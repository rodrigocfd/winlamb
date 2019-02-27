/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <algorithm>
#include <system_error>
#include "com.h"
#include "str.h"
#include "wnd.h"
#include <Shlobj.h>

namespace wl {

// Wrappers system dialogs calls.
class sysdlg final {
private:
	sysdlg() = delete;

	template<typename T>
	struct _static_holder final {
		static T val; // http://stackoverflow.com/a/11709860
	};

	static _static_holder<HHOOK> _hHookMsgBox;
	static _static_holder<HWND>  _hWndParent;

public:
	// Ordinary MessageBox, centered at parent.
	static int msgbox(HWND hParent, const std::wstring& title, const std::wstring& text, UINT uType = 0) {
		if (hParent) { // the hook will center the messagebox window on parent
			_hWndParent.val = hParent;
			_hHookMsgBox.val = SetWindowsHookExW(WH_CBT, [](int code, WPARAM wp, LPARAM lp) noexcept->LRESULT {
				// http://www.codeguru.com/cpp/w-p/win32/messagebox/print.php/c4541
				if (code == HCBT_ACTIVATE) {
					HWND hMsgbox = reinterpret_cast<HWND>(wp);
					RECT rcMsgbox{}, rcParent{};

					if (hMsgbox && _hWndParent.val && GetWindowRect(hMsgbox, &rcMsgbox) && GetWindowRect(_hWndParent.val, &rcParent)) {
						RECT  rcScreen{};
						POINT pos{};
						SystemParametersInfoW(SPI_GETWORKAREA, 0, static_cast<PVOID>(&rcScreen), 0); // size of desktop

						// Adjusted x,y coordinates to message box window.
						pos.x = rcParent.left + (rcParent.right - rcParent.left) / 2 - (rcMsgbox.right - rcMsgbox.left) / 2;
						pos.y = rcParent.top + (rcParent.bottom - rcParent.top) / 2 - (rcMsgbox.bottom - rcMsgbox.top) / 2;

						// Screen out-of-bounds corrections.
						if (pos.x < 0) {
							pos.x = 0;
						} else if (pos.x + (rcMsgbox.right - rcMsgbox.left) > rcScreen.right) {
							pos.x = rcScreen.right - (rcMsgbox.right - rcMsgbox.left);
						}
						if (pos.y < 0) {
							pos.y = 0;
						} else if (pos.y + (rcMsgbox.bottom - rcMsgbox.top) > rcScreen.bottom) {
							pos.y = rcScreen.bottom - (rcMsgbox.bottom - rcMsgbox.top);
						}
						MoveWindow(hMsgbox, pos.x, pos.y,
							rcMsgbox.right - rcMsgbox.left, rcMsgbox.bottom - rcMsgbox.top,
							FALSE);
					}
					UnhookWindowsHookEx(_hHookMsgBox.val); // release hook
				}
				return CallNextHookEx(nullptr, code, wp, lp);
			}, nullptr, GetCurrentThreadId());

			if (!_hHookMsgBox.val) {
				throw std::system_error(GetLastError(), std::system_category(),
					"SetWindowsHookEx failed for message box");
			}
		}
		return MessageBoxW(hParent, text.c_str(), title.c_str(), uType);
	}

	// Ordinary MessageBox, centered at parent.
	static int msgbox(const wnd* parent, const std::wstring& title, const std::wstring& text, UINT uType = 0) {
		return msgbox(parent->hwnd(), title, text, uType);
	}

private:
	static std::vector<wchar_t> _format_file_filter(const wchar_t* filterWithPipes) {
		// Input filter follows same C# syntax:
		// L"Text Files (*.txt)|*.txt|All Files (*.*)|*.*"
		std::vector<wchar_t> ret(lstrlenW(filterWithPipes) + 2, L'\0'); // two terminating nulls
		for (size_t i = 0; i < ret.size() - 1; ++i) {
			ret[i] = (filterWithPipes[i] != L'|') ? filterWithPipes[i] : L'\0';
		}
		return ret;
	}

public:
	// System dialog to select one file to be opened.
	static bool open_file(HWND hParent, const wchar_t* filterWithPipes, std::wstring& buf) {
		OPENFILENAME         ofn{};
		wchar_t              tmpBuf[MAX_PATH]{};
		std::vector<wchar_t> zfilter = _format_file_filter(filterWithPipes);

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner   = hParent;
		ofn.lpstrFilter = &zfilter[0];
		ofn.lpstrFile   = tmpBuf;
		ofn.nMaxFile    = ARRAYSIZE(tmpBuf);
		ofn.Flags       = OFN_EXPLORER | OFN_ENABLESIZING | OFN_FILEMUSTEXIST;// | OFN_HIDEREADONLY;

		bool ret = GetOpenFileNameW(&ofn) != 0;
		if (ret) buf = tmpBuf;
		return ret;
	}

	// System dialog to select one file to be opened.
	static bool open_file(const wnd* parent, const wchar_t* filterWithPipes, std::wstring& buf) {
		return open_file(parent->hwnd(), filterWithPipes, buf);
	}

	// System dialog to select many files to be opened.
	static bool open_files(HWND hParent, const wchar_t* filterWithPipes, std::vector<std::wstring>& arrBuf) {
		OPENFILENAME         ofn{};
		std::vector<wchar_t> multiBuf(65536, L'\0'); // http://www.askjf.com/?q=2179s http://www.askjf.com/?q=2181s
		std::vector<wchar_t> zfilter = _format_file_filter(filterWithPipes);
		arrBuf.clear();

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner   = hParent;
		ofn.lpstrFilter = &zfilter[0];
		ofn.lpstrFile   = &multiBuf[0];
		ofn.nMaxFile    = static_cast<DWORD>(multiBuf.size()); // including terminating null
		ofn.Flags       = OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER | OFN_ENABLESIZING;
		//ofn.FlagsEx = OFN_EX_NOPLACESBAR;
		// Call to GetOpenFileName() causes "First-chance exception (KernelBase.dll): The RPC server is unavailable."
		// in debug mode, but nothing else happens. The only way to get rid of it was using OFN_EX_NOPLACESBAR flag,
		// don't know why!

		if (!GetOpenFileNameW(&ofn)) {
			DWORD err = CommDlgExtendedError();
			if (err == FNERR_BUFFERTOOSMALL) {
				throw std::system_error(err, std::system_category(),
					"GetOpenFileName failed, buffer is too small");
			}
			return false;
		}

		std::vector<std::wstring> strs = str::split_multi_zero(&multiBuf[0]);
		if (strs.empty()) {
			throw std::runtime_error("GetOpenFileName didn't return multiple strings.");
		}

		if (strs.size() == 1) { // if user selected only 1 file, the string is the full path, and that's all
			arrBuf.emplace_back(strs[0]);
		} else { // user selected 2 or more files
			std::wstring& basePath = strs[0]; // 1st string is the base path; others are the filenames
			arrBuf.resize(strs.size() - 1); // alloc return buffer

			for (size_t i = 0; i < strs.size() - 1; ++i) {
				arrBuf[i].reserve(basePath.length() + strs[i + 1].size() + 1); // room for backslash
				arrBuf[i] = basePath;
				arrBuf[i].append(L"\\").append(strs[i + 1]); // concat folder + file
			}
			std::sort(arrBuf.begin(), arrBuf.end());
		}
		return true;
	}

	// System dialog to select many files to be opened.
	static bool open_files(const wnd* parent, const wchar_t* filterWithPipes, std::vector<std::wstring>& arrBuf) {
		return open_files(parent->hwnd(), filterWithPipes, arrBuf);
	}

	// System dialog to select where one file will be saved.
	static bool save_file(HWND hParent, const wchar_t* filterWithPipes, std::wstring& buf, const std::wstring& defFile) {
		OPENFILENAME         ofn{};
		wchar_t              tmpBuf[MAX_PATH]{};
		std::vector<wchar_t> zfilter = _format_file_filter(filterWithPipes);

		if (!defFile.empty()) lstrcpyW(tmpBuf, defFile.c_str());

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner   = hParent;
		ofn.lpstrFilter = &zfilter[0];
		ofn.lpstrFile   = tmpBuf;
		ofn.nMaxFile    = ARRAYSIZE(tmpBuf);
		ofn.Flags       = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
		ofn.lpstrDefExt = L"txt"; // apparently could be anything, will just force append of combo selected extension

		bool ret = GetSaveFileNameW(&ofn) != 0;
		if (ret) buf = tmpBuf;
		return ret;
	}

	// System dialog to select where one file will be saved.
	static bool save_file(const wnd* parent, const wchar_t* filterWithPipes, std::wstring& buf, const std::wstring& defFile) {
		return save_file(parent->hwnd(), filterWithPipes, buf, defFile);
	}

	// System dialog to select one folder.
	static bool choose_folder(HWND hParent, std::wstring& buf) {
		com::lib comLib{com::lib::init::NOW};
		//LPITEMIDLIST pidlRoot = 0;
		//if (defFolder) SHParseDisplayName(defFolder, nullptr, &pidlRoot, 0, nullptr);

		BROWSEINFOW bi{};
		bi.hwndOwner = hParent;
		bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

		PIDLIST_ABSOLUTE pidl = SHBrowseForFolderW(&bi);
		if (!pidl) return false; // user cancelled

		wchar_t tmpbuf[MAX_PATH]{};
		if (!SHGetPathFromIDListW(pidl, tmpbuf)) {
			throw std::runtime_error("SHGetPathFromIDList failed.");
		}

		buf = tmpbuf;
		return true;
	}

	// System dialog to select one folder.
	static bool choose_folder(const wnd* parent, std::wstring& buf) {
		return choose_folder(parent->hwnd(), buf);
	}
};

template<typename T> T sysdlg::_static_holder<T>::val;

}//namespace wl