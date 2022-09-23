#include "network_rest_server_builder.h"

#include <gsl/assert>
#include <memory>

#include "cpp_not_null.h"
#include "network_endpoint_request_dispatcher.h"
#include "network_request_exception_handler.h"
#include "network_typed_endpoint.h"
#include "network_typed_endpoint_handler.h"
#include "network_types.h"
#include "not_null.hpp"

namespace stonks::network {
RestServerBuilder::RestServerBuilder(
    cpp::NnSp<di::IFactory<IRestRequestReceiver>> request_receiver_factory)
    : request_receiver_factory_{std::move(request_receiver_factory)} {}

auto RestServerBuilder::On(Uri base_uri) -> RestServerBuilder& {
  Expects(!base_uri_.has_value());
  base_uri_ = std::move(base_uri);
  Ensures(base_uri_.has_value());
  return *this;
}

auto RestServerBuilder::Start() -> cpp::NnUp<IRestRequestReceiver> {
  Expects(base_uri_.has_value());
  Expects(!endpoint_handlers_.empty());

  auto request_receiver = cpp::AssumeNn(request_receiver_factory_->create());
  request_receiver->Receive(
      std::move(*base_uri_),
      cpp::MakeNnSp<EndpointRequestDispatcher>(std::move(endpoint_handlers_)));

  base_uri_.reset();
  endpoint_handlers_.clear();

  Ensures(!base_uri_.has_value());
  Ensures(endpoint_handlers_.empty());
  return request_receiver;
}

auto RestServerBuilder::Handling(TypedEndpoint endpoint,
                                 cpp::NnSp<IRestRequestHandler> handler)
    -> RestServerBuilder& {
  auto decorated_handler = cpp::MakeNnSp<RequestExceptionHandler>(
      cpp::MakeNnSp<TypedEndpointHandler>(std::move(endpoint.expected_types),
                                          std::move(handler)));

  endpoint_handlers_.emplace(std::move(endpoint.endpoint),
                             std::move(decorated_handler));

  Ensures(!endpoint_handlers_.empty());
  return *this;
}
}  // namespace stonks::network