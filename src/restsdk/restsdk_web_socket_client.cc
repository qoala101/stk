#include "restsdk_web_socket_client.h"

#include <cpprest/json.h>
#include <cpprest/ws_client.h>
#include <cpprest/ws_msg.h>
#include <fmt/core.h>
#include <pplx/pplxtasks.h>

#include <functional>
#include <gsl/assert>
#include <memory>
#include <utility>

#include "cpp_not_null.h"
#include "network_i_json.h"
#include "not_null.hpp"
#include "restsdk_json_native_handle.h"
#include "restsdk_parse_json_fom_string.h"

namespace stonks::restsdk {
WebSocketClient::WebSocketClient(cpp::NnUp<log::ILogger> logger)
    : web_socket_client_{cpp::MakeNnUp<
          web::websockets::client::websocket_callback_client>()},
      logger_{std::move(logger)} {}

WebSocketClient::WebSocketClient(WebSocketClient &&) noexcept = default;

auto WebSocketClient::operator=(WebSocketClient &&) noexcept
    -> WebSocketClient & = default;

WebSocketClient::~WebSocketClient() = default;

void WebSocketClient::Connect(network::WsEndpoint endpoint) {
  logger_->LogImportantEvent(
      fmt::format("Connecting to web socket: {}...", endpoint.value.value));

  web_socket_client_->connect(endpoint.value.value).wait();

  logger_->LogImportantEvent(
      fmt::format("Connected to web socket: {}", endpoint.value.value));
}

void WebSocketClient::SetMessageHandler(
    cpp::NnUp<network::IWebSocketHandler> handler) {
  handler_ = std::move(handler).as_nullable();

  web_socket_client_->set_message_handler(
      std::bind_front(&WebSocketClient::HandleWsMessage, this));

  Ensures(handler_ != nullptr);
}

void WebSocketClient::SendMessage(network::WsMessage message) const {
  auto web_socket_message =
      web::websockets::client::websocket_outgoing_message{};
  web_socket_message.set_utf8_message(message->GetNativeHandle()->serialize());

  web_socket_client_->send(std::move(web_socket_message)).wait();
}

void WebSocketClient::HandleWsMessage(
    const web::websockets::client::websocket_incoming_message &message) {
  Expects(handler_ != nullptr);

  if (const auto non_text_message =
          message.message_type() !=
          web::websockets::client::websocket_message_type::text_message) {
    return;
  }

  const auto message_text = message.extract_string().get();
  handler_->HandleMessage(ParseJsonFromString(message_text));
}
}  // namespace stonks::restsdk