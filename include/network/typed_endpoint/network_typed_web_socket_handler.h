#ifndef STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_TYPED_WEB_SOCKET_HANDLER_H_
#define STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_TYPED_WEB_SOCKET_HANDLER_H_

#include "cpp_not_null.h"
#include "network_i_web_socket_handler.h"
#include "network_typed_endpoint.h"
#include "network_types.h"

namespace stonks::network {
/**
 * @brief Decorator of web socket handler which invalidates unexpected messages.
 */
class TypedWebSocketHandler : public IWebSocketHandler {
 public:
  /**
   * @param message_type Expected message type. Others will be invalidated.
   */
  TypedWebSocketHandler(ParseTypeCheck received_message_type,
                        cpp::NnUp<IWebSocketHandler> handler);

  /**
   * @brief Validates request prior forwarding it to the handler.
   */
  void HandleMessage(WsMessage message) const override;

 private:
  ParseTypeCheck received_message_type_{};
  cpp::NnUp<IWebSocketHandler> handler_;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_TYPED_WEB_SOCKET_HANDLER_H_
