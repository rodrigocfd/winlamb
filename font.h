/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <system_error>
#include <Windows.h>

namespace wl {

// Wrapper to HFONT handle.
class font final {
private:
	HFONT _hFont = nullptr;

public:
	enum class deco { NONE, BOLD, ITALIC, BOLD_ITALIC };

	~font() {
		this->destroy();
	}

	font() = default;
	font(font&& other) noexcept { this->operator=(std::move(other)); }

	HFONT hfont() const noexcept {
		return this->_hFont;
	}

	font& operator=(font&& other) noexcept {
		this->destroy();
		std::swap(this->_hFont, other._hFont);
		return *this;
	}

	font& destroy() noexcept {
		if (this->_hFont) {
			DeleteObject(this->_hFont);
			this->_hFont = nullptr;
		}
		return *this;
	}

	font& create(const LOGFONT& lf) {
		this->destroy();
		this->_hFont = CreateFontIndirectW(&lf);
		if (!this->_hFont) {
			throw std::system_error(GetLastError(), std::system_category(),
				"CreateFontIndirect failed");
		}
		return *this;
	}

	font& create(const wchar_t* fontName, int size, deco style = deco::NONE) {
		this->destroy();
		LOGFONT lf{};
		lstrcpyW(lf.lfFaceName, fontName);
		lf.lfHeight = -(size + 3);
		lf.lfWeight = style == deco::BOLD || style == deco::BOLD_ITALIC ? FW_BOLD : FW_DONTCARE;
		lf.lfItalic = style == deco::ITALIC || style == deco::BOLD_ITALIC ? TRUE : FALSE;
		return this->create(lf);
	}

	// Create the same exact font used by UI, like Tahoma or Segoe UI.
	font& create_ui() {
		OSVERSIONINFO ovi{};
		ovi.dwOSVersionInfoSize = sizeof(ovi);

		#pragma warning (disable: 4996)
		// http://www.codeproject.com/Articles/678606/Part-Overcoming-Windows-s-deprecation-of-GetVe
		GetVersionExW(&ovi);
		#pragma warning (default: 4996)

		NONCLIENTMETRICS ncm{};
		ncm.cbSize = sizeof(ncm);
		if (ovi.dwMajorVersion < 6) { // below Vista
			ncm.cbSize -= sizeof(ncm.iBorderWidth);
		}
		SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0);
		return this->create(ncm.lfMenuFont); // Tahoma/Segoe
	}

public:
	class util final {
	private:
		util() = delete;

	public:
		// Applies default UI font on all children of the window.
		static void set_ui_on_children(HWND hParent) {
			static font oneFont; // keep one single font instance for all windows
			if (!oneFont._hFont) oneFont.create_ui();

			SendMessageW(hParent, WM_SETFONT,
				reinterpret_cast<WPARAM>(oneFont._hFont),
				MAKELPARAM(FALSE, 0));
			EnumChildWindows(hParent, [](HWND hWnd, LPARAM lp) noexcept->BOOL {
				SendMessageW(hWnd, WM_SETFONT,
					reinterpret_cast<WPARAM>(reinterpret_cast<HFONT>(lp)),
					MAKELPARAM(FALSE, 0)); // will run on each child
				return TRUE;
			}, reinterpret_cast<LPARAM>(oneFont._hFont));
		}

		// Checks if the font is currently installed.
		static bool exists(const wchar_t* fontName) {
			// http://cboard.cprogramming.com/windows-programming/90066-how-determine-if-font-support-unicode.html
			bool isInstalled = false;
			HDC hdc = GetDC(nullptr);
			if (!hdc) {
				throw std::system_error(GetLastError(), std::system_category(),
					"GetDC failed when checking if font exists");
			}
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
};

}//namespace wl
