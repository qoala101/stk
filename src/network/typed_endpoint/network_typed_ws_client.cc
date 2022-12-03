#include "network_typed_ws_client.h"

#include <coroutine>
#include <exception>
#include <function2/function2.hpp>
#include <gsl/assert>
#include <memory>
#include <not_null.hpp>
#include <utility>

#include "network_typed_endpoint.h"
#include "network_typed_ws_message_handler.h"

namespace stonks::network {
TypedWsClient::TypedWsClient(WsEndpointTypes endpoint_types,
                             cpp::NnUp<IWsClient> ws_client)
    : endpoint_types_{std::move(endpoint_types)},
      ws_client_{std::move(ws_client)} {}

void TypedWsClient::Connect(WsEndpoint endpoint) {
  ws_client_->Connect(std::move(endpoint));
}

void TypedWsClient::SetMessageHandler(cpp::NnUp<IWsMessageHandler> handler) {
  Expects(!endpoint_types_.received_message.empty());
  ws_client_->SetMessageHandler(cpp::MakeNnUp<TypedWsMessageHandler>(
      std::move(endpoint_types_.received_message), std::move(handler)));

  endpoint_types_.received_message = ParseTypeCheck{};
  Ensures(endpoint_types_.received_message.empty());
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