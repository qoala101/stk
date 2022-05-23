#ifndef STONKS_UTILITY_CONCEPTS_H_
#define STONKS_UTILITY_CONCEPTS_H_

#include <type_traits>
#include <vector>

template <typename T>
concept Number = std::is_integral_v<T> || std::is_floating_point_v<T>;

template <typename T>
concept Enumeration = std::is_enum_v<T>;

template <typename T>
struct IsVector {
  static constexpr auto Value() -> bool { return false; }
};

template <typename T, typename U>
struct IsVector<std::vector<T, U>> {
  static constexpr auto Value() -> bool { return true; }
};

template <typename T>
concept Vector = IsVector<T>::Value();

template <typename T>
concept NotVector = !Vector<T>;

#endif  // STONKS_UTILITY_CONCEPTS_H_
