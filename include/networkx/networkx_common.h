#ifndef STONKS_NETWORKX_NETWORKX_COMMON_H_
#define STONKS_NETWORKX_NETWORKX_COMMON_H_

#include <tuple>

#include "cpp_concepts.h"       // IWYU pragma: keep
#include "networkx_concepts.h"  // IWYU pragma: keep

namespace stonks::networkx {
/**
 * @brief Used to declare endpoint function parameter list.
 */
template <EndpointParam... Args>
constexpr auto ParamList [[nodiscard]] (Args &&...args) {
  return std::make_tuple(std::forward<Args>(args)...);
}

/**
 * @brief Used to declare client-server list of endpoint functions.
 */
template <cpp::MemberFunction... Args>
constexpr auto EndpointFunctionList [[nodiscard]] (Args &&...args) {
  return std::make_tuple(std::forward<Args>(args)...);
}
}  // namespace stonks::networkx

#endif  // STONKS_NETWORKX_NETWORKX_COMMON_H_