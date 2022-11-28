#include "restsdk_ws_client.h"

#include <cpprest/base_uri.h>
#include <cpprest/json.h>  // IWYU pragma: keep
#include <cpprest/ws_client.h>
#include <cpprest/ws_msg.h>
#include <fmt/core.h>
#include <pplx/pplxtasks.h>

#include <cppcoro/single_consumer_event.hpp>
#include <memory>
#include <not_null.hpp>
#include <utility>

#include "cpp_not_null.h"
#include "cpp_typed_struct.h"
#include "network_i_json.h"
#include "network_types.h"
#include "restsdk_json_native_handle.h"
#include "restsdk_parse_json_fom_string.h"

namespace stonks::restsdk {
WsClient::WsClient(cpp::NnUp<log::ILogger> logger)
    : native_ws_client_{cpp::MakeNnUp<
          web::websockets::client::websocket_client>()},
      logger_{std::move(logger)} {}

WsClient::WsClient(WsClient &&) noexcept = default;

auto WsClient::operator=(WsClient &&) noexcept -> WsClient & = default;

WsClient::~WsClient() {
  if (const auto object_was_moved = native_ws_client_ == nullptr) {
    return;
  }

  const auto uri = native_ws_client_->uri().to_string();
  logger_->LogImportantEvent(
      fmt::format("Disconnecting from web socket: {}...", uri));

  native_ws_client_->close().wait();

  logger_->LogImportantEvent(
      fmt::format("Disconnected from web socket: {}", uri));
}

void WsClient::Connect(network::WsEndpoint endpoint) {
  logger_->LogImportantEvent(
      fmt::format("Connecting to web socket: {}...", **endpoint));

  native_ws_client_->connect(**endpoint).wait();

  logger_->LogImportantEvent(
      fmt::format("Connected to web socket: {}", **endpoint));
}

auto WsClient::SendMessage(network::WsMessage message) const
    -> cppcoro::task<> {
  auto native_ws_message =
      web::websockets::client::websocket_outgoing_message{};
  native_ws_message.set_utf8_message(message->GetNativeHandle()->serialize());

  auto message_sent = cppcoro::single_consumer_event{};

  native_ws_client_->send(std::move(native_ws_message)).then([&message_sent]() {
    message_sent.set();
  });

  co_await message_sent;
}

auto WsClient::ReceiveMessage() -> cppcoro::task<network::WsMessage> {
  auto message = network::WsMessage{};
  auto message_received = cppcoro::single_consumer_event{};

  native_ws_client_->receive()
      .then([](const web::websockets::client::websocket_incoming_message
                   &native_message) {
        if (const auto non_text_message =
                native_message.message_type() !=
                web::websockets::client::websocket_message_type::text_message) {
          return pplx::task<std::string>{};
        }

        return native_message.extract_string();
      })
      .then([&message, &message_received](const std::string &message_text) {
        if (message_text.empty()) {
          return;
        }

        message = ParseJsonFromString(message_text);
        message_received.set();
      });

  co_await message_received;
  co_return message;
}
}  // namespace stonks::restsdk