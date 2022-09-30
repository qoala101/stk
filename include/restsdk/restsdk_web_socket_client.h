#ifndef STONKS_RESTSDK_RESTSDK_WEB_SOCKET_CLIENT_H_
#define STONKS_RESTSDK_RESTSDK_WEB_SOCKET_CLIENT_H_

#include "cpp_not_null.h"
#include "log_i_logger.h"
#include "network_i_web_socket_client.h"
#include "network_i_web_socket_handler.h"
#include "network_types.h"

namespace web::websockets::client {
class websocket_callback_client;
}  // namespace web::websockets::client

namespace stonks::restsdk {
/**
 * @copydoc network::IWebSocketClient
 */
class WebSocketClient : public network::IWebSocketClient {
 public:
  explicit WebSocketClient(cpp::NnSp<log::ILogger> logger);

  WebSocketClient(const WebSocketClient &) = delete;
  WebSocketClient(WebSocketClient &&) noexcept;

  auto operator=(const WebSocketClient &) -> WebSocketClient & = delete;
  auto operator=(WebSocketClient &&) noexcept -> WebSocketClient &;

  ~WebSocketClient() override;

  /**
   * @copydoc network::IWebSocketClient::Connect
   */
  void Connect(network::WsEndpoint endpoint) override;

  /**
   * @copydoc network::IWebSocketClient::SetMessagesHandler
   */
  void SetMessageHandler(
      cpp::NnSp<network::IWebSocketHandler> handler) override;

  /**
   * @copydoc network::IWebSocketClient::SendMessage
   */
  void SendMessage(network::WsMessage message) const override;

 private:
  cpp::NnUp<web::websockets::client::websocket_callback_client>
      web_socket_client_;
  cpp::NnSp<log::ILogger> logger_;
};
}  // namespace stonks::restsdk

#endif  // STONKS_RESTSDK_RESTSDK_WEB_SOCKET_CLIENT_H_
