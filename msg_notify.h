/**
 * Part of WinLamb - Win32 API Lambda Library
 * @author Rodrigo Cesar de Freitas Dias
 * @see https://github.com/rodrigocfd/winlamb
 */

#pragma once
#include "base_msgs.h"

/**
 * base_wnd <-- base_inventory <-- base_msgs <-- msg_notify
 */

namespace wl {

// Adds on_notify() method to handle WM_NOTIFY messages.
class msg_notify : private base::msgs {
public:
	using idT = std::pair<UINT_PTR, UINT>;
	using notify_funcT = base::depot<idT>::funcT;
private:
	base::depot<idT> _nfyDepot;

protected:
	explicit msg_notify(size_t msgsReserve = 0) : _nfyDepot(msgsReserve) {
		this->on_message(WM_NOTIFY, [&](params& p)->LONG_PTR {
			notify_funcT* pFunc = this->_nfyDepot.find({
				reinterpret_cast<NMHDR*>(p.lParam)->idFrom,
				reinterpret_cast<NMHDR*>(p.lParam)->code
			});
			return pFunc ? (*pFunc)(p) : this->msgs::_proc_unhandled(p);
		});
	}

	void on_notify(idT idFromAndCode, notify_funcT func) {
		this->_nfyDepot.add(idFromAndCode, std::move(func));
	}

	void on_notify(UINT_PTR idFrom, UINT code, notify_funcT func) {
		this->_nfyDepot.add({idFrom, code}, std::move(func));
	}

	void on_notify(std::initializer_list<idT> idFromAndCodes, notify_funcT func) {
		this->_nfyDepot.add(idFromAndCodes, std::move(func));
	}
};


namespace wm {

#define WINLAMB_NOTIFYWM(sname, oname) \
	struct sname : public notify { \
		sname(const params& p) : notify(p) { } \
		oname& nmhdr() const { return *reinterpret_cast<oname*>(this->lParam); } \
	};

	WINLAMB_NOTIFYWM(cben_beginedit, NMHDR);
	WINLAMB_NOTIFYWM(cben_deleteitem, NMCOMBOBOXEX);
	WINLAMB_NOTIFYWM(cben_dragbegin, NMCBEDRAGBEGIN);
	WINLAMB_NOTIFYWM(cben_endedit, NMCBEENDEDIT);
	WINLAMB_NOTIFYWM(cben_getdispinfo, NMCOMBOBOXEX);
	WINLAMB_NOTIFYWM(cben_insertitem, NMCOMBOBOXEX);
	WINLAMB_NOTIFYWM(cben_setcursor, NMMOUSE);

	WINLAMB_NOTIFYWM(dtn_closeup, NMHDR);
	WINLAMB_NOTIFYWM(dtn_datetimechange, NMDATETIMECHANGE);
	WINLAMB_NOTIFYWM(dtn_dropdown, NMHDR);
	WINLAMB_NOTIFYWM(dtn_format, NMDATETIMEFORMAT);
	WINLAMB_NOTIFYWM(dtn_formatquery, NMDATETIMEFORMATQUERY);
	WINLAMB_NOTIFYWM(dtn_userstring, NMDATETIMESTRING);
	WINLAMB_NOTIFYWM(dtn_wmkeydown, NMDATETIMEWMKEYDOWN);
	WINLAMB_NOTIFYWM(dtn_killfocus, NMHDR);
	WINLAMB_NOTIFYWM(dtn_setfocus, NMHDR);

