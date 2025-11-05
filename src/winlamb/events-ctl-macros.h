#pragma once

// To be included only in control event .cpp files.

#define EVENT_CMD(cls, method, cmd) \
	void cls::method(std::function<void()> &&cb) { \
		_events._owner._userEvents.wm_command(_events._ctrlId, cmd, std::move(cb)); \
	}

#define EVENT_NFY_ARG(cls, method, nm, argty) \
	void cls::method(std::function<void(argty&)> &&cb) { \
		_events._owner._userEvents.wm_notify(_events._ctrlId, nm, [this, cb = std::move(cb)](wm::Notify p) -> LRESULT { \
			cb(p.hdr<argty>()); \
			return _events._owner._isDlg ? TRUE : 0; \
		}); \
	}

#define EVENT_NFY_ARG_RET_BOOL(cls, method, nm, argty) \
	void cls::method(std::function<bool(argty&)> &&cb) { \
		_events._owner._userEvents.wm_notify(_events._ctrlId, nm, [cb = std::move(cb)](wm::Notify p) -> LRESULT { \
			return cb(p.hdr<argty>()); \
		}); \
	}

