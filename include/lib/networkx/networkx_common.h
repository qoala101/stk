/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_NETWORKX_COMMON_H_
#define VH_NETWORKX_COMMON_H_

#include <shared_mutex>
#include <tuple>

#include "cpp_concepts.h"       // IWYU pragma: keep
#include "networkx_concepts.h"  // IWYU pragma: keep
#include "networkx_types.h"

namespace vh::networkx {
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
}  // namespace vh::networkx

#endif  // VH_NETWORKX_COMMON_H_
