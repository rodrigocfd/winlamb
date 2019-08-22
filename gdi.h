/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <string>
#include <vector>
#include "internals/gdi_objects.h"
#include "wnd.h"

namespace wl {

// Wrappers to GDI objects.
namespace gdi {

// Wrapper to device context, BeginPaint/EndPaint must be called manually.
class dc {
protected:
	HDC _hDC = nullptr;

public:
	explicit dc(HDC hDC) noexcept :
		_hDC(hDC) { }

	HDC hdc() const noexcept {
		return this->_hDC;
	}

	// Selects a GDI object into current device context. The new object replaces
	// the previous object of the same type. The previous object is returned.
	HGDIOBJ select_object(HGDIOBJ obj) noexcept {
		return SelectObject(this->_hDC, obj);
	}

	// Selects the stock GDI font into current device context.
	// The previous font object is returned.
	HGDIOBJ select_stock_font() noexcept {
		return this->select_object(GetStockObject(SYSTEM_FONT));
	}

	// Selects the stock GDI pen into current device context.
	// The previous pen object is returned.
	HGDIOBJ select_stock_pen() noexcept {
		return this->select_object(GetStockObject(BLACK_PEN));
	}

	// Selects the stock GDI brush into current device context.
	// The previous brush object is returned.
	HGDIOBJ select_stock_brush() noexcept {
		return this->select_object(GetStockObject(WHITE_BRUSH));
	}

	// Deletes a logical pen, brush, font, bitmap, region, or palette,
	// freeing all system resources associated with the object.
	dc& delete_object(HGDIOBJ obj) noexcept {
		DeleteObject(obj);
		return *this;
	}

	// Updates the current position to the specified point.
	dc& move_to(int x, int y) noexcept {
		MoveToEx(this->_hDC, x, y, nullptr);
		return *this;
	}

	// Updates the current position to the specified point.
	dc& move_to(const POINT& pt) noexcept {
		return this->move_to(pt.x, pt.y);
	}

	// Draws a line from the current position up to,
	// but not including, the specified point.
	dc& line_to(int x, int y) noexcept {
		LineTo(this->_hDC, x, y);
		return *this;
	}

	// Draws a line from the current position up to,
	// but not including, the specified point.
	dc& line_to(const POINT& pt) noexcept {
		return this->line_to(pt.x, pt.y);
	}

	// Calls line_to() four times to draw a rectangle.
	dc& line_rect(int left, int top, int right, int bottom) noexcept {
		return this->move_to(left, top)
			.line_to(right, top)
			.line_to(right, bottom)
			.line_to(left, bottom);
	}

	// Calls line_to() four times to draw a rectangle.
	dc& line_rect(const RECT& rc) noexcept {
		return this->line_rect(rc.left, rc.top, rc.right, rc.bottom);
	}

	// Sets the background mix mode to transparent, calling SetBkMode().
	dc& set_bk_transparent(bool yes) noexcept {
		SetBkMode(this->_hDC, yes ? TRANSPARENT : OPAQUE);
		return *this;
	}

	// Sets the current background color to the specified color value, calling SetBkColor().
	dc& set_bk_color(COLORREF color) noexcept {
		SetBkColor(this->_hDC, color);
		return *this;
	}

	// Sets the current background color to the specified color value, calling SetBkColor().
	dc& set_bk_color(std::array<BYTE, 3> rgbColor) noexcept {
		return this->set_bk_color(RGB(rgbColor[0], rgbColor[1], rgbColor[2]));
	}

	// Sets the current text color, calling SetTextColor().
	dc& set_text_color(COLORREF color) noexcept {
		SetTextColor(this->_hDC, color);
		return *this;
	}

	// Sets the current text color, calling SetTextColor().
	dc& set_text_color(std::array<BYTE, 3> rgbColor) noexcept {
		return this->set_text_color(RGB(rgbColor[0], rgbColor[1], rgbColor[2]));
	}

	// Writes a character string at the specified location, using the
	// currently selected font, background color, and text color.
	dc& text_out(int x, int y, const wchar_t* text,
		size_t numChars = std::wstring::npos) noexcept
	{
		TextOutW(this->_hDC, x, y, text,
			(numChars == std::wstring::npos) ? lstrlenW(text) : static_cast<int>(numChars));
		return *this;
	}

