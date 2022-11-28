#include "network_typed_ws_client.h"

#include <bits/exception.h>

#include <cppcoro/single_consumer_event.hpp>
#include <cppcoro/task.hpp>
#include <function2/function2.hpp>
#include <gsl/assert>
#include <memory>
#include <not_null.hpp>
#include <utility>

#include "network_typed_endpoint.h"
#include "network_ws_types.h"

namespace stonks::network {
TypedWsClient::TypedWsClient(WsEndpointTypes endpoint_types,
                             cpp::NnUp<IWsClient> ws_client)
    : endpoint_types_{std::move(endpoint_types)},
      ws_client_{std::move(ws_client)} {}

void TypedWsClient::Connect(WsEndpoint endpoint) {
  ws_client_->Connect(std::move(endpoint));
}

auto TypedWsClient::ReceiveMessage() -> cppcoro::task<WsMessage> {
  auto message = WsMessage{};
  auto valid_message_received = false;

  while (!valid_message_received) {
    message = co_await ws_client_->ReceiveMessage();

    try {
      endpoint_types_.received_message(*message);
      valid_message_received = true;
    } catch (const std::exception &) {
    }
  }

  co_return message;
}

auto TypedWsClient::SendMessage(WsMessage message) const -> cppcoro::task<> {
  Expects(!endpoint_types_.sent_message.empty());

  try {
    endpoint_types_.sent_message(*message);
  } catch (const std::exception &) {
    Expects(false);
  }

  co_await ws_client_->SendMessage(std::move(message));
}
}  // namespace stonks::network