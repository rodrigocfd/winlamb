#pragma once
#include <system_error>
#include <type_traits>

#include <sdkddkver.h>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <Unknwnbase.h>
#include <WTypesbase.h>

namespace wl {

	/// @brief Templated COM smart pointer.
	///
	/// Wraps an [`IUnknown`]-derived pointer. Calls [`IUnknown::AddRef`] and
	/// [`IUnknown::Release`] automatically to prevent resource leaks.
	///
	/// Example calling [`CoCreateInstance`] to create a COM object:
	///
	/// ```cpp
	/// wl::ComPtr<IFileOpenDialog> openDlg{};
	/// openDlg.co_create_instance(CLSID_FileOpenDialog);
	/// openDlg->SetFileTypeIndex(1);
	/// ```
	///
	/// [`IUnknown`]: https://learn.microsoft.com/en-us/windows/win32/api/unknwn/nn-unknwn-iunknown
	/// [`IUnknown::AddRef`]: https://learn.microsoft.com/en-us/windows/win32/api/unknwn/nf-unknwn-iunknown-addref
	/// [`IUnknown::Release`]: https://learn.microsoft.com/en-us/windows/win32/api/unknwn/nf-unknwn-iunknown-release
	/// [`CoCreateInstance`]: https://learn.microsoft.com/en-us/windows/win32/api/combaseapi/nf-combaseapi-cocreateinstance
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
		/// Clones the pointer by calling [`IUnknown::AddRef`] to increment the
		/// COM reference count.
		///
		/// [`IUnknown::AddRef`]: https://learn.microsoft.com/en-us/windows/win32/api/unknwn/nf-unknwn-iunknown-addref
		ComPtr(const ComPtr &other) { operator=(other); }

		/// Move-constructor.
		///
		/// Takes ownership of `other`, so no resource leaks happen – after the
		/// move, `other` will hold a `nullptr`.
		ComPtr(ComPtr &&other) noexcept { operator=(std::forward<ComPtr<T>>(other)); }

		/// Constructs `ComPtr` by wrapping `p`.
		///
		/// Ideally, you should never need this.
		constexpr explicit ComPtr(T *p) : _p{p} { }

		/// Copy-assignment operator.
		///
		/// Clones the pointer by calling [`IUnknown::AddRef`] to increment the
		/// COM reference count.
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
		/// Calls [`IUnknown::Release`] on the current pointer and takes ownership
		/// of `other`, so no resource leaks happen – after the move, `other` will
		/// hold a `nullptr`.
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
		/// Ideally, you should never need this.
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

		/// Returns a pointer to the wrapped COM pointer.
		///
		/// Useful to functions which create a COM pointer externally, placing it
		/// inside an existing pointer variable.
		///
		/// Example:
		///
		/// ```cpp
		/// wl::ComPtr<IShellItem> item{};
		/// SHCreateItemFromParsingName(L"C:\\Temp\\foo.txt", nullptr, IID_IShellItem, item.pptr<void>());
		/// ```
		template<typename Q = T>
		[[nodiscard]] constexpr Q** pptr() { return reinterpret_cast<Q**>(&_p); }

		/// Returns the wrapped COM pointer, setting the current pointer to
		/// `nullptr`, so that `release` won't be called.
		///
		/// It's your responsability to release the returned pointer, or a
		/// resource leak will occur.
		///
		/// Example:
		///
		/// ```cpp
		/// wl::ComPtr<IFileOpenDialog> openDlg{};
		/// openDlg.co_create_instance(CLSID_FileOpenDialog);
		/// IFileOpenDialog *pLeaked = openDlg.leak();
		///
		/// wl::ComPtr<IFileOpenDialog> openDlg2{pLeaked}; // take ownership again
		/// ```
		[[nodiscard]] T* leak() {
			T *ptr = _p;
			_p = nullptr;
			return ptr;
		}

		/// Calls [`IUnknown::Release`] on the current pointer, then calls
		/// [`CoCreateInstance`].
		///
		/// Example:
		///
		/// ```cpp
		/// wl::ComPtr<IFileOpenDialog> openDlg{};
		/// openDlg.co_create_instance(CLSID_FileOpenDialog);
		/// ```
		///
		/// [`IUnknown::Release`]: https://learn.microsoft.com/en-us/windows/win32/api/unknwn/nf-unknwn-iunknown-release
		/// [`CoCreateInstance`]: https://learn.microsoft.com/en-us/windows/win32/api/combaseapi/nf-combaseapi-cocreateinstance
		void co_create_instance(REFCLSID clsid, DWORD clsctx = CLSCTX_INPROC_SERVER) {
			release();
			if (HRESULT hr = CoCreateInstance(clsid, nullptr, clsctx, IID_PPV_ARGS(&_p)); FAILED(hr)) [[unlikely]] {
				throw std::system_error(hr, std::system_category(), "CoCreateInstance failed");
			}
		}

		/// Returns a new `ComPtr` by calling [`IUnknown::QueryInterface`].
		///
		/// Example:
		///
		/// ```cpp
		/// wl::ComPtr<IShellItem> item{};
		/// SHCreateItemFromParsingName(L"C:\\Temp\\foo.txt", nullptr, IID_IShellItem, item.pptr());
		///
		/// wl::ComPtr<IShellItem2> item2 = item.query_interface<IShellItem2>();
		/// ```
		///
		/// [`IUnknown::QueryInterface`]: https://learn.microsoft.com/en-us/windows/win32/api/unknwn/nf-unknwn-iunknown-queryinterface(refiid_void)
		template<typename Q>
			requires std::is_base_of_v<IUnknown, Q>
		[[nodiscard]] ComPtr<Q> query_interface() const {
			Q *pQueried = nullptr;
			if (HRESULT hr = _p->QueryInterface(IID_PPV_ARGS(&pQueried)); FAILED(hr)) [[unlikely]] {
				throw std::system_error(hr, std::system_category(), "QueryInterface failed");
			}
			return ComPtr<Q>{pQueried};
		}

		/// Manually calls [`IUnknown::Release`], releasing the pointer
		/// immediately.
		///
		/// This method is automatically called by the `~ComPtr` destructor.
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
