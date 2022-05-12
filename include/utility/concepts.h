#ifndef STONKS_UTILITY_CONCEPTS_H_
#define STONKS_UTILITY_CONCEPTS_H_

#include <type_traits>

template <typename T>
concept Number = std::is_integral_v<T> || std::is_floating_point_v<T>;

template <typename T>
concept Enumeration = std::is_enum_v<T>;

#endif  // STONKS_UTILITY_CONCEPTS_H_
