#include "network_typed_web_socket_client.h"

#include <bits/exception.h>

#include <function2/function2.hpp>
#include <gsl/assert>
#include <memory>
#include <utility>

#include "network_typed_endpoint.h"
#include "network_typed_web_socket_handler.h"
#include "not_null.hpp"

namespace stonks::network {
TypedWebSocketClient::TypedWebSocketClient(
    WsEndpointTypes endpoint_types,
    cpp::NnUp<IWebSocketClient> web_socket_client)
    : endpoint_types_{std::move(endpoint_types)},
      web_socket_client_{std::move(web_socket_client)} {}

void TypedWebSocketClient::Connect(WsEndpoint endpoint) {
  web_socket_client_->Connect(std::move(endpoint));
}

void TypedWebSocketClient::SetMessageHandler(
    cpp::NnUp<IWebSocketHandler> handler) {
  Expects(!endpoint_types_.received_message.empty());
  web_socket_client_->SetMessageHandler(cpp::MakeNnUp<TypedWebSocketHandler>(
      std::move(endpoint_types_.received_message), std::move(handler)));
  Ensures(endpoint_types_.received_message.empty());
}

void TypedWebSocketClient::SendMessage(WsMessage message) const {
  Expects(!endpoint_types_.sent_message.empty());

  try {
    endpoint_types_.sent_message(*message);
  } catch (const std::exception &) {
    Expects(false);
  }

  web_socket_client_->SendMessage(std::move(message));
}
}  // namespace stonks::network