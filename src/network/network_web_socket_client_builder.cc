#include "network_web_socket_client_builder.h"

#include <gsl/assert>
#include <memory>
#include <type_traits>

#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"
#include "network_typed_endpoint.h"
#include "network_typed_web_socket_client.h"
#include "network_web_socket_sender.h"
#include "not_null.hpp"

namespace stonks::network {
WebSocketClientBuilder::WebSocketClientBuilder(
    cpp::NnUp<IWebSocketClient> web_socket_client)
    : web_socket_client_{std::move(web_socket_client).as_nullable()} {
  Ensures(web_socket_client_ != nullptr);
}

auto WebSocketClientBuilder::On(TypedWsEndpoint endpoint)
    -> WebSocketClientBuilder& {
  Expects(!endpoint_.has_value());
  endpoint_ = std::move(endpoint);
  Ensures(endpoint_.has_value());
  return *this;
}

auto WebSocketClientBuilder::Connect() -> WebSocketSender {
  Expects(web_socket_client_ != nullptr);
  Expects(endpoint_.has_value());

  auto typed_client = cpp::MakeNnUp<TypedWebSocketClient>(
      std::move(endpoint_->expected_types),
      cpp::AssumeNn(std::move(web_socket_client_)));

  if (handler_ != nullptr) {
    typed_client->SetMessageHandler(cpp::AssumeNn(std::move(handler_)));
  }

  typed_client->Connect(std::move(endpoint_->endpoint));

  web_socket_client_.reset();
  endpoint_.reset();
  handler_.reset();

  Ensures(web_socket_client_ == nullptr);
  Ensures(!endpoint_.has_value());
  Ensures(handler_ == nullptr);

  return cpp::CallExposedPrivateConstructorOf<WebSocketSender,
                                              WebSocketClientBuilder>{}(
      std::move(typed_client));
}

auto WebSocketClientBuilder::Handling(cpp::NnUp<IWebSocketHandler> handler)
    -> WebSocketClientBuilder& {
  Expects(handler_ == nullptr);
  handler_ = std::move(handler).as_nullable();
  Ensures(handler_ != nullptr);
  return *this;
}
}  // namespace stonks::network