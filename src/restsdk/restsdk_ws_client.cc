#include "restsdk_ws_client.h"

#include <cpprest/base_uri.h>
#include <cpprest/json.h>  // IWYU pragma: keep
#include <cpprest/ws_client.h>
#include <cpprest/ws_msg.h>
#include <fmt/core.h>
#include <pplx/pplx.h>
#include <pplx/pplxtasks.h>

#include <coroutine>
#include <cppcoro/single_consumer_event.hpp>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/task.hpp>
#include <exception>
#include <not_null.hpp>
#include <string>
#include <utility>

#include "cpp_not_null.h"
#include "cpp_typed_struct.h"
#include "network_i_json.h"
#include "network_types.h"
#include "restsdk_json_native_handle.h"
#include "restsdk_parse_json_fom_string.h"

namespace stonks::restsdk {
namespace {
auto HandleWsMessage(
    const network::IWsMessageHandler &handler,
    const web::websockets::client::websocket_incoming_message &native_message)
    -> cppcoro::task<> {
  if (const auto non_text_message =
          native_message.message_type() !=
          web::websockets::client::websocket_message_type::text_message) {
    co_return;
  }

  auto parsed_message = network::WsMessage{};
  auto message_parsed = cppcoro::single_consumer_event{};

  native_message.extract_string().then(
      [&parsed_message, &message_parsed](const std::string &message_text) {
        parsed_message = ParseJsonFromString(message_text);
        message_parsed.set();
      });

  co_await message_parsed;
  co_await handler.HandleMessage(std::move(parsed_message));
}
}  // namespace

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
      fmt::format("Disconnecting from web socket {}...", uri));

  try {
    native_ws_client_->close().wait();
  } catch (...) {
  }

  logger_->LogImportantEvent(
      fmt::format("Disconnected from web socket {}", uri));
}

void WsClient::Connect(network::WsEndpoint endpoint) {
  logger_->LogImportantEvent(
      fmt::format("Connecting to web socket {}...", **endpoint));

  try {
    native_ws_client_->connect(**endpoint).wait();
  } catch (const std::exception &e) {
    logger_->LogErrorCondition(fmt::format(
        "Failed to connect to web socket {}: {}", **endpoint, e.what()));
    throw;
  }

  logger_->LogImportantEvent(
      fmt::format("Connected to web socket {}", **endpoint));
}

void WsClient::SetMessageHandler(
    cpp::NnUp<network::IWsMessageHandler> handler) {
  native_ws_client_->set_message_handler(
      [handler = cpp::NnSp<network::IWsMessageHandler>{std::move(handler)}](
          const auto &message) {
        cppcoro::sync_wait(HandleWsMessage(*handler, message));
      });
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
}  // namespace stonks::restsdk