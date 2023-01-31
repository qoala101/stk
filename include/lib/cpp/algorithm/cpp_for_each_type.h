#ifndef VH_CPP_FOR_EACH_TYPE_H_
#define VH_CPP_FOR_EACH_TYPE_H_

namespace vh::cpp {
/**
 * @brief Passed to function to get info about the current type.
 */
template <typename T>
struct CurrentTypeTraits {
  using Type = T;
};

namespace detail {
template <typename Type, typename... OtherTypes>
void ForEachTypeImpl(const auto &function) {
  function(CurrentTypeTraits<Type>{});

  if constexpr (sizeof...(OtherTypes) > 0) {
    ForEachTypeImpl<OtherTypes...>(function);
  }
}
}  // namespace detail

/**
 * @brief Calls the function for each type parameter.
 * @param function Function taking current type traits.
 */
template <typename... Types>
void ForEachType(const auto &function) {
  detail::ForEachTypeImpl<Types...>(function);
}
}  // namespace vh::cpp

#endif  // VH_CPP_FOR_EACH_TYPE_H_
