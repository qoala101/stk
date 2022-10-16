#include "restsdk_ws_client.h"

#include <cpprest/base_uri.h>
#include <cpprest/json.h>
#include <cpprest/ws_client.h>
#include <cpprest/ws_msg.h>
#include <fmt/core.h>
#include <pplx/pplxtasks.h>

#include <functional>
#include <gsl/assert>
#include <memory>
#include <not_null.hpp>
#include <type_traits>
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
          web::websockets::client::websocket_callback_client>()},
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
      fmt::format("Connecting to web socket: {}...", endpoint.value.value));

  native_ws_client_->connect(endpoint.value.value).wait();

  logger_->LogImportantEvent(
      fmt::format("Connected to web socket: {}", endpoint.value.value));
}

void WsClient::SetMessageHandler(
    cpp::NnUp<network::IWsMessageHandler> handler) {
  handler_ = std::move(handler).as_nullable();

  native_ws_client_->set_message_handler(
      std::bind_front(&WsClient::HandleWsMessage, this));

  Ensures(handler_ != nullptr);
}

void WsClient::SendMessage(network::WsMessage message) const {
  auto native_ws_message =
      web::websockets::client::websocket_outgoing_message{};
  native_ws_message.set_utf8_message(message->GetNativeHandle()->serialize());

  native_ws_client_->send(std::move(native_ws_message)).wait();
}

void WsClient::HandleWsMessage(
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