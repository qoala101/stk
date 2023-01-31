#ifndef VH_NETWORK_TYPED_WS_CLIENT_H_
#define VH_NETWORK_TYPED_WS_CLIENT_H_

#include <cppcoro/task.hpp>

#include "cpp_not_null.h"
#include "network_i_ws_client.h"
#include "network_i_ws_message_handler.h"
#include "network_typed_ws_endpoint.h"
#include "network_ws_types.h"

namespace vh::network {
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
   * @copydoc IWsClient::SetMessagesHandler
   */
  void SetMessageHandler(cpp::NnUp<IWsMessageHandler> handler) override;

  /**
   * @copydoc IWsClient::SendMessage
   */
  auto SendMessage [[nodiscard]] (WsMessage message) const
      -> cppcoro::task<> override;

 private:
  WsEndpointTypes endpoint_types_{};
  cpp::NnUp<IWsClient> ws_client_;
};
}  // namespace vh::network

#endif  // VH_NETWORK_TYPED_WS_CLIENT_H_
