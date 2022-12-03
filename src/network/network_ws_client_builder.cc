#include "network_ws_client_builder.h"

#include <gsl/assert>
#include <memory>
#include <not_null.hpp>

#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"
#include "network_typed_ws_client.h"
#include "network_typed_ws_endpoint.h"
#include "network_ws_connection.h"

namespace stonks::network {
WsClientBuilder::WsClientBuilder(TypedWsEndpoint endpoint,
                                 cpp::NnUp<IWsClient> ws_client)
    : endpoint_{std::move(endpoint)},
      ws_client_{std::move(ws_client).as_nullable()} {
  Ensures(endpoint_.has_value());
  Ensures(ws_client_ != nullptr);
}

auto WsClientBuilder::Connect() -> WsConnection {
  Expects(ws_client_ != nullptr);
  Expects(endpoint_.has_value());

  auto typed_client =
      cpp::MakeNnUp<TypedWsClient>(std::move(endpoint_->expected_types),
                                   cpp::AssumeNn(std::move(ws_client_)));

  if (handler_ != nullptr) {
    typed_client->SetMessageHandler(cpp::AssumeNn(std::move(handler_)));
  }

  typed_client->Connect(std::move(endpoint_->endpoint));
  endpoint_.reset();

  Ensures(ws_client_ == nullptr);
  Ensures(!endpoint_.has_value());
  Ensures(handler_ == nullptr);

  return cpp::CallExposedPrivateConstructorOf<WsConnection, WsClientBuilder>{}(
      std::move(typed_client));
}

auto WsClientBuilder::Handling(cpp::NnUp<IWsMessageHandler> handler)
    -> WsClientBuilder& {
  Expects(handler_ == nullptr);
  handler_ = std::move(handler).as_nullable();
  Ensures(handler_ != nullptr);
  return *this;
}
}  // namespace stonks::network