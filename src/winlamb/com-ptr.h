#pragma once
#include <system_error>
#include <type_traits>
#include <Windows.h>

namespace wl {

	// Templated COM smart pointer.
	template<typename T>
		requires std::is_base_of_v<IUnknown, T>
	class ComPtr final {
	public:
		~ComPtr() { release(); }

		constexpr ComPtr() = default;
		ComPtr(const ComPtr &other) { operator=(other); }
		ComPtr(ComPtr &&other) noexcept { operator=(std::forward<ComPtr<T>>(other)); }
		constexpr explicit ComPtr(T *p) : _p{p} { }
		explicit ComPtr(REFCLSID clsid, DWORD clsctx = CLSCTX_INPROC_SERVER) { co_create_instance(clsid, clsctx); }

		ComPtr& operator=(const ComPtr &other) {
			release();
			if (other._p) {
				other._p->AddRef(); // we're effectively cloning the COM pointer
				_p = other._p;
			}
			return *this;
		}

		ComPtr& operator=(ComPtr &&other) noexcept {
			release();
			std::swap(_p, other._p);
			return *this;
		}

		ComPtr& operator=(T *p) {
			release();
			_p = p; // take ownership
			return *this;
		}

		[[nodiscard]] constexpr T* operator->() const { return _p; }
		[[nodiscard]] constexpr T* ptr() const { return _p; }
		[[nodiscard]] constexpr T** pptr() { return &_p; }

		// Creates the COM pointer with CoCreateInstance().
		void co_create_instance(REFCLSID clsid, DWORD clsctx = CLSCTX_INPROC_SERVER) {
			release();
			if (HRESULT hr = CoCreateInstance(clsid, nullptr, clsctx, IID_PPV_ARGS(&_p)); FAILED(hr)) {
				throw std::system_error(hr, std::system_category(), "CoCreateInstance failed.");
			}
		}

		// Returns a new COM pointer with QueryInterface().
		template<typename Q>
			requires std::is_base_of_v<IUnknown, Q>
		[[nodiscard]] ComPtr<Q> query_interface() const {
			Q *pQueried = nullptr;
			if (HRESULT hr = _p->QueryInterface(IID_PPV_ARGS(&pQueried)); FAILED(hr)) {
				throw std::system_error(hr, std::system_category(), "QueryInterface failed.");
			}
			return ComPtr<Q>{pQueried};
		}

		// Manually calls Release(). The destructor will call this method automatically.
		void release() noexcept {
			if (_p) {
				_p->Release();
				_p = nullptr;
			}
		}

	private:
		T *_p = nullptr;
	};

}
