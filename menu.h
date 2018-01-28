/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <string>
#include <system_error>
#include <Windows.h>

namespace wl {

// Wrapper to HMENU handle.
class menu final {
protected:
	HMENU _hMenu = nullptr;

public:
	menu() = default;
	menu(HMENU hMenu) noexcept   : _hMenu(hMenu) { }
	menu(const menu& m) noexcept : _hMenu(m._hMenu) { }

	HMENU hmenu() const noexcept {
		return this->_hMenu;
	}

	menu& operator=(HMENU hMenu) noexcept {
		this->_hMenu = hMenu;
		return *this;
	}

	menu& operator=(const menu& m) noexcept {
		this->_hMenu = m._hMenu;
		return *this;
	}

	void destroy() noexcept {
		// Since HMENU resource can be shared, destroy() won't be called on destructor.
		if (this->_hMenu) {
			DestroyMenu(this->_hMenu);
			this->_hMenu = nullptr;
		}
	}

	menu& load_from_resource(int resourceId, HINSTANCE hInst = nullptr) {
		if (!hInst) hInst = GetModuleHandleW(nullptr);
		this->_hMenu = LoadMenuW(hInst, MAKEINTRESOURCEW(resourceId));
		if (!this->_hMenu) {
			throw std::system_error(GetLastError(), std::system_category(),
				"LoadMenu failed when trying to load menu resource");
		}
		return *this;
	}

	menu& load_from_resource(int resourceId, HWND hOwner) {
		return this->load_from_resource(resourceId,
			reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(hOwner, GWLP_HINSTANCE)));
	}

	menu& load_from_resource_submenu(int resourceId, size_t subMenuIndex, HINSTANCE hInst = nullptr) {
		this->load_from_resource(resourceId, hInst);
		this->_hMenu = GetSubMenu(this->_hMenu, static_cast<int>(subMenuIndex));
		return *this;
	}

	menu& load_from_resource_submenu(int resourceId, size_t subMenuIndex, HWND hOwner) {
		return this->load_from_resource_submenu(resourceId, subMenuIndex,
			reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(hOwner, GWLP_HINSTANCE)));
	}

	menu get_submenu(size_t pos) const noexcept {
		return menu(GetSubMenu(this->_hMenu, static_cast<int>(pos)));
	}

	size_t get_item_count() const noexcept {
		return static_cast<size_t>(GetMenuItemCount(this->_hMenu));
	}

	WORD get_item_id(size_t pos) const noexcept {
		return GetMenuItemID(this->_hMenu, static_cast<int>(pos));
	}

private:
	std::wstring _get_caption(UINT identif, BOOL byPos) const {
		wchar_t captionBuf[64]{}; // arbitrary buffer length
		MENUITEMINFOW mii{};
		mii.cbSize     = sizeof(mii);
		mii.cch        = ARRAYSIZE(captionBuf);
		mii.dwTypeData = captionBuf;
		mii.fMask      = MIIM_STRING;

		GetMenuItemInfoW(this->_hMenu, identif, byPos, &mii);
		return captionBuf;
	}

	menu& _set_caption(UINT identif, const wchar_t* caption, BOOL byPos) noexcept {
		MENUITEMINFOW mii{};
		mii.cbSize = sizeof(mii);
		mii.dwTypeData = const_cast<wchar_t*>(caption);
		mii.fMask = MIIM_STRING;

		SetMenuItemInfoW(this->_hMenu, identif, byPos, &mii);
		return *this;
	}

