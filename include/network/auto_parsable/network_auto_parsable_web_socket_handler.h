#ifndef STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_WEB_SOCKET_HANDLER_H_
#define STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_WEB_SOCKET_HANDLER_H_

#include <utility>

#include "network_aprh_handler_variant.h"
#include "network_auto_parsable.h"
#include "network_i_web_socket_handler.h"
#include "network_types.h"

namespace stonks::network {
/**
 * @brief Convenient request handler constructible from any callable which may
 * take auto-parsable request and return any convertible value.
 */
class AutoParsableWebSocketHandler : public IWebSocketHandler {
 public:
  template <cpp::VoidInvocable T>
  // NOLINTNEXTLINE(*-forwarding-reference-overload)
  explicit AutoParsableWebSocketHandler(T &&handler)
      : handler_{aprh::WsHandlerVariant::ValueType{
            std::in_place_type_t<aprh::Handler>{}, std::forward<T>(handler)}} {}

  template <cpp::VoidInvocableTakes<AutoParsable> T>
  // NOLINTNEXTLINE(*-forwarding-reference-overload)
  explicit AutoParsableWebSocketHandler(T &&handler)
      : handler_{aprh::WsHandlerVariant::ValueType{
            std::in_place_type_t<aprh::HandlerWithWsMessage>{},
            std::forward<T>(handler)}} {}

  /**
   * @brief Wraps request in auto-parsable and forwards it to the handler.
   */
  void HandleMessage(WsMessage message) const override;

 private:
  mutable aprh::WsHandlerVariant handler_{};
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_WEB_SOCKET_HANDLER_H_
