#ifndef VH_CPP_FOR_EACH_ARG_H_
#define VH_CPP_FOR_EACH_ARG_H_

#include <utility>

namespace vh::cpp {
/**
 * @brief Passed to function to get info about the current argument.
 */
template <unsigned kArgIndex>
struct CurrentArgTraits {
  static constexpr auto kIndex = kArgIndex;
};

namespace detail {
template <unsigned kNumArgsPerIteration, typename... AllArgs>
struct ForEachArgImpl;

template <typename... AllArgs>
struct ForEachArgImpl<1, AllArgs...> {
  template <typename Arg, typename... RemainingArgs>
  void operator()(const auto &function, Arg &&arg,
                  RemainingArgs &&...args) const {
    const auto num_remaining_args = sizeof...(RemainingArgs);
    const auto arg_index = sizeof...(AllArgs) - num_remaining_args - 1;

    function(std::forward<Arg>(arg), CurrentArgTraits<arg_index>{});

    if constexpr (num_remaining_args > 0) {
      (*this)(function, std::forward<RemainingArgs>(args)...);
    }
  }
};

template <typename... AllArgs>
struct ForEachArgImpl<2, AllArgs...> {
  template <typename Arg1, typename Arg2, typename... RemainingArgs>
  void operator()(const auto &function, Arg1 &&arg1, Arg2 &&arg2,
                  RemainingArgs &&...args) const {
    const auto num_remaining_args = sizeof...(RemainingArgs);
    const auto arg_index = sizeof...(AllArgs) - num_remaining_args - 1;

    function(std::forward<Arg1>(arg1), std::forward<Arg2>(arg2),
             CurrentArgTraits<arg_index>{});

    if constexpr (num_remaining_args > 0) {
      (*this)(function, std::forward<RemainingArgs>(args)...);
    }
  }
};
}  // namespace detail

/**
 * @brief Calls the function for each argument.
 * @tparam kNumArgsPerIteration Number of arguments the function takes.
 * @param function Function taking specified number of arguments
 * plus current type traits.
 */
template <unsigned kNumArgsPerIteration = 1, typename... Args>
void ForEachArg(const auto &function, Args &&...args) {
  static_assert((sizeof...(Args) % kNumArgsPerIteration) == 0);

  detail::ForEachArgImpl<kNumArgsPerIteration, Args...>{}(
      function, std::forward<Args>(args)...);
}
}  // namespace vh::cpp

#endif  // VH_CPP_FOR_EACH_ARG_H_
