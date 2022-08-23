#ifndef STONKS_CPP_CPP_CONCEPTS_H_
#define STONKS_CPP_CPP_CONCEPTS_H_

#include <type_traits>

#include "cpp_optional.h"

/**
 * @file Common concepts.
 */

namespace stonks::cpp {
template <typename T>
concept Optional = std::is_same_v<T, Opt<typename T::value_type>>;
}  // namespace stonks::cpp

#endif  // STONKS_CPP_CPP_CONCEPTS_H_