	WINLAMB_NOTIFYWM(lvn_begindrag, NMLISTVIEW);
	WINLAMB_NOTIFYWM(lvn_beginlabeledit, NMLVDISPINFO);
	WINLAMB_NOTIFYWM(lvn_beginrdrag, NMLISTVIEW);
	WINLAMB_NOTIFYWM(lvn_beginscroll, NMLVSCROLL);
	WINLAMB_NOTIFYWM(lvn_columnclick, NMLISTVIEW);
	WINLAMB_NOTIFYWM(lvn_columndropdown, NMLISTVIEW);
	WINLAMB_NOTIFYWM(lvn_columnoverflowclick, NMLISTVIEW);
	WINLAMB_NOTIFYWM(lvn_deleteallitems, NMLISTVIEW);
	WINLAMB_NOTIFYWM(lvn_deleteitem, NMLISTVIEW);
	WINLAMB_NOTIFYWM(lvn_endlabeledit, NMLVDISPINFO);
	WINLAMB_NOTIFYWM(lvn_endscroll, NMLVSCROLL);
	WINLAMB_NOTIFYWM(lvn_getdispinfo, NMLVDISPINFO);
	WINLAMB_NOTIFYWM(lvn_getemptymarkup, NMLVEMPTYMARKUP);
	WINLAMB_NOTIFYWM(lvn_getinfotip, NMLVGETINFOTIP);
	WINLAMB_NOTIFYWM(lvn_hottrack, NMLISTVIEW);
	WINLAMB_NOTIFYWM(lvn_incrementalsearch, NMLVFINDITEM);
	WINLAMB_NOTIFYWM(lvn_insertitem, NMLISTVIEW);
	WINLAMB_NOTIFYWM(lvn_itemactivate, NMITEMACTIVATE);
	WINLAMB_NOTIFYWM(lvn_itemchanged, NMLISTVIEW);
	WINLAMB_NOTIFYWM(lvn_itemchanging, NMLISTVIEW);
	WINLAMB_NOTIFYWM(lvn_keydown, NMLVKEYDOWN);
	WINLAMB_NOTIFYWM(lvn_linkclick, NMLVLINK);
	WINLAMB_NOTIFYWM(lvn_marqueebegin, NMHDR);
	WINLAMB_NOTIFYWM(lvn_odcachehint, NMLVCACHEHINT);
	WINLAMB_NOTIFYWM(lvn_odfinditem, NMLVFINDITEM);
	WINLAMB_NOTIFYWM(lvn_odstatechanged, NMLVODSTATECHANGE);
	WINLAMB_NOTIFYWM(lvn_setdispinfo, NMLVDISPINFO);
	WINLAMB_NOTIFYWM(lvn_click, NMITEMACTIVATE);
	WINLAMB_NOTIFYWM(lvn_customdraw, NMLVCUSTOMDRAW);
	WINLAMB_NOTIFYWM(lvn_dblclk, NMITEMACTIVATE);
	WINLAMB_NOTIFYWM(lvn_hover, NMHDR);
	WINLAMB_NOTIFYWM(lvn_killfocus, NMHDR);
	WINLAMB_NOTIFYWM(lvn_rclick, NMITEMACTIVATE);
	WINLAMB_NOTIFYWM(lvn_rdblclk, NMITEMACTIVATE);
	WINLAMB_NOTIFYWM(lvn_releasedcapture, NMHDR);
	WINLAMB_NOTIFYWM(lvn_return, NMHDR);
	WINLAMB_NOTIFYWM(lvn_setfocus, NMHDR);

	WINLAMB_NOTIFYWM(mcn_getdaystate, NMDAYSTATE);
	WINLAMB_NOTIFYWM(mcn_selchange, NMSELCHANGE);
	WINLAMB_NOTIFYWM(mcn_select, NMSELCHANGE);
	WINLAMB_NOTIFYWM(mcn_viewchange, NMVIEWCHANGE);
	WINLAMB_NOTIFYWM(mcn_releasedcapture, NMHDR);

	WINLAMB_NOTIFYWM(sbn_simplemodechange, NMHDR);
	WINLAMB_NOTIFYWM(sbn_click, NMMOUSE);
	WINLAMB_NOTIFYWM(sbn_dblclk, NMMOUSE);
	WINLAMB_NOTIFYWM(sbn_rclick, NMMOUSE);
	WINLAMB_NOTIFYWM(sbn_rdblclk, NMMOUSE);

