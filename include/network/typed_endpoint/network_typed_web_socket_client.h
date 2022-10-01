#ifndef STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_TYPED_WEB_SOCKET_CLIENT_H_
#define STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_TYPED_WEB_SOCKET_CLIENT_H_

#include "cpp_not_null.h"
#include "network_i_web_socket_client.h"
#include "network_i_web_socket_handler.h"
#include "network_typed_endpoint.h"
#include "network_types.h"

namespace stonks::network {
/**
 * @brief Decorator of web socket client which invalidates unexpected messages.
 */
class TypedWebSocketClient : public IWebSocketClient {
 public:
  explicit TypedWebSocketClient(WsEndpointTypes endpoint_types,
                                cpp::NnUp<IWebSocketClient> web_socket_client);

  /**
   * @copydoc IWebSocketClient::Connect
   */
  void Connect(WsEndpoint endpoint) override;

  /**
   * @copydoc IWebSocketClient::SetMessagesHandler
   */
  void SetMessageHandler(cpp::NnUp<IWebSocketHandler> handler) override;

  /**
   * @copydoc IWebSocketClient::SendMessage
   */
  void SendMessage(WsMessage message) const override;

 private:
  WsEndpointTypes endpoint_types_{};
  cpp::NnUp<IWebSocketClient> web_socket_client_;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_TYPED_WEB_SOCKET_CLIENT_H_
