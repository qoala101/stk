#ifndef STONKS_CPP_CPP_AS_SP_H_
#define STONKS_CPP_CPP_AS_SP_H_

#include "cpp_not_null.h"

namespace stonks::cpp {
/**
 * @brief Explicit conversion to shared pointer.
 */
template <typename T>
auto Share(cpp::Up<T> t) {
  return cpp::Sp<T>{std::move(t)};
}

/**
 * @copydoc Share
 */
template <typename T>
auto Share(cpp::NnUp<T> t) {
  return cpp::NnSp<T>{std::move(t)};
}
}  // namespace stonks::cpp

#endif  // STONKS_CPP_CPP_AS_SP_H_
