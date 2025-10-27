#pragma once
#include <system_error>
#include <type_traits>
#include "lib-include-win.h"
#include <WTypesbase.h>

namespace wl {

	/// @brief Templated COM smart pointer.
	///
	/// Wraps an [`IUnknown`]-derived pointer, properly managing it to avoid memory leaks.
	///
	/// [`IUnknown`]: https://learn.microsoft.com/en-us/windows/win32/api/unknwn/nn-unknwn-iunknown
	template<typename T>
		requires std::is_base_of_v<IUnknown, T>
	class ComPtr final {
	public:
		/// Destructor.
		///
		/// Calls [`IUnknown::Release`] and sets the pointer to `nullptr`.
		///
		/// [`IUnknown::Release`]: https://learn.microsoft.com/en-us/windows/win32/api/unknwn/nf-unknwn-iunknown-release
		~ComPtr() { release(); }

		/// Default constructor.
		///
		/// Initializes the pointer to `nullptr`.
		constexpr ComPtr() = default;

		/// Copy-constructor.
		///
		/// Clones the pointer by calling [`IUnknown::AddRef`] to increment the COM reference count.
		///
		/// [`IUnknown::AddRef`]: https://learn.microsoft.com/en-us/windows/win32/api/unknwn/nf-unknwn-iunknown-addref
		ComPtr(const ComPtr &other) { operator=(other); }

		/// Move-constructor.
		///
		/// Takes ownership of `other`, so no memory leaks happen.
		ComPtr(ComPtr &&other) noexcept { operator=(std::forward<ComPtr<T>>(other)); }

		/// Constructs `ComPtr` by wrapping `p`.
		///
		/// Ideally, you should never need this.
		constexpr explicit ComPtr(T *p) : _p{p} { }

		/// Initializes the internal pointer by calling [`CoCreateInstance`] immediately.
		///
		/// [`CoCreateInstance`]: https://learn.microsoft.com/en-us/windows/win32/api/combaseapi/nf-combaseapi-cocreateinstance
		explicit ComPtr(REFCLSID clsid, DWORD clsctx = CLSCTX_INPROC_SERVER) { co_create_instance(clsid, clsctx); }

		/// Copy-assignment operator.
		///
		/// Clones the pointer by calling [`IUnknown::AddRef`] to increment the COM reference count.
		///
		/// [`IUnknown::AddRef`]: https://learn.microsoft.com/en-us/windows/win32/api/unknwn/nf-unknwn-iunknown-addref
		ComPtr& operator=(const ComPtr &other) {
			release();
			if (other._p) {
				other._p->AddRef(); // we're effectively cloning the COM pointer
				_p = other._p;
			}
			return *this;
		}

		/// Move-assignment operator.
		///
		/// Calls [`IUnknown::Release`] on the current pointer and takes ownership of `other`, so no memory leaks happen.
		///
		/// [`IUnknown::Release`]: https://learn.microsoft.com/en-us/windows/win32/api/unknwn/nf-unknwn-iunknown-release
		ComPtr& operator=(ComPtr &&other) noexcept {
			release();
			std::swap(_p, other._p);
			return *this;
		}

		/// Pointer-assignment operator.
		///
		/// Calls [`IUnknown::Release`] on the current pointer and wraps `p`.
		///
		/// [`IUnknown::Release`]: https://learn.microsoft.com/en-us/windows/win32/api/unknwn/nf-unknwn-iunknown-release
		ComPtr& operator=(T *p) {
			release();
			_p = p; // take ownership
			return *this;
		}

		/** Overloads the arrow operator to directly call the COM pointer member functions. */
		[[nodiscard]] constexpr T* operator->() const { return _p; }

		/** Returns the wrapped COM pointer. */
		[[nodiscard]] constexpr T* ptr() const { return _p; }

		/** Returns a pointer to the wrapped COM pointer. */
		[[nodiscard]] constexpr T** pptr() { return &_p; }

		/// Calls [`IUnknown::Release`] on the current pointer, then calls [`CoCreateInstance`].
		///
		/// [`IUnknown::Release`]: https://learn.microsoft.com/en-us/windows/win32/api/unknwn/nf-unknwn-iunknown-release
		/// [`CoCreateInstance`]: https://learn.microsoft.com/en-us/windows/win32/api/combaseapi/nf-combaseapi-cocreateinstance
		void co_create_instance(REFCLSID clsid, DWORD clsctx = CLSCTX_INPROC_SERVER) {
			release();
			if (HRESULT hr = CoCreateInstance(clsid, nullptr, clsctx, IID_PPV_ARGS(&_p)); FAILED(hr)) {
				throw std::system_error(hr, std::system_category(), "CoCreateInstance failed");
			}
		}

		/// Returns a new `ComPtr` by calling [`IUnknown::QueryInterface`].
		///
		/// [`IUnknown::QueryInterface`]: https://learn.microsoft.com/en-us/windows/win32/api/unknwn/nf-unknwn-iunknown-queryinterface(refiid_void)
		template<typename Q>
			requires std::is_base_of_v<IUnknown, Q>
		[[nodiscard]] ComPtr<Q> query_interface() const {
			Q *pQueried = nullptr;
			if (HRESULT hr = _p->QueryInterface(IID_PPV_ARGS(&pQueried)); FAILED(hr)) {
				throw std::system_error(hr, std::system_category(), "QueryInterface failed");
			}
			return ComPtr<Q>{pQueried};
		}

		/// Manually calls [`IUnknown::Release`], releasing the pointer immediately.
		///
		/// This method is automatically called by the destructor.
		///
		/// [`IUnknown::Release`]: https://learn.microsoft.com/en-us/windows/win32/api/unknwn/nf-unknwn-iunknown-release
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
