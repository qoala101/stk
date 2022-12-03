#ifndef STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_TYPED_WS_MESSAGE_HANDLER_H_
#define STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_TYPED_WS_MESSAGE_HANDLER_H_

#include <cppcoro/task.hpp>

#include "cpp_not_null.h"
#include "network_i_ws_message_handler.h"
#include "network_typed_endpoint.h"
#include "network_ws_types.h"

namespace stonks::network {
/**
 * @brief Decorator of web socket handler which invalidates unexpected messages.
 */
class TypedWsMessageHandler : public IWsMessageHandler {
 public:
  /**
   * @param message_type Expected message type. Others will be invalidated.
   */
  TypedWsMessageHandler(ParseTypeCheck received_message_type,
                        cpp::NnUp<IWsMessageHandler> handler);

  /**
   * @brief Validates request prior forwarding it to the handler.
   */
  auto HandleMessage [[nodiscard]] (WsMessage message) const
      -> cppcoro::task<> override;

 private:
  ParseTypeCheck received_message_type_{};
  cpp::NnUp<IWsMessageHandler> handler_;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_TYPED_WS_MESSAGE_HANDLER_H_
