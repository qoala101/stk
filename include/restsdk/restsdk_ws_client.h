#ifndef STONKS_RESTSDK_RESTSDK_WS_CLIENT_H_
#define STONKS_RESTSDK_RESTSDK_WS_CLIENT_H_

#include "cpp_not_null.h"
#include "log_i_logger.h"
#include "network_i_ws_client.h"
#include "network_i_ws_message_handler.h"
#include "network_types.h"

namespace web::websockets::client {
class websocket_callback_client;
class websocket_incoming_message;
}  // namespace web::websockets::client

namespace stonks::restsdk {
/**
 * @copydoc network::IWsClient
 */
class WsClient : public network::IWsClient {
 public:
  explicit WsClient(cpp::NnUp<log::ILogger> logger);

  WsClient(const WsClient &) = delete;
  WsClient(WsClient &&) noexcept;

  auto operator=(const WsClient &) -> WsClient & = delete;
  auto operator=(WsClient &&) noexcept -> WsClient &;

  ~WsClient() override;

  /**
   * @copydoc network::IWsClient::Connect
   */
  void Connect(network::WsEndpoint endpoint) override;

  /**
   * @copydoc network::IWsClient::SetMessagesHandler
   */
  void SetMessageHandler(
      cpp::NnUp<network::IWsMessageHandler> handler) override;

  /**
   * @copydoc network::IWsClient::SendMessage
   */
  void SendMessage(network::WsMessage message) const override;

 private:
  void HandleWsMessage(
      const web::websockets::client::websocket_incoming_message &message);

  cpp::NnUp<web::websockets::client::websocket_callback_client>
      native_ws_client_;
  cpp::NnUp<log::ILogger> logger_;
  cpp::Up<network::IWsMessageHandler> handler_{};
};
}  // namespace stonks::restsdk

#endif  // STONKS_RESTSDK_RESTSDK_WS_CLIENT_H_
