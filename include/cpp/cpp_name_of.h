#ifndef STONKS_CPP_CPP_TYPE_NAME_H_
#define STONKS_CPP_CPP_TYPE_NAME_H_

#include <gsl/assert>
#include <nameof.hpp>

namespace stonks::cpp {
/**
 * @brief Gives short name of the type.
 */
template <typename T>
[[nodiscard]] auto NameOf() {
  return std::string{nameof::nameof_short_type<T>()};
}
}  // namespace stonks::cpp

#endif  // STONKS_CPP_CPP_TYPE_NAME_H_