	// Writes a character string at the specified location, using the
	// currently selected font, background color, and text color.
	dc& text_out(int x, int y, const std::wstring& text,
		size_t numChars = std::wstring::npos) noexcept
	{
		return this->text_out(x, y, text.c_str(),
			(numChars == std::wstring::npos) ? text.length() : numChars);
	}

	// Draws formatted text in the specified rectangle. It formats the text according to the
	// specified method (expanding tabs, justifying characters, breaking lines, and so forth).
	dc& draw_text(int x, int y, int cx, int cy, const wchar_t* text,
		UINT fmtFlags = 0, size_t numChars = std::wstring::npos) noexcept
	{
		RECT rc{x, y, x + cx, y + cy};
		DrawTextW(this->_hDC, text,
			(numChars == std::wstring::npos) ? lstrlenW(text) : static_cast<int>(numChars),
			&rc, fmtFlags); // DT_LEFT|DT_TOP is zero
		return *this;
	}

	// Draws formatted text in the specified rectangle. It formats the text according to the
	// specified method (expanding tabs, justifying characters, breaking lines, and so forth).
	dc& draw_text(int x, int y, int cx, int cy, const std::wstring& text,
		UINT fmtFlags = 0, size_t numChars = std::wstring::npos) noexcept
	{
		return this->draw_text(x, y, cx, cy, text.c_str(), fmtFlags,
			(numChars == std::wstring::npos) ? text.length() : numChars);
	}

	// Gets box size according to GetTextExtentPoint32().
	SIZE get_text_extent(const wchar_t* text, size_t numChars = std::wstring::npos) const noexcept {
		SIZE sz{};
		GetTextExtentPoint32W(this->_hDC, text,
			(numChars == std::wstring::npos) ? lstrlenW(text) : static_cast<int>(numChars),
			&sz);
		return sz;
	}

	// Gets box size according to GetTextExtentPoint32().
	SIZE get_text_extent(const std::wstring& text, size_t numChars = std::wstring::npos) const noexcept {
		return this->get_text_extent(text.c_str(),
			(numChars == std::wstring::npos) ? text.length() : numChars);
	}

	// Fills a rectangle by using the specified brush. This function includes the left and top
	// borders, but excludes the right and bottom borders of the rectangle.
	dc& fill_rect(int left, int top, int right, int bottom, HBRUSH hBrush) noexcept {
		RECT rc{left, top, right, bottom};
		FillRect(this->_hDC, &rc, hBrush);
		return *this;
	}

	// Fills a rectangle by using the specified brush. This function includes the left and top
	// borders, but excludes the right and bottom borders of the rectangle.
	dc& fill_rect(int left, int top, int right, int bottom, const brush& brush) noexcept {
		return this->fill_rect(left, top, right, bottom, brush.hbrush());
	}

	// Fills a region by using the specified brush.
	dc& fill_rgn(HRGN hrgn, HBRUSH hBrush) noexcept {
		FillRgn(this->_hDC, hrgn, hBrush);
		return *this;
	}

	// Fills a region by using the specified brush.
	dc& fill_rgn(HRGN hrgn, const brush& brush) noexcept {
		return this->fill_rgn(hrgn, brush.hbrush());
	}

	// Draws a polygon consisting of two or more vertices connected by straight lines.
	// The polygon is outlined by using the current pen and filled by using the current
	// brush and polygon fill mode.
	dc& polygon(const POINT* points, size_t numPoints) noexcept {
		Polygon(this->_hDC, points, static_cast<int>(numPoints));
		return *this;
	}

	// Draws a polygon consisting of two or more vertices connected by straight lines.
	// The polygon is outlined by using the current pen and filled by using the current
	// brush and polygon fill mode.
	dc& polygon(const std::vector<POINT>& points) noexcept {
		return this->polygon(&points[0], points.size());
	}

	// Draws a polygon consisting of two or more vertices connected by straight lines.
	// The polygon is outlined by using the current pen and filled by using the current
	// brush and polygon fill mode.
	dc& polygon(int left, int top, int right, int bottom) noexcept {
		POINT pts[] = {
			{left, top},
			{left, bottom},
			{right, bottom},
			{right, top}
		};
		return this->polygon(pts, 4);
	}

