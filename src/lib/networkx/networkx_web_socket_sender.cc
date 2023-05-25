/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "networkx_web_socket_sender.h"

#include "network_ws_client_builder.h"

namespace vh::networkx::detail {
WebSocketSenderBase::WebSocketSenderBase(network::WsConnection connection)
    : connection_{std::move(connection)} {}

WebSocketSenderBase::WebSocketSenderBase(
    network::WsEndpoint endpoint, cpp::NnUp<network::IWsClient> ws_client,
    network::ParseTypeCheck sent_message_type)
    : WebSocketSenderBase{network::WsClientBuilder{
          {.endpoint = std::move(endpoint),
           .expected_types = {.sent_message = std::move(sent_message_type)}},
          std::move(ws_client)}
                              .Connect()} {}

auto WebSocketSenderBase::GetConnection() const
    -> const network::WsConnection& {
  return connection_;
}
}  // namespace vh::networkx::detail