#pragma once

// Proper #include <Windows.h>

#include <sdkddkver.h>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace wl {

	/// @brief Makes the derived class non-copyable.
	///
	/// Implements [P2895] with [C.35 core guideline].
	///
	/// Example:
	///
	///     class Foo final : wl::NonCopyable {
	///     public:
	///         std::wstring name{};
	///     };
	///
	/// [P2895]: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p2895r0.html
	/// [C.35 core guideline]: https://www.sandordargo.com/blog/2024/11/27/non-movable-classes
	class NonCopyable {
	protected:
		~NonCopyable() = default;
	public:
		NonCopyable() = default;
		NonCopyable(NonCopyable&&) = default;
		NonCopyable& operator=(NonCopyable&&) = default;
	};

	/// @brief Makes the derived class non-copyable and non-movable.
	///
	/// Implements [P2895] with [C.35 core guideline].
	///
	/// Example:
	///
	///     class Foo final : wl::NonMovable {
	///     public:
	///         std::wstring name{};
	///     };
	///
	/// [P2895]: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p2895r0.html
	/// [C.35 core guideline]: https://www.sandordargo.com/blog/2024/11/27/non-movable-classes
	class NonMovable {
	protected:
		~NonMovable() = default;
	public:
		NonMovable() = default;
		NonMovable(NonMovable const&) = delete;
		NonMovable& operator=(NonMovable const&) = delete;
	};

}
