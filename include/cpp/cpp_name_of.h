#ifndef STONKS_CPP_CPP_TYPE_NAME_H_
#define STONKS_CPP_CPP_TYPE_NAME_H_

#include <nameof.hpp>

namespace stonks::cpp {
/**
 * @brief Gives short name of the type.
 */
template <typename T>
[[nodiscard]] auto NameOf() -> auto& {
  static const auto kConstant = std::string{nameof::nameof_short_type<T>()};
  return kConstant;
}
}  // namespace stonks::cpp

#endif  // STONKS_CPP_CPP_TYPE_NAME_H_
