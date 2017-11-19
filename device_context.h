/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <string>
#include <vector>
#include "internals/hwnd_wrapper.h"

namespace wl {

// Wrapper to HDC.
class device_context {
public:
	class pen final {
	public:
		enum class style : int {
			SOLID      = PS_SOLID,
			DASH       = PS_DASH,
			DOT        = PS_DOT,
			DASHDOT    = PS_DASHDOT,
			DASHDOTDOT = PS_DASHDOTDOT
		};

	private:
		HPEN _hPen;

	public:
		~pen() {
			this->release();
		}

		pen(style styleType, int width, COLORREF color) :
			_hPen(CreatePen(static_cast<int>(styleType), width, color)) { }

		HPEN hpen() const {
			return this->_hPen;
		}

		void release() {
			if (this->_hPen) {
				DeleteObject(this->_hPen);
				this->_hPen = nullptr;
			}
		}
	};

	class brush final {
	public:
		enum class pattern : int {
			BDIAGONAL  = HS_BDIAGONAL,
			CROSS      = HS_CROSS,
			DIAGCROSS  = HS_DIAGCROSS,
			FDIAGONAL  = HS_FDIAGONAL,
			HORIZONTAL = HS_HORIZONTAL,
			VERTICAL   = HS_VERTICAL
		};

		enum class color : int {
			BTNFACE      = COLOR_BTNFACE,
			DESKTOP      = COLOR_DESKTOP,
			BTNTEXT      = COLOR_BTNTEXT,
			WINDOW       = COLOR_WINDOW,
			APPWORKSPACE = COLOR_APPWORKSPACE
		};

	private:
		HBRUSH _hBrush;

	public:
		~brush() {
			this->release();
		}

		brush(COLORREF color) :
			_hBrush(CreateSolidBrush(color)) { }

		brush(pattern hatch, COLORREF color) :
			_hBrush(CreateHatchBrush(static_cast<int>(hatch), color)) { }

		brush(color sysColor) :
			_hBrush(GetSysColorBrush(static_cast<int>(sysColor))) { }

		HBRUSH hbrush() const {
			return this->_hBrush;
		}

		void release() {
			if (this->_hBrush) {
				DeleteObject(this->_hBrush);
				this->_hBrush = nullptr;
			}
		}
	};

protected:
	HWND _hWnd;
	HDC  _hDC;
	SIZE _sz;

public:
	explicit device_context(HWND hWnd, HDC hDC = nullptr)
		: _hWnd(hWnd), _hDC(hDC)
	{
		RECT rcClient{};
		GetClientRect(hWnd, &rcClient); // let's keep available width & height
		this->_sz.cx = rcClient.right;
		this->_sz.cy = rcClient.bottom;
	}

	explicit device_context(const wli::hwnd_wrapper* wnd, HDC hDC = nullptr)
		: device_context(wnd->hwnd(), hDC) { }

	HDC     hdc() const                { return this->_hDC; }
	HWND    hwnd() const               { return this->_hWnd; }
	SIZE    size() const               { return this->_sz; }
	HGDIOBJ select_object(HGDIOBJ obj) { return SelectObject(this->_hDC, obj); }
	HGDIOBJ select_stock_font()        { return select_object(GetStockObject(SYSTEM_FONT)); }
	HGDIOBJ select_stock_pen()         { return select_object(GetStockObject(BLACK_PEN)); }
	HGDIOBJ select_stock_brush()       { return select_object(GetStockObject(WHITE_BRUSH)); }

	device_context& delete_object(HGDIOBJ obj) {
		DeleteObject(obj);
		return *this;
	}

	device_context& move_to(int x, int y) {
		MoveToEx(this->_hDC, x, y, nullptr);
		return *this;
	}

	device_context& line_to(int x, int y) {
		LineTo(this->_hDC, x, y);
		return *this;
	}

	device_context& line_rect(int left, int top, int right, int bottom) {
		return this->move_to(left, top)
			.line_to(right, top)
			.line_to(right, bottom)
			.line_to(left, bottom);
	}

	device_context& line_rect(RECT rc) {
		return this->line_rect(rc.left, rc.top, rc.right, rc.bottom);
	}

	device_context& set_bk_transparent(bool yes) {
		SetBkMode(this->_hDC, yes ? TRANSPARENT : OPAQUE);
		return *this;
	}

	device_context& set_bk_color(COLORREF color = -1) {
		SetBkColor(this->_hDC, (color == -1) ? // default?
			this->get_bk_brush_color() : color);
		return *this;
	}