public:
	std::wstring get_caption_by_pos(size_t pos) const { return this->_get_caption(static_cast<UINT>(pos), TRUE); }
	std::wstring get_caption_by_id(WORD cmdId) const  { return this->_get_caption(cmdId, FALSE); }
	menu&        set_caption_by_pos(size_t pos, const wchar_t* caption) noexcept      { return this->_set_caption(static_cast<UINT>(pos), caption, TRUE); }
	menu&        set_caption_by_pos(size_t pos, const std::wstring& caption) noexcept { return this->_set_caption(static_cast<UINT>(pos), caption.c_str(), TRUE); }
	menu&        set_caption_by_id(WORD cmdId, const wchar_t* caption) noexcept       { return this->_set_caption(cmdId, caption, FALSE); }
	menu&        set_caption_by_id(WORD cmdId, const std::wstring& caption) noexcept  { return this->_set_caption(cmdId, caption.c_str(), FALSE); }

	menu& append_separator() noexcept {
		InsertMenuW(this->_hMenu, -1, MF_BYPOSITION | MF_SEPARATOR, 0, nullptr);
		return *this;
	}

	menu& insert_separator_before_id(WORD beforeCmdId) noexcept {
		InsertMenuW(this->_hMenu, beforeCmdId, MF_BYCOMMAND | MF_SEPARATOR, 0, nullptr);
		return *this;
	}

	menu& append_item(WORD cmdId, const wchar_t* caption) noexcept {
		InsertMenuW(this->_hMenu, -1, MF_BYPOSITION | MF_STRING, cmdId, caption);
		return *this;
	}

	menu& append_item(WORD cmdId, const std::wstring& caption) noexcept {
		return this->append_item(cmdId, caption.c_str());
	}

	menu& insert_item_before_id(WORD newCmdId, WORD beforeCmdId, const wchar_t* caption) noexcept {
		InsertMenuW(this->_hMenu, beforeCmdId, MF_BYCOMMAND | MF_STRING, newCmdId, caption);
		return *this;
	}

	menu& insert_item_before_id(WORD newCmdId, WORD beforeCmdId, const std::wstring& caption) noexcept {
		return this->insert_item_before_id(newCmdId, beforeCmdId, caption.c_str());
	}

	menu& enable_item_by_pos(size_t pos, bool doEnable) noexcept {
		EnableMenuItem(this->_hMenu, static_cast<UINT>(pos),
			MF_BYPOSITION | ((doEnable) ? MF_ENABLED : MF_GRAYED));
		return *this;
	}

	menu& enable_item_by_pos(std::initializer_list<size_t> poss, bool doEnable) noexcept {
		for (size_t p : poss) {
			this->enable_item_by_pos(p, doEnable);
		}
		return *this;
	}

	menu& enable_item_by_id(WORD cmdId, bool doEnable) noexcept {
		EnableMenuItem(this->_hMenu, cmdId,
			MF_BYCOMMAND | ((doEnable) ? MF_ENABLED : MF_GRAYED));
		return *this;
	}

	menu& enable_item_by_id(std::initializer_list<WORD> cmdIds, bool doEnable) noexcept {
		for (WORD cmdId : cmdIds) {
			this->enable_item_by_id(cmdId, doEnable);
		}
		return *this;
	}

	menu& set_default_item_by_pos(size_t pos) noexcept {
		SetMenuDefaultItem(this->_hMenu, static_cast<UINT>(pos), MF_BYPOSITION);
		return *this;
	}

	menu& set_default_item_by_id(WORD cmdId) noexcept {
		SetMenuDefaultItem(this->_hMenu, cmdId, MF_BYCOMMAND);
		return *this;
	}

	menu& set_item_check_by_pos(size_t pos, bool doCheck) noexcept {
		CheckMenuItem(this->_hMenu, static_cast<UINT>(pos),
			MF_BYPOSITION | (doCheck ? MF_CHECKED : MF_UNCHECKED));
		return *this;
	}

	menu& set_item_check_by_id(WORD cmdId, bool doCheck) noexcept {
		CheckMenuItem(this->_hMenu, cmdId,
			MF_BYCOMMAND | (doCheck ? MF_CHECKED : MF_UNCHECKED));
		return *this;
	}

	menu& set_item_radio_by_pos(size_t firstPosInList, size_t numItemsInList, size_t checkedPos) noexcept {
		CheckMenuRadioItem(this->_hMenu, static_cast<UINT>(firstPosInList),
			static_cast<UINT>(firstPosInList + numItemsInList),
			static_cast<UINT>(checkedPos), MF_BYPOSITION);
		return *this;
	}

	menu& set_item_radio_by_id(WORD firstCmdIdInList, size_t numItemsInList, WORD checkedCmdId) noexcept {
		CheckMenuRadioItem(this->_hMenu, firstCmdIdInList,
			static_cast<UINT>(firstCmdIdInList + numItemsInList),
			checkedCmdId, MF_BYCOMMAND);
		return *this;
	}

	menu& delete_item_by_pos(size_t pos) noexcept {
		DeleteMenu(this->_hMenu, static_cast<UINT>(pos), MF_BYPOSITION);
		return *this;
	}

	menu& delete_item_by_id(WORD cmdId) noexcept {
		DeleteMenu(this->_hMenu, cmdId, MF_BYCOMMAND);
		return *this;
	}

	menu& delete_all_items() noexcept {
		for (size_t i = this->get_item_count(); i-- > 0; ) {
			this->delete_item_by_pos(i);
		}
		return *this;
	}

	menu append_submenu(const wchar_t* caption) const noexcept {
		menu sub;
		sub._hMenu = CreatePopupMenu();
		AppendMenuW(this->_hMenu, MF_STRING | MF_POPUP,
			reinterpret_cast<UINT_PTR>(sub._hMenu), caption);
		return sub; // return new submenu, so it can be edited
	}

	menu append_submenu(const std::wstring& caption) const noexcept {
		return this->append_submenu(caption.c_str());
	}

	menu insert_submenu_before_id(WORD beforeCmdId, const wchar_t* caption) const noexcept {
		menu sub;
		sub._hMenu = CreatePopupMenu();
		InsertMenuW(this->_hMenu, beforeCmdId, MF_POPUP | MF_BYCOMMAND,
			reinterpret_cast<UINT_PTR>(sub._hMenu), caption);
		return sub; // return new submenu, so it can be edited
	}

	menu insert_submenu_before_id(WORD beforeCmdId, const std::wstring& caption) const noexcept {
		return this->insert_submenu_before_id(beforeCmdId, caption.c_str());
	}

	menu& show_at_point(HWND hParent, POINT pt, HWND hWndCoordsRelativeTo) noexcept {
		// Shows a popup context menu, anchored at the given coordinates.
		// The passed coordinates can be relative to any window.
		POINT ptParent = pt; // receives coordinates relative to hParent
		ClientToScreen(hWndCoordsRelativeTo ? hWndCoordsRelativeTo : hParent, &ptParent); // to screen coordinates
		SetForegroundWindow(hParent);
		TrackPopupMenu(this->_hMenu, 0, ptParent.x, ptParent.y, 0, hParent, nullptr); // owned by dialog, so messages go to it
		PostMessageW(hParent, WM_NULL, 0, 0); // http://msdn.microsoft.com/en-us/library/ms648002%28VS.85%29.aspx
		return *this;
	}
};

}//namespace wl