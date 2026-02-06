#pragma once
#include <concepts>
#include <optional>
#include <ranges>
#include <span>
#include <vector>

/// @brief Utilities for [`vector`], [`span`] and [`array`].
///
/// [`vector`]: https://en.cppreference.com/w/cpp/container/vector.html
/// [`span`]: https://en.cppreference.com/w/cpp/container/span.html
/// [`array`]: https://en.cppreference.com/w/cpp/container/array.html
namespace wl::vec {

	/// Returns true if all elements in `v` are equal to `elem`.
	///
	/// Example:
	///
	/// ```cpp
	/// std::vector<int> nums{2, 2, 2, 2};
	/// bool allTwo = wl::vec::all(nums, 2);
	/// ```
	template<
		std::ranges::contiguous_range R,
		typename T = std::remove_reference_t<std::ranges::range_reference_t<R>> // https://stackoverflow.com/q/78827063
	> requires std::ranges::sized_range<R>
	[[nodiscard]] constexpr bool all(R &&v, const std::type_identity_t<T> &elem) {
		for (auto it = v.begin(); it != v.end(); ++it) {
			if (*it != elem) return false;
		}
		return true;
	}

	/// Returns true if `pred` returns true for all elements in `v`.
	///
	/// Example:
	///
	/// ```cpp
	/// std::vector<int> nums{2, 2, 2, 2};
	/// bool allTwo = wl::vec::all_if(nums, [](const int &n) -> bool {
	///     return n == 2;
	/// });
	/// ```
	template<
		std::ranges::contiguous_range R,
		typename T = std::remove_reference_t<std::ranges::range_reference_t<R>>
	> requires std::ranges::sized_range<R>
	[[nodiscard]] bool all_if(R &&v, std::predicate<T> auto &&pred) {
		for (auto it = v.begin(); it != v.end(); ++it) {
			if (!pred(*it)) return false;
		}
		return true;
	}

	/// Returns true if one of the elements in `v` is equal to `elem`.
	///
	/// Example:
	///
	/// ```cpp
	/// std::vector<int> nums{1, 2, 3, 4};
	/// bool anyTwo = wl::vec::any(nums, 2);
	/// ```
	template<
		std::ranges::contiguous_range R,
		typename T = std::remove_reference_t<std::ranges::range_reference_t<R>>
	> requires std::ranges::sized_range<R>
	[[nodiscard]] constexpr bool any(R &&v, const std::type_identity_t<T> &elem) {
		return std::find(v.begin(), v.end(), elem) != v.end();
	}

	/// Returns true if `pred` returns true for any of the elements in `v`.
	///
	/// Example:
	///
	/// ```cpp
	/// std::vector<int> nums{1, 2, 3, 4};
	/// bool anyTwo = wl::vec::any_if(nums, [](const int &n) -> bool {
	///     return n == 2;
	/// });
	/// ```
	template<
		std::ranges::contiguous_range R,
		typename T = std::remove_reference_t<std::ranges::range_reference_t<R>>
	> requires std::ranges::sized_range<R>
	[[nodiscard]] bool any_if(R &&v, std::predicate<T> auto &&pred) {
		return std::find_if(v.begin(), v.end(), pred) != v.end();
	}

	/// Appends all elements of `other` into `dest`.
	///
	/// Example:
	///
	/// ```cpp
	/// std::vector<int> nums{1, 2, 3, 4};
	/// std::vector<int> more{5, 6, 7};
	/// wl::vec::append(nums, more);
	/// ```
	template<
		std::ranges::contiguous_range R,
		typename T = std::remove_reference_t<std::ranges::range_reference_t<R>>
	> requires std::ranges::sized_range<R>
	void append(std::vector<T> &dest, R &&other) {
		dest.insert(dest.end(), other.begin(), other.end());
	}

	/// Appends multiple elements to `dest`.
	///
	/// Example:
	///
	/// ```cpp
	/// std::vector<int> nums{1, 2, 3, 4};
	/// wl::vec::append(nums, {5, 6, 7});
	/// ```
	template<typename T>
	void append(std::vector<T> &dest, std::initializer_list<const std::type_identity_t<T>> elems) {
		dest.insert(dest.end(), elems.begin(), elems.end());
	}

