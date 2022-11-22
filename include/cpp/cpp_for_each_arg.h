#ifndef STONKS_CPP_CPP_FOR_EACH_ARG_H_
#define STONKS_CPP_CPP_FOR_EACH_ARG_H_

#include <utility>

namespace stonks::cpp {
namespace detail {
template <unsigned kNumArgsPerIteration, typename... Args>
struct ForEachArgImpl {};

template <typename... Args>
struct ForEachArgImpl<1, Args...> {
  template <typename Arg, typename... OtherArgs>
  void operator()(const auto &function, Arg &&arg, OtherArgs &&...args) {
    function(std::forward<Arg>(arg));

    if constexpr (sizeof...(OtherArgs) > 0) {
      (*this)(function, std::forward<OtherArgs>(args)...);
    }
  }
};

template <typename... Args>
struct ForEachArgImpl<2, Args...> {
  template <typename Arg1, typename Arg2, typename... OtherArgs>
  void operator()(const auto &function, Arg1 &&arg1, Arg2 &&arg2,
                  OtherArgs &&...args) {
    function(std::forward<Arg1>(arg1), std::forward<Arg2>(arg2));

    if constexpr (sizeof...(OtherArgs) > 0) {
      (*this)(function, std::forward<OtherArgs>(args)...);
    }
  }
};
}  // namespace detail

/**
 * @brief Calls the function for each argument.
 * @tparam kNumArgsPerIteration Number of arguments the function takes.
 */
template <unsigned kNumArgsPerIteration = 1, typename... Args>
void ForEachArg(const auto &function, Args &&...args) {
  static_assert((sizeof...(Args) % kNumArgsPerIteration) == 0);

  detail::ForEachArgImpl<kNumArgsPerIteration, Args...>{}(
      function, std::forward<Args>(args)...);
}
}  // namespace stonks::cpp

#endif  // STONKS_CPP_CPP_FOR_EACH_ARG_H_
