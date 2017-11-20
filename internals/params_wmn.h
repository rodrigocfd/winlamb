/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "params_wm.h"
#include <CommCtrl.h>

namespace wl {

// Message crackers for all documented WM_NOTIFY messages.
namespace wmn {

#define WINLAMB_NOTIFYWM(sname, oname) \
	struct sname : public wm::notify { \
		sname(const params& p) : notify(p) { } \
		oname& nmhdr() const { return *reinterpret_cast<oname*>(this->lParam); } \
	};

	// ComboBoxEx notifications.
	namespace cben {
		WINLAMB_NOTIFYWM(beginedit, NMHDR);
		WINLAMB_NOTIFYWM(deleteitem, NMCOMBOBOXEX);
		WINLAMB_NOTIFYWM(dragbegin, NMCBEDRAGBEGIN);
		WINLAMB_NOTIFYWM(endedit, NMCBEENDEDIT);
		WINLAMB_NOTIFYWM(getdispinfo, NMCOMBOBOXEX);
		WINLAMB_NOTIFYWM(insertitem, NMCOMBOBOXEX);
		WINLAMB_NOTIFYWM(setcursor, NMMOUSE);
	}

	// Date and time picker notifications.
	namespace dtn {
		WINLAMB_NOTIFYWM(closeup, NMHDR);
		WINLAMB_NOTIFYWM(datetimechange, NMDATETIMECHANGE);
		WINLAMB_NOTIFYWM(dropdown, NMHDR);
		WINLAMB_NOTIFYWM(format, NMDATETIMEFORMAT);
		WINLAMB_NOTIFYWM(formatquery, NMDATETIMEFORMATQUERY);
		WINLAMB_NOTIFYWM(userstring, NMDATETIMESTRING);
		WINLAMB_NOTIFYWM(wmkeydown, NMDATETIMEWMKEYDOWN);
		WINLAMB_NOTIFYWM(killfocus, NMHDR);
		WINLAMB_NOTIFYWM(setfocus, NMHDR);
	}

	// List view notifications.
	namespace lvn {
		WINLAMB_NOTIFYWM(begindrag, NMLISTVIEW);
		WINLAMB_NOTIFYWM(beginlabeledit, NMLVDISPINFO);
		WINLAMB_NOTIFYWM(beginrdrag, NMLISTVIEW);
		WINLAMB_NOTIFYWM(beginscroll, NMLVSCROLL);
		WINLAMB_NOTIFYWM(columnclick, NMLISTVIEW);
		WINLAMB_NOTIFYWM(columndropdown, NMLISTVIEW);
		WINLAMB_NOTIFYWM(columnoverflowclick, NMLISTVIEW);
		WINLAMB_NOTIFYWM(deleteallitems, NMLISTVIEW);
		WINLAMB_NOTIFYWM(deleteitem, NMLISTVIEW);
		WINLAMB_NOTIFYWM(endlabeledit, NMLVDISPINFO);
		WINLAMB_NOTIFYWM(endscroll, NMLVSCROLL);
		WINLAMB_NOTIFYWM(getdispinfo, NMLVDISPINFO);
		WINLAMB_NOTIFYWM(getemptymarkup, NMLVEMPTYMARKUP);
		WINLAMB_NOTIFYWM(getinfotip, NMLVGETINFOTIP);
		WINLAMB_NOTIFYWM(hottrack, NMLISTVIEW);
		WINLAMB_NOTIFYWM(incrementalsearch, NMLVFINDITEM);
		WINLAMB_NOTIFYWM(insertitem, NMLISTVIEW);
		WINLAMB_NOTIFYWM(itemactivate, NMITEMACTIVATE);
		WINLAMB_NOTIFYWM(itemchanged, NMLISTVIEW);
		WINLAMB_NOTIFYWM(itemchanging, NMLISTVIEW);
		WINLAMB_NOTIFYWM(keydown, NMLVKEYDOWN);
		WINLAMB_NOTIFYWM(linkclick, NMLVLINK);
		WINLAMB_NOTIFYWM(marqueebegin, NMHDR);
		WINLAMB_NOTIFYWM(odcachehint, NMLVCACHEHINT);
		WINLAMB_NOTIFYWM(odfinditem, NMLVFINDITEM);
		WINLAMB_NOTIFYWM(odstatechanged, NMLVODSTATECHANGE);
		WINLAMB_NOTIFYWM(setdispinfo, NMLVDISPINFO);
		WINLAMB_NOTIFYWM(click, NMITEMACTIVATE);
		WINLAMB_NOTIFYWM(customdraw, NMLVCUSTOMDRAW);
		WINLAMB_NOTIFYWM(dblclk, NMITEMACTIVATE);
		WINLAMB_NOTIFYWM(hover, NMHDR);
		WINLAMB_NOTIFYWM(killfocus, NMHDR);
		WINLAMB_NOTIFYWM(rclick, NMITEMACTIVATE);
		WINLAMB_NOTIFYWM(rdblclk, NMITEMACTIVATE);
		WINLAMB_NOTIFYWM(releasedcapture, NMHDR);
		WINLAMB_NOTIFYWM(return_, NMHDR);
		WINLAMB_NOTIFYWM(setfocus, NMHDR);
	}