	/// Returns true if `v` and `q` have the same length, and all elements are
	/// equal.
	///
	/// Example:
	///
	/// ```cpp
	/// std::vector<int> nums{1, 2, 3, 4};
	/// std::vector<int> other{1, 2, 3, 4};
	/// bool theSame = wl::vec::eq(nums, other);
	/// ```
	template<
		std::ranges::contiguous_range R,
		std::ranges::contiguous_range Q
	> requires std::ranges::sized_range<R>
		&& std::ranges::contiguous_range<Q>
		&& std::same_as<std::ranges::range_value_t<R>, std::ranges::range_value_t<Q>>
	[[nodiscard]] constexpr bool eq(R &&v, Q &&q) {
		if (std::ranges::size(v) != std::ranges::size(q))
			return false;
		for (size_t i = 0; i < std::ranges::size(v); ++i) {
			if (v[i] != q[i]) return false;
		}
		return true;
	}

	/// Returns true if `v` and `sequence` have the same length, and all elements
	/// are equal.
	///
	/// Example:
	///
	/// ```cpp
	/// std::vector<int> nums{1, 2, 3, 4};
	/// bool theSame = wl::vec::eq(nums, {1, 2, 3, 4});
	/// ```
	template<
		std::ranges::contiguous_range R,
		typename T = std::remove_reference_t<std::ranges::range_reference_t<R>>
	> requires std::ranges::sized_range<R>
	[[nodiscard]] constexpr bool eq(
			R &&v, std::initializer_list<const std::type_identity_t<T>> sequence) { // initializer_list overload
		return eq(std::forward<R>(v), std::span(sequence));
	}

	/// Returns a pointer to the first found element, or `nullptr`.
	///
	/// Example:
	///
	/// ```cpp
	/// std::vector<int> nums{1, 2, 3, 4};
	/// if (int *pFound = wl::vec::find(nums, 2); pFound) {
	///     // ...
	/// }
	/// ```
	template<
		std::ranges::contiguous_range R,
		typename T = std::remove_reference_t<std::ranges::range_reference_t<R>>
	> requires std::ranges::sized_range<R>
	[[nodiscard]] constexpr T* find(R &&v, const std::type_identity_t<T> &elem) {
		auto foundIt = std::find(v.begin(), v.end(), elem);
		return (foundIt == v.end()) ? nullptr : &(*foundIt);
	}

	/// Returns a pointer to the first element according to `pred`, or nullptr.
	///
	/// Example:
	///
	/// ```cpp
	/// std::vector<int> nums{1, 2, 3, 4};
	/// int *pFound = wl::vec::find_if(nums, [](const int &n) -> bool {
	///     return n == 2;
	/// });
	/// if (pFound) {
	///     // ...
	/// }
	/// ```
	template<
		std::ranges::contiguous_range R,
		typename T = std::remove_reference_t<std::ranges::range_reference_t<R>>
	> requires std::ranges::sized_range<R>
	[[nodiscard]] T* find_if(R &&v, std::predicate<T> auto &&pred) {
		auto foundIt = std::find_if(v.begin(), v.end(), pred);
		return (foundIt == v.end()) ? nullptr : &(*foundIt);
	}

	/// Returns a pointer to the last found element, or `nullptr`.
	///
	///
	/// Example:
	///
	/// ```cpp
	/// std::vector<int> nums{1, 2, 3, 4};
	/// if (int *pFound = wl::vec::find_rev(nums, 2); pFound) {
	///     // ...
	/// }
	/// ```
	template<
		std::ranges::contiguous_range R,
		typename T = std::remove_reference_t<std::ranges::range_reference_t<R>>
	> requires std::ranges::sized_range<R>
	[[nodiscard]] constexpr T* find_rev(R &&v, const std::type_identity_t<T> &elem) {
		auto foundIt = std::find(v.rbegin(), v.rend(), elem);
		return (foundIt == v.rend()) ? nullptr : &(*foundIt);
	}

	/// Returns a pointer to the last element according to `pred`, or `nullptr`.
	///
	/// Example:
	///
	/// ```cpp
	/// std::vector<int> nums{1, 2, 3, 4};
	/// int *pFound = wl::vec::find_rev_if(nums, [](const int &n) -> bool {
	///     return n == 2;
	/// });
	/// if (pFound) {
	///     // ...
	/// }
	/// ```
	template<
		std::ranges::contiguous_range R,
		typename T = std::remove_reference_t<std::ranges::range_reference_t<R>>
	> requires std::ranges::sized_range<R>
	[[nodiscard]] T* find_rev_if(R &&v, std::predicate<T> auto &&pred) {
		auto foundIt = std::find_if(v.rbegin(), v.rend(), pred);
		return (foundIt == v.rend()) ? nullptr : &(*foundIt);
	}

