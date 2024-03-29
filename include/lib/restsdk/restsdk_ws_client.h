/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_RESTSDK_WS_CLIENT_H_
#define VH_RESTSDK_WS_CLIENT_H_

#include <cppcoro/task.hpp>

#include "cpp_not_null.h"
#include "log_i_logger.h"
#include "network_i_ws_client.h"
#include "network_i_ws_message_handler.h"
#include "network_ws_types.h"

// IWYU pragma: no_include <cpprest/ws_client.h>

namespace web::websockets::client {
class websocket_callback_client;  // IWYU pragma: keep
}  // namespace web::websockets::client

namespace vh::restsdk {
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
  auto SendMessage [[nodiscard]] (network::WsMessage message) const
      -> cppcoro::task<> override;

 private:
  cpp::NnUp<web::websockets::client::websocket_callback_client>
      native_ws_client_;
  cpp::NnSp<log::ILogger> logger_;
};
}  // namespace vh::restsdk

#endif  // VH_RESTSDK_WS_CLIENT_H_
