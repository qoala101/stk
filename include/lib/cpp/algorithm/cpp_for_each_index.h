/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_CPP_FOR_EACH_INDEX_H_
#define VH_CPP_FOR_EACH_INDEX_H_

#include <gsl/assert>
#include <optional>
#include <tuple>
#include <type_traits>

#include "cpp_concepts.h"  // IWYU pragma: keep

namespace vh::cpp {
/**
 * @brief Passed to function to get info about the current index.
 */
template <unsigned kCurrentIndex>
struct CurrentIndexTraits {
  static constexpr auto kIndex = kCurrentIndex;
};

namespace detail {
template <unsigned kIndex, typename ResultType>
auto ForEachIndexImpl [[nodiscard]] (const auto &function) {
  if constexpr (!std::is_same_v<ResultType, void>) {
    static_assert(Optional<ResultType>);

    auto result = function(CurrentIndexTraits<kIndex>{});

    if (result.has_value()) {
      return *result;
    }

    if constexpr (kIndex > 0) {
      return ForEachIndexImpl<kIndex - 1, ResultType>(function);
    } else {
      Expects(false);
    }
  } else {
    function(CurrentIndexTraits<kIndex>{});

    if constexpr (kIndex > 0) {
      ForEachIndexImpl<kIndex - 1, ResultType>(function);
    }
  }
}
}  // namespace detail

/**
 * @brief Calls the function for each index.
 * @tparam kSize Number of calls.
 * @param function Function taking current index traits.
 * Has to either return optional value or nothing.
 */
template <unsigned kSize>
auto ForEachIndex [[nodiscard]] (const auto &function) {
  using ResultType = decltype(function(CurrentIndexTraits<0>{}));

  return detail::ForEachIndexImpl<kSize - 1, ResultType>(function);
}
}  // namespace vh::cpp

#endif  // VH_CPP_FOR_EACH_INDEX_H_