	/// Returns the index of the first found element.
	///
	/// Example:
	///
	/// ```cpp
	/// std::vector<int> nums{1, 2, 3, 4};
	/// std::optional<size_t> idx2 = wl::vec::index(nums, 2);
	/// ```
	template<
		std::ranges::contiguous_range R,
		typename T = std::remove_reference_t<std::ranges::range_reference_t<R>>
	> requires std::ranges::sized_range<R>
	[[nodiscard]] constexpr std::optional<size_t> index(R &&v, const std::type_identity_t<T> &elem) {
		auto foundIt = std::find(v.begin(), v.end(), elem);
		return (foundIt == v.end()) ? std::nullopt : std::make_optional(std::distance(v.begin(), foundIt));
	}

	/// Returns the first index according to `pred`.
	///
	/// Example:
	///
	/// ```cpp
	/// std::vector<int> nums{1, 2, 3, 4};
	/// std::optional<size_t> idx2 = wl::vec::index_if(nums, [](const int &n) -> bool {
	///     return n == 2;
	/// });
	/// ```
	template<
		std::ranges::contiguous_range R,
		typename T = std::remove_reference_t<std::ranges::range_reference_t<R>>
	> requires std::ranges::sized_range<R>
	[[nodiscard]] std::optional<size_t> index_if(R &&v, std::predicate<T> auto &&pred) {
		auto foundIt = std::find_if(v.begin(), v.end(), pred);
		return (foundIt == v.end()) ? std::nullopt : std::make_optional(std::distance(v.begin(), foundIt));
	}

	/// Returns the index of the last found element.
	///
	/// Example:
	///
	/// ```cpp
	/// std::vector<int> nums{1, 2, 3, 4};
	/// std::optional<size_t> idx2 = wl::vec::index_rev(nums, 2);
	/// ```
	template<
		std::ranges::contiguous_range R,
		typename T = std::remove_reference_t<std::ranges::range_reference_t<R>>
	> requires std::ranges::sized_range<R>
	[[nodiscard]] constexpr std::optional<size_t> index_rev(R &&v, const std::type_identity_t<T> &elem) {
		auto foundIt = std::find(v.rbegin(), v.rend(), elem);
		return (foundIt == v.rend()) ? std::nullopt : std::make_optional(std::distance(foundIt, std::prev(v.rend())));
	}

	/// Returns the last index according to `pred`.
	///
	/// Example:
	///
	/// ```cpp
	/// std::vector<int> nums{1, 2, 3, 4};
	/// std::optional<size_t> idx2 = wl::vec::index_rev_if(nums, [](const int &n) -> bool {
	///     return n == 2;
	/// });
	/// ```
	template<
		std::ranges::contiguous_range R,
		typename T = std::remove_reference_t<std::ranges::range_reference_t<R>>
	> requires std::ranges::sized_range<R>
	[[nodiscard]] std::optional<size_t> index_rev_if(R &&v, std::predicate<T> auto &&pred) {
		auto foundIt = std::find_if(v.rbegin(), v.rend(), pred);
		return (foundIt == v.rend()) ? std::nullopt : std::make_optional(std::distance(foundIt, std::prev(v.rend())));
	}

	/// Returns the index of the first element where the entire `sequence`
	/// starts, if any.
	///
	/// Example:
	///
	/// ```cpp
	/// std::vector<int> nums{1, 2, 3, 4};
	/// const int SEQ[] = {2, 3};
	/// std::optional<size_t> idx = wl::vec::index_seq(nums, SEQ);
	/// ```
	template<
		std::ranges::contiguous_range R,
		std::ranges::contiguous_range Q
	> requires std::ranges::sized_range<R>
		&& std::ranges::contiguous_range<Q>
		&& std::same_as<std::ranges::range_value_t<R>, std::ranges::range_value_t<Q>>
	[[nodiscard]] constexpr std::optional<size_t> index_seq(R &&v, Q &&sequence) {
		size_t vSize = std::ranges::size(v);
		size_t seqSize = std::ranges::size(sequence);
		for (size_t i = 0; i <= vSize - seqSize; ++i) {
			bool found = true;
			for (size_t e = 0; e < seqSize; ++e) {
				if (v[e + i] != sequence[e]) {
					found = false;
					break;
				}
			}
			if (found) return std::make_optional(i);
		}
		return std::nullopt;
	}

