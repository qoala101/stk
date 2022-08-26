#ifndef STONKS_CPP_CPP_CONCEPTS_H_
#define STONKS_CPP_CPP_CONCEPTS_H_

#include <type_traits>
#include <vector>

#include "cpp_optional.h"

/**
 * @file Common concepts.
 */

namespace stonks::cpp {
template <typename T>
concept Const = std::is_const_v<std::remove_reference_t<T>>;

template <typename T>
concept Rvalue = std::is_rvalue_reference_v<T>;

template <typename T>
concept Optional = std::is_same_v<T, Opt<typename T::value_type>>;

template <typename T>
concept Vector = std::is_same_v<T, std::vector<typename T::value_type>>;
}  // namespace stonks::cpp

#endif  // STONKS_CPP_CPP_CONCEPTS_H_
