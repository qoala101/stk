#include "web_socket.h"

#include <bits/exception.h>
#include <cpprest/base_uri.h>
#include <cpprest/ws_msg.h>
#include <cxxabi.h>
#include <fmt/format.h>
#include <pplx/pplxtasks.h>
#include <spdlog/spdlog.h>

#include <gsl/assert>
#include <string>
#include <system_error>
#include <utility>

namespace stonks::network {
WebSocket::WebSocket(std::string_view uri) : uri_builder_{std::string{uri}} {}

WebSocket::~WebSocket() {
  shared_future_.wait();
  Disconnect();
}

void WebSocket::AppendUri(std::string_view uri) {
  uri_builder_.append(std::string{uri});
}

void WebSocket::Connect() {
  const auto uri = uri_builder_.to_uri();
  spdlog::info("Connecting web socket to {}", uri.to_string());
  websocket_client_.connect(uri_builder_.to_uri()).wait();
  spdlog::info("Connected");
}

void WebSocket::Disconnect() {
  websocket_client_.close().wait();
  spdlog::info("Disconnected");
}

void WebSocket::SendMessage(std::string_view message) {
  auto web_socket_message =
      web::websockets::client::websocket_outgoing_message{};
  web_socket_message.set_utf8_message(std::string{message});
  websocket_client_.send(web_socket_message).wait();
  spdlog::info("Sent message: {}", message);
}

auto WebSocket::ReceiveMessages(MessageHandler message_handler)
    -> std::shared_future<void> {
  Expects(message_handler);

  if (shared_future_.valid()) {
    shared_future_.wait();
  }

  shared_future_ = std::async(
      std::launch::async, [&websocket_client = websocket_client_,
                           message_handler = std::move(message_handler)]() {
        while (true) {
          const auto message = websocket_client.receive().get();
          const auto message_string = message.extract_string().get();
          const auto json = web::json::value::parse(message_string);

          if (json.is_null()) {
            spdlog::error("Cannot parse JSON from string: {}", message_string);
          }

          const auto message_result = message_handler(json);

          if (message_result == ReceiveMessagesResult::kStop) {
            return;
          }
        }
      });

  return shared_future_;
}
}  // namespace stonks::network