#ifndef STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_TYPED_WS_CLIENT_H_
#define STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_TYPED_WS_CLIENT_H_

#include "cpp_not_null.h"
#include "network_i_ws_client.h"
#include "network_typed_ws_endpoint.h"
#include "network_ws_types.h"

namespace stonks::network {
/**
 * @brief Decorator of web socket client which invalidates unexpected messages.
 */
class TypedWsClient : public IWsClient {
 public:
  TypedWsClient(WsEndpointTypes endpoint_types, cpp::NnUp<IWsClient> ws_client);

  /**
   * @copydoc IWsClient::Connect
   */
  void Connect(WsEndpoint endpoint) override;

  /**
   * @copydoc IWsClient::ReceiveMessage
   */
  auto ReceiveMessage [[nodiscard]] () -> cppcoro::task<WsMessage> override;

  /**
   * @copydoc IWsClient::SendMessage
   */
  auto SendMessage [[nodiscard]] (WsMessage message) const
      -> cppcoro::task<> override;

 private:
  WsEndpointTypes endpoint_types_{};
  cpp::NnUp<IWsClient> ws_client_;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_TYPED_WS_CLIENT_H_
