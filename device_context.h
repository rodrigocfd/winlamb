/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <string>
#include <vector>
#include "internals/device_context_objects.h"
#include "wnd.h"

namespace wl {

// Wrapper to HDC.
class device_context {
public:
	using brush = wli::brush;
	using pen = wli::pen;
	
protected:
	HWND _hWnd;
	HDC  _hDC;
	SIZE _sz;

public:
	explicit device_context(HWND hWnd, HDC hDC = nullptr) noexcept
		: _hWnd(hWnd), _hDC(hDC)
	{
		RECT rcClient{};
		GetClientRect(hWnd, &rcClient); // let's keep available width & height
		this->_sz.cx = rcClient.right;
		this->_sz.cy = rcClient.bottom;
	}

	explicit device_context(const wnd* wnd, HDC hDC = nullptr) noexcept
		: device_context(wnd->hwnd(), hDC) { }

	HDC     hdc() const noexcept                { return this->_hDC; }
	HWND    hwnd() const noexcept               { return this->_hWnd; }
	SIZE    size() const noexcept               { return this->_sz; }
	HGDIOBJ select_object(HGDIOBJ obj) noexcept { return SelectObject(this->_hDC, obj); }
	HGDIOBJ select_stock_font() noexcept        { return select_object(GetStockObject(SYSTEM_FONT)); }
	HGDIOBJ select_stock_pen() noexcept         { return select_object(GetStockObject(BLACK_PEN)); }
	HGDIOBJ select_stock_brush() noexcept       { return select_object(GetStockObject(WHITE_BRUSH)); }

	device_context& delete_object(HGDIOBJ obj) noexcept {
		DeleteObject(obj);
		return *this;
	}

	device_context& move_to(int x, int y) noexcept {
		MoveToEx(this->_hDC, x, y, nullptr);
		return *this;
	}

	device_context& line_to(int x, int y) noexcept {
		LineTo(this->_hDC, x, y);
		return *this;
	}

	device_context& line_rect(int left, int top, int right, int bottom) noexcept {
		return this->move_to(left, top)
			.line_to(right, top)
			.line_to(right, bottom)
			.line_to(left, bottom);
	}

	device_context& line_rect(RECT rc) noexcept {
		return this->line_rect(rc.left, rc.top, rc.right, rc.bottom);
	}

	device_context& set_bk_transparent(bool yes) noexcept {
		SetBkMode(this->_hDC, yes ? TRANSPARENT : OPAQUE);
		return *this;
	}

	device_context& set_bk_color(COLORREF color = -1) noexcept {
		SetBkColor(this->_hDC, (color == -1) ? // default?
			this->get_bk_brush_color() : color);
		return *this;
	}

	device_context& set_bk_color(std::array<BYTE, 3> rgbColor) noexcept {
		return this->set_bk_color(RGB(rgbColor[0], rgbColor[1], rgbColor[2]));
	}

	COLORREF get_bk_brush_color() const noexcept {
		ULONG_PTR hbrBg = GetClassLongPtrW(this->_hWnd, GCLP_HBRBACKGROUND);
		if (hbrBg > 100) {
			// The hbrBackground is a brush handle, not a system color constant.
			// This 100 value is arbitrary, based on system color constants like COLOR_BTNFACE.
			LOGBRUSH logBrush;
			GetObjectW(reinterpret_cast<HBRUSH>(hbrBg), sizeof(LOGBRUSH), &logBrush);
			return logBrush.lbColor;
		}
		return GetSysColor(static_cast<int>(hbrBg - 1));
	}

	device_context& set_text_color(COLORREF color) noexcept {
		SetTextColor(this->_hDC, color);
		return *this;
	}

	device_context& set_text_color(std::array<BYTE, 3> rgbColor) noexcept {
		return this->set_text_color(RGB(rgbColor[0], rgbColor[1], rgbColor[2]));
	}

	device_context& text_out(int x, int y, const wchar_t* text,
		size_t numChars = std::wstring::npos) noexcept
	{
		TextOutW(this->_hDC, x, y, text,
			(numChars == std::wstring::npos) ? lstrlenW(text) : static_cast<int>(numChars));
		return *this;
	}

	device_context& text_out(int x, int y, const std::wstring& text,
		size_t numChars = std::wstring::npos) noexcept
	{
		return this->text_out(x, y, text.c_str(),
			(numChars == std::wstring::npos) ? text.length() : numChars);
	}

	device_context& draw_text(int x, int y, int cx, int cy, const wchar_t* text,
		UINT fmtFlags = 0, size_t numChars = std::wstring::npos) noexcept
	{
		RECT rc{x, y, x + cx, y + cy};
		DrawTextW(this->_hDC, text,
			(numChars == std::wstring::npos) ? lstrlenW(text) : static_cast<int>(numChars),
			&rc, fmtFlags); // DT_LEFT|DT_TOP is zero
		return *this;
	}

