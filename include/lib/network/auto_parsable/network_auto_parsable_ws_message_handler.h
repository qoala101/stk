/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_NETWORK_AUTO_PARSABLE_WS_MESSAGE_HANDLER_H_
#define VH_NETWORK_AUTO_PARSABLE_WS_MESSAGE_HANDLER_H_

#include <cppcoro/task.hpp>
#include <utility>
#include <variant>

#include "network_aprh_concepts.h"
#include "network_aprh_handler_variant.h"
#include "network_aprh_ws_handler_variant.h"
#include "network_auto_parsable_ws_message.h"
#include "network_i_ws_message_handler.h"
#include "network_ws_types.h"

namespace vh::network {
/**
 * @brief Convenient request handler constructible from any callable which may
 * take auto-parsable request and return any convertible value.
 */
class AutoParsableWsMessageHandler : public IWsMessageHandler {
 public:
  template <aprh::VoidCallable Handler>
  // NOLINTNEXTLINE(*-forwarding-reference-overload)
  explicit AutoParsableWsMessageHandler(Handler &&handler)
      : handler_{aprh::WsHandlerVariant::ValueType{
            std::in_place_type<aprh::Handler>,
            std::forward<Handler>(handler)}} {}

  template <aprh::VoidCallableTaking<AutoParsableWsMessage> Handler>
  // NOLINTNEXTLINE(*-forwarding-reference-overload)
  explicit AutoParsableWsMessageHandler(Handler &&handler)
      : handler_{aprh::WsHandlerVariant::ValueType{
            std::in_place_type<aprh::HandlerWithWsMessage>,
            std::forward<Handler>(handler)}} {}

  /**
   * @brief Wraps request in auto-parsable and forwards it to the handler.
   */
  auto HandleMessage [[nodiscard]] (WsMessage message)
  -> cppcoro::task<> override;

 private:
  aprh::WsHandlerVariant handler_{};
};
}  // namespace vh::network

#endif  // VH_NETWORK_AUTO_PARSABLE_WS_MESSAGE_HANDLER_H_
