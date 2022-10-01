#include "network_ws_client_builder.h"

#include <gsl/assert>
#include <memory>
#include <type_traits>

#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"
#include "network_typed_endpoint.h"
#include "network_typed_ws_client.h"
#include "network_ws_sender.h"
#include "not_null.hpp"

namespace stonks::network {
WsClientBuilder::WsClientBuilder(
    cpp::NnUp<IWsClient> ws_client)
    : ws_client_{std::move(ws_client).as_nullable()} {
  Ensures(ws_client_ != nullptr);
}

auto WsClientBuilder::On(TypedWsEndpoint endpoint)
    -> WsClientBuilder& {
  Expects(!endpoint_.has_value());
  endpoint_ = std::move(endpoint);
  Ensures(endpoint_.has_value());
  return *this;
}

auto WsClientBuilder::Connect() -> WsSender {
  Expects(ws_client_ != nullptr);
  Expects(endpoint_.has_value());

  auto typed_client = cpp::MakeNnUp<TypedWsClient>(
      std::move(endpoint_->expected_types),
      cpp::AssumeNn(std::move(ws_client_)));

  if (handler_ != nullptr) {
    typed_client->SetMessageHandler(cpp::AssumeNn(std::move(handler_)));
  }

  typed_client->Connect(std::move(endpoint_->endpoint));

  ws_client_.reset();
  endpoint_.reset();
  handler_.reset();

  Ensures(ws_client_ == nullptr);
  Ensures(!endpoint_.has_value());
  Ensures(handler_ == nullptr);

  return cpp::CallExposedPrivateConstructorOf<WsSender,
                                              WsClientBuilder>{}(
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