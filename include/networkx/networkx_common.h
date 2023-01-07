#ifndef STONKS_NETWORKX_NETWORKX_COMMON_H_
#define STONKS_NETWORKX_NETWORKX_COMMON_H_

#include <shared_mutex>
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

/**
 * @brief Protects the call with read-write mutex.
 * @tparam FunctionTraits Callable traits which define async policy.
 */
template <typename FunctionTraits,
          typename ResultType = typename FunctionTraits::ResultType>
auto CallSynchronized(
    const cpp::CallableReturning<cppcoro::task<ResultType>> auto
        &async_callable,
    std::shared_mutex &read_write_mutex) -> cppcoro::task<ResultType> {
  if constexpr (FunctionTraits::IsSynchronized()) {
    if constexpr (FunctionTraits::IsConst()) {
      auto lock = std::shared_lock{read_write_mutex};
      co_return co_await async_callable();
    } else {
      auto lock = std::unique_lock{read_write_mutex};
      co_return co_await async_callable();
    }
  } else {
    co_return co_await async_callable();
  }
}
}  // namespace stonks::networkx

#endif  // STONKS_NETWORKX_NETWORKX_COMMON_H_