	COLORREF get_bk_brush_color() const {
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

	device_context& set_text_color(COLORREF color) {
		SetTextColor(this->_hDC, color);
		return *this;
	}

	device_context& text_out(int x, int y, const wchar_t* text, size_t numChars = std::wstring::npos) {
		TextOutW(this->_hDC, x, y, text,
			(numChars == std::wstring::npos) ? lstrlenW(text) : static_cast<int>(numChars));
		return *this;
	}

	device_context& text_out(int x, int y, const std::wstring& text, size_t numChars = std::wstring::npos) {
		return this->text_out(x, y, text.c_str(),
			(numChars == std::wstring::npos) ? text.length() : numChars);
	}

	device_context& draw_text(int x, int y, int cx, int cy, const wchar_t* text,
		UINT fmtFlags = 0, size_t numChars = std::wstring::npos)
	{
		RECT rc{x, y, x + cx, y + cy};
		DrawTextW(this->_hDC, text,
			(numChars == std::wstring::npos) ? lstrlenW(text) : static_cast<int>(numChars),
			&rc, fmtFlags); // DT_LEFT|DT_TOP is zero
		return *this;
	}

	device_context& draw_text(int x, int y, int cx, int cy, const std::wstring& text,
		UINT fmtFlags = 0, size_t numChars = std::wstring::npos)
	{
		return this->draw_text(x, y, cx, cy, text.c_str(), fmtFlags,
			(numChars == std::wstring::npos) ? text.length() : numChars);
	}

	SIZE get_text_extent(const wchar_t* text, size_t numChars = std::wstring::npos) const {
		SIZE sz{};
		GetTextExtentPoint32W(this->_hDC, text,
			(numChars == std::wstring::npos) ? lstrlenW(text) : static_cast<int>(numChars),
			&sz);
		return sz;
	}

	SIZE get_text_extent(const std::wstring& text, size_t numChars = std::wstring::npos) const {
		return this->get_text_extent(text.c_str(),
			(numChars == std::wstring::npos) ? text.length() : numChars);
	}

	device_context& fill_rect(int left, int top, int right, int bottom, HBRUSH hBrush) {
		RECT rc{left, top, right, bottom};
		FillRect(this->_hDC, &rc, hBrush);
		return *this;
	}

	device_context& fill_rect(int left, int top, int right, int bottom, const brush& brush) {
		return this->fill_rect(left, top, right, bottom, brush.hbrush());
	}

	device_context& fill_rgn(HRGN hrgn, HBRUSH hBrush) {
		FillRgn(this->_hDC, hrgn, hBrush);
		return *this;
	}

	device_context& fill_rgn(HRGN hrgn, const brush& brush) {
		return this->fill_rgn(hrgn, brush.hbrush());
	}

	device_context& polygon(const POINT* points, size_t numPoints) {
		Polygon(this->_hDC, points, static_cast<int>(numPoints));
		return *this;
	}

	device_context& polygon(const std::vector<POINT>& points) {
		return this->polygon(&points[0], points.size());
	}

	device_context& polygon(int left, int top, int right, int bottom) {
		POINT pts[] = {
			{left, top},
			{left, bottom},
			{right, bottom},
			{right, top}
		};
		return this->polygon(pts, 4);
	}

	device_context& draw_edge(RECT rc, int edgeType, int flags) {
		DrawEdge(this->_hDC, &rc, edgeType, flags);
		return *this;
	}

	device_context& draw_edge(int left, int top, int right, int bottom, int edgeType, int flags) {
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
		EndPaint(this->device_context::_hWnd, &this->_ps);
	}

	explicit device_context_simple(HWND hWnd) :
		device_context(hWnd, BeginPaint(hWnd, &this->_ps)) { }

	explicit device_context_simple(const wli::hwnd_wrapper* wnd)
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
		BitBlt(this->device_context_simple::_ps.hdc, 0, 0, bm.bmWidth, bm.bmHeight,
			this->device_context::_hDC, 0, 0, SRCCOPY);
		DeleteObject(SelectObject(this->device_context::_hDC, this->_hBmpOld));
		DeleteObject(this->_hBmp);
		DeleteDC(this->device_context::_hDC);
		// ~device_context_simple() kicks in
	}

	explicit device_context_buffered(HWND hWnd) : device_context_simple(hWnd) {
		// In order to make the double-buffer work, you must
		// return zero on WM_ERASEBKGND message handling.
		this->device_context::_hDC = CreateCompatibleDC(this->device_context_simple::_ps.hdc); // overwrite our painting HDC
		this->_hBmp = CreateCompatibleBitmap(this->device_context_simple::_ps.hdc,
			this->device_context::_sz.cx, this->device_context::_sz.cy);
		this->_hBmpOld = reinterpret_cast<HBITMAP>(
			SelectObject(this->device_context::_hDC, this->_hBmp));

		RECT rcClient = {0, 0, this->device_context::_sz.cx, this->device_context::_sz.cy};
		FillRect(this->device_context::_hDC, &rcClient,
			reinterpret_cast<HBRUSH>(GetClassLongPtrW(this->device_context::_hWnd, GCLP_HBRBACKGROUND)) );
	}

	explicit device_context_buffered(const wli::hwnd_wrapper* wnd)
		: device_context_buffered(wnd->hwnd()) { }
};

}//namespace wl