	// Month calendar notifications.
	namespace mcn {
		WINLAMB_NOTIFYWM(getdaystate, NMDAYSTATE);
		WINLAMB_NOTIFYWM(selchange, NMSELCHANGE);
		WINLAMB_NOTIFYWM(select, NMSELCHANGE);
		WINLAMB_NOTIFYWM(viewchange, NMVIEWCHANGE);
		WINLAMB_NOTIFYWM(releasedcapture, NMHDR);
	}

	// Status bar notifications.
	namespace sbn {
		WINLAMB_NOTIFYWM(simplemodechange, NMHDR);
		WINLAMB_NOTIFYWM(click, NMMOUSE);
		WINLAMB_NOTIFYWM(dblclk, NMMOUSE);
		WINLAMB_NOTIFYWM(rclick, NMMOUSE);
		WINLAMB_NOTIFYWM(rdblclk, NMMOUSE);
	}

	// Tab control notifications.
	namespace tcn {
		WINLAMB_NOTIFYWM(focuschange, NMHDR);
		WINLAMB_NOTIFYWM(getobject, NMOBJECTNOTIFY);
		WINLAMB_NOTIFYWM(keydown, NMTCKEYDOWN);
		WINLAMB_NOTIFYWM(selchange, NMHDR);
		WINLAMB_NOTIFYWM(selchanging, NMHDR);
		WINLAMB_NOTIFYWM(click, NMHDR);
		WINLAMB_NOTIFYWM(dblclk, NMHDR);
		WINLAMB_NOTIFYWM(rclick, NMHDR);
		WINLAMB_NOTIFYWM(rdblclk, NMHDR);
		WINLAMB_NOTIFYWM(releasedcapture, NMHDR);
	}

	// Trackbar notifications.
	namespace trbn {
		WINLAMB_NOTIFYWM(thumbposchanging, NMTRBTHUMBPOSCHANGING);
		WINLAMB_NOTIFYWM(customdraw, NMCUSTOMDRAW);
		WINLAMB_NOTIFYWM(releasedcapture, NMHDR);
	}

	// Tooltip notifications.
	namespace ttn {
		WINLAMB_NOTIFYWM(getdispinfo, NMTTDISPINFO);
		WINLAMB_NOTIFYWM(linkclick, NMHDR);
		WINLAMB_NOTIFYWM(needtext, NMTTDISPINFO);
		WINLAMB_NOTIFYWM(pop, NMHDR);
		WINLAMB_NOTIFYWM(show, NMHDR);
		WINLAMB_NOTIFYWM(customdraw, NMTTCUSTOMDRAW);
	}

	// Tree view notifications.
	namespace tvn {
		WINLAMB_NOTIFYWM(asyncdraw, NMTVASYNCDRAW);
		WINLAMB_NOTIFYWM(begindrag, NMTREEVIEW);
		WINLAMB_NOTIFYWM(beginlabeledit, NMTVDISPINFO);
		WINLAMB_NOTIFYWM(beginrdrag, NMTREEVIEW);
		WINLAMB_NOTIFYWM(deleteitem, NMTREEVIEW);
		WINLAMB_NOTIFYWM(endlabeledit, NMTVDISPINFO);
		WINLAMB_NOTIFYWM(getdispinfo, NMTVDISPINFO);
		WINLAMB_NOTIFYWM(getinfotip, NMTVGETINFOTIP);
		WINLAMB_NOTIFYWM(itemchanged, NMTVITEMCHANGE);
		WINLAMB_NOTIFYWM(itemchanging, NMTVITEMCHANGE);
		WINLAMB_NOTIFYWM(itemexpanded, NMTREEVIEW);
		WINLAMB_NOTIFYWM(itemexpanding, NMTREEVIEW);
		WINLAMB_NOTIFYWM(keydown, NMTVKEYDOWN);
		WINLAMB_NOTIFYWM(selchanged, NMTREEVIEW);
		WINLAMB_NOTIFYWM(selchanging, NMTREEVIEW);
		WINLAMB_NOTIFYWM(setdispinfo, NMTVDISPINFO);
		WINLAMB_NOTIFYWM(singleexpand, NMTREEVIEW);
		WINLAMB_NOTIFYWM(click, NMHDR);
		WINLAMB_NOTIFYWM(customdraw, NMTVCUSTOMDRAW);
		WINLAMB_NOTIFYWM(dblclk, NMHDR);
		WINLAMB_NOTIFYWM(killfocus, NMHDR);
		WINLAMB_NOTIFYWM(rclick, NMHDR);
		WINLAMB_NOTIFYWM(rdblclk, NMHDR);
		WINLAMB_NOTIFYWM(return_, NMHDR);
		WINLAMB_NOTIFYWM(setcursor, NMMOUSE);
		WINLAMB_NOTIFYWM(setfocus, NMHDR);
	}

	// Up-down notifications.
	namespace udn {
		WINLAMB_NOTIFYWM(deltapos, NMUPDOWN);
		WINLAMB_NOTIFYWM(releasedcapture, NMHDR);
	}

}//namespace wmn
}//namespace wl
