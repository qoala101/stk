#include "restsdk_web_socket_client.h"

#include <cpprest/json.h>
#include <cpprest/ws_client.h>
#include <cpprest/ws_msg.h>
#include <fmt/core.h>
#include <pplx/pplxtasks.h>

#include <memory>
#include <utility>

#include "cpp_not_null.h"
#include "network_i_json.h"
#include "not_null.hpp"
#include "restsdk_json_native_handle.h"
#include "restsdk_parse_json_fom_string.h"

namespace stonks::restsdk {
WebSocketClient::WebSocketClient(cpp::NnSp<log::ILogger> logger)
    : web_socket_client_{cpp::MakeNnUp<
          web::websockets::client::websocket_callback_client>()},
      logger_{std::move(logger)} {}

void WebSocketClient::Connect(network::Uri uri,
                              cpp::NnSp<network::IWebSocketHandler> handler) {
  web_socket_client_->set_message_handler(
      [handler = std::move(handler)](
          const web::websockets::client::websocket_incoming_message& message) {
        if (const auto non_text_message =
                message.message_type() !=
                web::websockets::client::websocket_message_type::text_message) {
          return;
        }

        const auto message_text = message.extract_string().get();
        handler->HandleMessage(ParseJsonFromString(message_text));
      });

  web_socket_client_->connect(uri.value).wait();

  logger_->LogImportantEvent(
      fmt::format("Connected to web socket: {}", uri.value));
}

void WebSocketClient::SendMessage(network::Message message) const {
  auto web_socket_message =
      web::websockets::client::websocket_outgoing_message{};
  web_socket_message.set_utf8_message(message->GetNativeHandle()->serialize());

  web_socket_client_->send(std::move(web_socket_message)).wait();
}
}  // namespace stonks::restsdk