	// Draws one or more edges of rectangle.
	dc& draw_edge(RECT rc, int edgeType, int flags) noexcept {
		DrawEdge(this->_hDC, &rc, edgeType, flags);
		return *this;
	}

	// Draws one or more edges of rectangle.
	dc& draw_edge(int left, int top, int right, int bottom,
		int edgeType, int flags) noexcept
	{
		RECT rc{left, top, right, bottom};
		return this->draw_edge(std::move(rc), edgeType, flags);
	}
};


// Wrapper to device context, BeginPaint/EndPaint automatically called.
class dc_painter : public dc {
private:
	HWND        _hWnd;
	PAINTSTRUCT _ps;
	SIZE        _sz;

public:
	~dc_painter() {
		EndPaint(this->_hWnd, &this->_ps);
	}

	explicit dc_painter(HWND hWnd) noexcept :
		dc(BeginPaint(hWnd, &_ps)), _hWnd(hWnd)
	{
		RECT rcClient{};
		GetClientRect(hWnd, &rcClient); // cache width & height
		this->_sz.cx = rcClient.right;
		this->_sz.cy = rcClient.bottom;
	}

	explicit dc_painter(const wnd* w) noexcept :
		dc_painter(w->hwnd()) { }

	// Returns the handle to the window being repainted.
	HWND hwnd() const noexcept {
		return this->_hWnd;
	}

	// Returns the PAINTSTRUCT being used to repaint this window.
	const PAINTSTRUCT& ps() const noexcept {
		return this->_ps;
	}

	// Returns the cached width and height of the client area being repainted.
	const SIZE& size() const noexcept {
		return this->_sz;
	}

	// Gets the color of the current background brush.
	COLORREF get_bk_brush_color() const noexcept {
		ULONG_PTR hbrBg = GetClassLongPtrW(this->_hWnd, GCLP_HBRBACKGROUND);
		if (hbrBg > 100) {
			// The hbrBackground is a brush handle, not a system color constant.
			// This 100 value is arbitrary, based on system color constants like COLOR_BTNFACE.
			LOGBRUSH logBrush{};
			GetObjectW(reinterpret_cast<HBRUSH>(hbrBg), sizeof(LOGBRUSH), &logBrush);
			return logBrush.lbColor;
		}
		return GetSysColor(static_cast<int>(hbrBg - 1));
	}
};


// Wrapper to device context, BeginPaint/EndPaint automatically called with double-buffer.
class dc_painter_buffered final : public dc_painter {
private:
	HBITMAP _hBmp, _hBmpOld;

public:
	~dc_painter_buffered() {
		BITMAP bm{}; // http://www.ureader.com/msg/14721900.aspx
		GetObjectW(this->_hBmp, sizeof(bm), &bm);
		BitBlt(this->ps().hdc, 0, 0, bm.bmWidth, bm.bmHeight,
			this->_hDC, 0, 0, SRCCOPY);
		DeleteObject(SelectObject(this->_hDC, this->_hBmpOld));
		DeleteObject(this->_hBmp);
		DeleteDC(this->_hDC);
	}

	explicit dc_painter_buffered(HWND hWnd) noexcept :
		dc_painter(hWnd)
	{
		// In order to make the double-buffer work, you must
		// return zero on WM_ERASEBKGND message handling.
		this->_hDC = CreateCompatibleDC(this->ps().hdc); // overwrite our painting HDC
		this->_hBmp = CreateCompatibleBitmap(this->ps().hdc, this->size().cx, this->size().cy);
		this->_hBmpOld = reinterpret_cast<HBITMAP>(SelectObject(this->_hDC, this->_hBmp));

		RECT rcClient = {0, 0, this->size().cx, this->size().cy};
		FillRect(this->_hDC, &rcClient,
			reinterpret_cast<HBRUSH>(GetClassLongPtrW(this->hwnd(), GCLP_HBRBACKGROUND)) );
	}

	explicit dc_painter_buffered(const wnd* w) noexcept :
		dc_painter_buffered(w->hwnd()) { }
};

}//namespace gdi
}//namespace wl