	WINLAMB_NOTIFYWM(tcn_focuschange, NMHDR);
	WINLAMB_NOTIFYWM(tcn_getobject, NMOBJECTNOTIFY);
	WINLAMB_NOTIFYWM(tcn_keydown, NMTCKEYDOWN);
	WINLAMB_NOTIFYWM(tcn_selchange, NMHDR);
	WINLAMB_NOTIFYWM(tcn_selchanging, NMHDR);
	WINLAMB_NOTIFYWM(tcn_click, NMHDR);
	WINLAMB_NOTIFYWM(tcn_dblclk, NMHDR);
	WINLAMB_NOTIFYWM(tcn_rclick, NMHDR);
	WINLAMB_NOTIFYWM(tcn_rdblclk, NMHDR);
	WINLAMB_NOTIFYWM(tcn_releasedcapture, NMHDR);

	WINLAMB_NOTIFYWM(trbn_thumbposchanging, NMTRBTHUMBPOSCHANGING);
	WINLAMB_NOTIFYWM(trbn_customdraw, NMCUSTOMDRAW);
	WINLAMB_NOTIFYWM(trbn_releasedcapture, NMHDR);

	WINLAMB_NOTIFYWM(ttn_getdispinfo, NMTTDISPINFO);
	WINLAMB_NOTIFYWM(ttn_linkclick, NMHDR);
	WINLAMB_NOTIFYWM(ttn_needtext, NMTTDISPINFO);
	WINLAMB_NOTIFYWM(ttn_pop, NMHDR);
	WINLAMB_NOTIFYWM(ttn_show, NMHDR);
	WINLAMB_NOTIFYWM(ttn_customdraw, NMTTCUSTOMDRAW);

	WINLAMB_NOTIFYWM(tvn_asyncdraw, NMTVASYNCDRAW);
	WINLAMB_NOTIFYWM(tvn_begindrag, NMTREEVIEW);
	WINLAMB_NOTIFYWM(tvn_beginlabeledit, NMTVDISPINFO);
	WINLAMB_NOTIFYWM(tvn_beginrdrag, NMTREEVIEW);
	WINLAMB_NOTIFYWM(tvn_deleteitem, NMTREEVIEW);
	WINLAMB_NOTIFYWM(tvn_endlabeledit, NMTVDISPINFO);
	WINLAMB_NOTIFYWM(tvn_getdispinfo, NMTVDISPINFO);
	WINLAMB_NOTIFYWM(tvn_getinfotip, NMTVGETINFOTIP);
	WINLAMB_NOTIFYWM(tvn_itemchanged, NMTVITEMCHANGE);
	WINLAMB_NOTIFYWM(tvn_itemchanging, NMTVITEMCHANGE);
	WINLAMB_NOTIFYWM(tvn_itemexpanded, NMTREEVIEW);
	WINLAMB_NOTIFYWM(tvn_itemexpanding, NMTREEVIEW);
	WINLAMB_NOTIFYWM(tvn_keydown, NMTVKEYDOWN);
	WINLAMB_NOTIFYWM(tvn_selchanged, NMTREEVIEW);
	WINLAMB_NOTIFYWM(tvn_selchanging, NMTREEVIEW);
	WINLAMB_NOTIFYWM(tvn_setdispinfo, NMTVDISPINFO);
	WINLAMB_NOTIFYWM(tvn_singleexpand, NMTREEVIEW);
	WINLAMB_NOTIFYWM(tvn_click, NMHDR);
	WINLAMB_NOTIFYWM(tvn_customdraw, NMTVCUSTOMDRAW);
	WINLAMB_NOTIFYWM(tvn_dblclk, NMHDR);
	WINLAMB_NOTIFYWM(tvn_killfocus, NMHDR);
	WINLAMB_NOTIFYWM(tvn_rclick, NMHDR);
	WINLAMB_NOTIFYWM(tvn_rdblclk, NMHDR);
	WINLAMB_NOTIFYWM(tvn_return, NMHDR);
	WINLAMB_NOTIFYWM(tvn_setcursor, NMMOUSE);
	WINLAMB_NOTIFYWM(tvn_setfocus, NMHDR);

	WINLAMB_NOTIFYWM(udn_deltapos, NMUPDOWN);
	WINLAMB_NOTIFYWM(udn_releasedcapture, NMHDR);

}//namespace wm
}//namespace wl