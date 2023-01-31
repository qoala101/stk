#ifndef STONKS_CPP_CPP_THIS_H_
#define STONKS_CPP_CPP_THIS_H_

#include <type_traits>

#include "cpp_concepts.h"  // IWYU pragma: keep

namespace vh::cpp {
/**
 * @brief Explicit narrowing to be used for the member instance
 * passed to the static functions.
 */
template <typename T, typename U>
concept This = std::same_as<std::remove_const_t<std::remove_reference_t<T>>, U>;
}  // namespace vh::cpp

#endif  // STONKS_CPP_CPP_THIS_H_
