#ifndef STONKS_NETWORKX_NETWORKX_WEB_SOCKET_RECEIVER_TRAITS_FACADE_H_
#define STONKS_NETWORKX_NETWORKX_WEB_SOCKET_RECEIVER_TRAITS_FACADE_H_

#include <callable.hpp>  // IWYU pragma: keep
#include <member_function.hpp>

#include "cpp_concepts.h"       // IWYU pragma: keep
#include "networkx_concepts.h"  // IWYU pragma: keep
#include "networkx_types.h"

namespace stonks::networkx {
/**
 * @brief Convenience API over web socket receiver traits.
 */
template <cpp::MemberFunction auto kFunction,
          typename FunctionTraits = WebSocketReceiverTraits<kFunction>>
  requires WebSocketReceiver<kFunction> || WebSocketReceiverSender<kFunction>
struct WebSocketReceiverTraitsFacade : public FunctionTraits {
  /**
   * @brief Tells whether receiver is const.
   */
  static constexpr auto IsConst [[nodiscard]] () {
    return member_function_traits<decltype(kFunction)>::is_const;
  }
};
}  // namespace stonks::networkx

#endif  // STONKS_NETWORKX_NETWORKX_WEB_SOCKET_RECEIVER_TRAITS_FACADE_H_