	device_context& draw_text(int x, int y, int cx, int cy, const std::wstring& text,
		UINT fmtFlags = 0, size_t numChars = std::wstring::npos) noexcept
	{
		return this->draw_text(x, y, cx, cy, text.c_str(), fmtFlags,
			(numChars == std::wstring::npos) ? text.length() : numChars);
	}

	// Gets box size according to GetTextExtentPoint32 function.
	SIZE get_text_extent(const wchar_t* text, size_t numChars = std::wstring::npos) const noexcept {
		SIZE sz{};
		GetTextExtentPoint32W(this->_hDC, text,
			(numChars == std::wstring::npos) ? lstrlenW(text) : static_cast<int>(numChars),
			&sz);
		return sz;
	}

	SIZE get_text_extent(const std::wstring& text, size_t numChars = std::wstring::npos) const noexcept {
		return this->get_text_extent(text.c_str(),
			(numChars == std::wstring::npos) ? text.length() : numChars);
	}

	device_context& fill_rect(int left, int top, int right, int bottom, HBRUSH hBrush) noexcept {
		RECT rc{left, top, right, bottom};
		FillRect(this->_hDC, &rc, hBrush);
		return *this;
	}

	device_context& fill_rect(int left, int top, int right, int bottom, const brush& brush) noexcept {
		return this->fill_rect(left, top, right, bottom, brush.hbrush());
	}

	device_context& fill_rgn(HRGN hrgn, HBRUSH hBrush) noexcept {
		FillRgn(this->_hDC, hrgn, hBrush);
		return *this;
	}

	device_context& fill_rgn(HRGN hrgn, const brush& brush) noexcept {
		return this->fill_rgn(hrgn, brush.hbrush());
	}

	device_context& polygon(const POINT* points, size_t numPoints) noexcept {
		Polygon(this->_hDC, points, static_cast<int>(numPoints));
		return *this;
	}

	device_context& polygon(const std::vector<POINT>& points) noexcept {
		return this->polygon(&points[0], points.size());
	}

	device_context& polygon(int left, int top, int right, int bottom) noexcept {
		POINT pts[] = {
			{left, top},
			{left, bottom},
			{right, bottom},
			{right, top}
		};
		return this->polygon(pts, 4);
	}

	device_context& draw_edge(RECT rc, int edgeType, int flags) noexcept {
		DrawEdge(this->_hDC, &rc, edgeType, flags);
		return *this;
	}

	device_context& draw_edge(int left, int top, int right, int bottom,
		int edgeType, int flags) noexcept
	{
		RECT rc{left, top, right, bottom};
		return this->draw_edge(std::move(rc), edgeType, flags);
	}
};


// Wrapper to HDC within WM_PAINT, uses BeginPaint/EndPaint.
class device_context_simple : public device_context {
protected:
	PAINTSTRUCT _ps;

public:
	~device_context_simple() {
		EndPaint(this->_hWnd, &this->_ps);
	}

	explicit device_context_simple(HWND hWnd) noexcept
		: device_context(hWnd, BeginPaint(hWnd, &this->_ps)) { }

	explicit device_context_simple(const wnd* wnd) noexcept
		: device_context_simple(wnd->hwnd()) { }
};


// Wrapper to HDC within WM_PAINT, uses BeginPaint/EndPaint with double-buffer.
class device_context_buffered final : public device_context_simple {
private:
	HBITMAP _hBmp, _hBmpOld;

public:
	~device_context_buffered() {
		BITMAP bm{}; // http://www.ureader.com/msg/14721900.aspx
		GetObjectW(this->_hBmp, sizeof(bm), &bm);
		BitBlt(this->_ps.hdc, 0, 0, bm.bmWidth, bm.bmHeight,
			this->_hDC, 0, 0, SRCCOPY);
		DeleteObject(SelectObject(this->_hDC, this->_hBmpOld));
		DeleteObject(this->_hBmp);
		DeleteDC(this->_hDC);
		// ~device_context_simple() kicks in
	}

	explicit device_context_buffered(HWND hWnd) noexcept : device_context_simple(hWnd) {
		// In order to make the double-buffer work, you must
		// return zero on WM_ERASEBKGND message handling.
		this->_hDC = CreateCompatibleDC(this->_ps.hdc); // overwrite our painting HDC
		this->_hBmp = CreateCompatibleBitmap(this->_ps.hdc, this->_sz.cx, this->_sz.cy);
		this->_hBmpOld = reinterpret_cast<HBITMAP>(SelectObject(this->_hDC, this->_hBmp));

		RECT rcClient = {0, 0, this->_sz.cx, this->_sz.cy};
		FillRect(this->_hDC, &rcClient,
			reinterpret_cast<HBRUSH>(GetClassLongPtrW(this->_hWnd, GCLP_HBRBACKGROUND)) );
	}

	explicit device_context_buffered(const wnd* wnd) noexcept
		: device_context_buffered(wnd->hwnd()) { }
};

}//namespace wl
