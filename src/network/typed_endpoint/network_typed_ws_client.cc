#include "network_typed_ws_client.h"

#include <bits/exception.h>

#include <function2/function2.hpp>
#include <gsl/assert>
#include <memory>
#include <utility>

#include "cpp_optional.h"
#include "network_typed_ws_message_handler.h"
#include "not_null.hpp"

namespace stonks::network {
TypedWsClient::TypedWsClient(WsEndpointTypes endpoint_types,
                             cpp::NnUp<IWsClient> ws_client)
    : endpoint_types_{std::move(endpoint_types)},
      ws_client_{std::move(ws_client)} {}

void TypedWsClient::Connect(WsEndpoint endpoint) {
  ws_client_->Connect(std::move(endpoint));
}

void TypedWsClient::SetMessageHandler(cpp::NnUp<IWsMessageHandler> handler) {
  Expects(endpoint_types_.received_message.has_value());
  ws_client_->SetMessageHandler(cpp::MakeNnUp<TypedWsMessageHandler>(
      std::move(*endpoint_types_.received_message), std::move(handler)));

  endpoint_types_.received_message.reset();
  Ensures(!endpoint_types_.received_message.has_value());
}

void TypedWsClient::SendMessage(WsMessage message) const {
  Expects(endpoint_types_.sent_message.has_value());
  Expects(!endpoint_types_.sent_message->empty());

  try {
    (*endpoint_types_.sent_message)(*message);
  } catch (const std::exception &) {
    Expects(false);
  }

  ws_client_->SendMessage(std::move(message));
}
}  // namespace stonks::network