	/// Returns the index of the first element where the entire `sequence`
	/// starts, if any.
	///
	/// Example:
	///
	/// ```cpp
	/// std::vector<int> nums{1, 2, 3, 4};
	/// std::optional<size_t> idx = wl::vec::index_seq(nums, {2, 3});
	/// ```
	template<
		std::ranges::contiguous_range R,
		typename T = std::remove_reference_t<std::ranges::range_reference_t<R>>
	> requires std::ranges::sized_range<R>
	[[nodiscard]] constexpr std::optional<size_t> index_seq(
			R &&v, std::initializer_list<const std::type_identity_t<T>> sequence) { // initializer_list overload
		return index_seq(std::forward<R>(v), std::span(sequence));
	}

	/** Removes the element at the given zero-based index. */
	template<typename T>
	void remove(std::vector<T> &v, size_t itemIndex) {
		v.erase(v.begin() + itemIndex);
	}

	/// Removes all the elements to which `pred` returns true.
	///
	/// Example:
	///
	/// ```cpp
	/// std::vector<int> nums{1, 2, 3, 4};
	/// wl::vec::remove_if(nums, [](const int& n) -> bool {
	///     return n < 2;
	/// });
	/// ```
	template<typename T>
	void remove_if(std::vector<T> &v, std::predicate<T> auto &&pred) {
		v.erase(std::remove_if(v.begin(), v.end(), pred), v.end());
	}

	/// Returns spans over the `v`, splitted by `delimiter`, including empty
	/// spans.
	///
	/// Example:
	///
	/// ```cpp
	/// std::vector<int> nums{1, 2, 99, 3, 4, 99, 5};
	/// std::vector<std::span<int>> parts = wl::vec::split(nums, 99);
	/// ```
	template<
		std::ranges::contiguous_range R,
		typename T = std::remove_reference_t<std::ranges::range_reference_t<R>>
	> requires std::ranges::sized_range<R>
	[[nodiscard]] std::vector<std::span<T>> split(
			R &&v, const std::type_identity_t<T> &delimiter, std::optional<size_t> maxParts = std::nullopt) {
		if (v.empty()) return {};

		std::span<T> src{v};
		size_t count = 0, head = 0;
		for (;;) {
			if (maxParts.has_value() && maxParts.value() && count >= maxParts.value() - 1)
				break;

			std::optional<size_t> maybeHead = index(src, delimiter);
			if (!maybeHead.has_value()) {
				break;
			} else {
				head = maybeHead.value();
				++count;
				++head;
				src = src.subspan(head);
			}
		}
		++count;

		std::vector<std::span<T>> ret;
		ret.reserve(count);

		src = v;
		head = 0;
		for (;;) {
			if (maxParts.has_value() && maxParts.value() && ret.size() >= maxParts.value() - 1)
				break;

			std::optional<size_t> maybeHead = index(src, delimiter);
			if (!maybeHead.has_value()) {
				break;
			} else {
				head = maybeHead.value();
				ret.emplace_back(src.subspan(0, head));
				++head;
				src = src.subspan(head);
			}
		}

		ret.emplace_back(src);
		return ret;
	}

	/// Returns a new vector populated with the results of calling `cb` on each
	/// element of `v`.
	///
	/// Example:
	///
	/// ```cpp
	/// std::vector<int> nums{1, 2, 3, 4};
	/// std::vector<std::wstring> strs = wl::vec::transform(nums, [](const int& n) -> std::wstring {
	///     return L"num " + std::to_wstring(n);
	/// });
	/// ```
	template<
		std::ranges::contiguous_range R,
		typename T = std::remove_reference_t<std::ranges::range_reference_t<R>>,
		typename F = std::is_invocable<const std::type_identity_t<T>&>,
		typename U = std::invoke_result_t<F, const std::type_identity_t<T>&>
	> requires std::ranges::sized_range<R>
	[[nodiscard]] std::vector<U> transform(R &&v, F &&cb) {
		std::vector<U> ret;
		ret.reserve(v.size());
		for (auto &&elem : v)
			ret.emplace_back(cb(elem));
		return ret;
	}

}
