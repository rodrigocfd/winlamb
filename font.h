/**
 * Part of WinLamb - Win32 API Lambda Library
 * @author Rodrigo Cesar de Freitas Dias
 * @see https://github.com/rodrigocfd/winlamb
 */

#pragma once
#include <utility>
#include <Windows.h>

namespace wl {

// Wrapper to HFONT.
class font final {
private:
	HFONT _hFont;
public:
	enum class deco { NONE, BOLD, ITALIC, BOLD_ITALIC };

	~font()        { this->release(); }
	font()         : _hFont(nullptr)  { }
	font(font&& f) : _hFont(f._hFont) { f._hFont = nullptr; }

	HFONT hfont() const { return this->_hFont; }

	font& operator=(font&& f) {
		std::swap(this->_hFont, f._hFont);
		return *this;
	}

	font& release() {
		if (this->_hFont) {
			DeleteObject(this->_hFont);
			this->_hFont = nullptr;
		}
		return *this;
	}

	font& create(const LOGFONT& lf) {
		this->release();
		this->_hFont = CreateFontIndirectW(&lf);
		return *this;
	}

	font& create(const wchar_t* fontName, int size, deco style = deco::NONE) {
		this->release();
		LOGFONT lf = { 0 };
		lstrcpyW(lf.lfFaceName, fontName);
		lf.lfHeight = -(size + 3);
		lf.lfWeight = style == deco::BOLD || style == deco::BOLD_ITALIC ? FW_BOLD : FW_DONTCARE;
		lf.lfItalic = style == deco::ITALIC || style == deco::BOLD_ITALIC ? TRUE : FALSE;
		return this->create(lf);
	}

	font& create_ui() {
		OSVERSIONINFO ovi = { 0 };
		ovi.dwOSVersionInfoSize = sizeof(ovi);

		#pragma warning (disable: 4996)
		// http://www.codeproject.com/Articles/678606/Part-Overcoming-Windows-s-deprecation-of-GetVe
		GetVersionExW(&ovi);
		#pragma warning (default: 4996)

		NONCLIENTMETRICS ncm = { 0 };
		ncm.cbSize = sizeof(ncm);
		if (ovi.dwMajorVersion < 6) { // below Vista
			ncm.cbSize -= sizeof(ncm.iBorderWidth);
		}
		SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0);
		return this->create(ncm.lfMenuFont); // Tahoma/Segoe
	}

	static void set_ui_on_children(HWND hParent) {
		static font oneFont; // keep one single font instance for all windows
		if (!oneFont._hFont) oneFont.create_ui();

		SendMessageW(hParent, WM_SETFONT,
			reinterpret_cast<WPARAM>(oneFont._hFont),
			MAKELPARAM(FALSE, 0));
		EnumChildWindows(hParent, [](HWND hWnd, LPARAM lp)->BOOL {
			SendMessageW(hWnd, WM_SETFONT,
				reinterpret_cast<WPARAM>(reinterpret_cast<HFONT>(lp)),
				MAKELPARAM(FALSE, 0)); // will run on each child
			return TRUE;
		}, reinterpret_cast<LPARAM>(oneFont._hFont));
	}

	static bool exists(const wchar_t* fontName) {
		// http://cboard.cprogramming.com/windows-programming/90066-how-determine-if-font-support-unicode.html
		bool isInstalled = false;
		HDC hdc = GetDC(nullptr);
		EnumFontFamiliesW(hdc, fontName,
			[](const LOGFONT* lpelf, const TEXTMETRIC* lpntm, DWORD fontType, LPARAM lp)->int {
				bool* pIsInstalled = reinterpret_cast<bool*>(lp);
				*pIsInstalled = true; // if we're here, font does exist
				return 0;
			}, reinterpret_cast<LPARAM>(&isInstalled));
		ReleaseDC(nullptr, hdc);
		return isInstalled;
	}
};

}//namespace wl