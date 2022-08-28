#include "network_rest_server.h"

#include <gsl/assert>
#include <memory>

#include "cpp_not_null.h"
#include "cpp_smart_pointers.h"
#include "network_endpoint_request_dispatcher.h"
#include "network_request_exception_handler.h"
#include "network_typed_endpoint.h"
#include "network_typed_endpoint_handler.h"
#include "network_types.h"
#include "not_null.hpp"

namespace stonks::network {
RestServer::RestServer(cpp::NnUp<IFactory> network_factory)
    : network_factory_{std::move(network_factory)} {}

auto RestServer::On(std::string base_uri) -> RestServer& {
  Expects(!base_uri_.has_value());
  base_uri_.emplace(std::move(base_uri));
  Ensures(base_uri_.has_value());
  return *this;
}

auto RestServer::Start() && -> cpp::Nn<cpp::Up<IRestRequestReceiver>> {
  Expects(base_uri_.has_value());
  Expects(!endpoint_handlers_.empty());
  auto result = network_factory_->CreateRestRequestReceiver(
      std::move(*base_uri_),
      cpp::MakeNnUp<EndpointRequestDispatcher>(std::move(endpoint_handlers_)));
  base_uri_.reset();
  endpoint_handlers_.clear();
  Ensures(!base_uri_.has_value());
  Ensures(endpoint_handlers_.empty());
  return result;
}

auto RestServer::Handling(TypedEndpoint endpoint,
                          cpp::NnUp<IRestRequestHandler> handler)
    -> RestServer& {
  auto decorated_handler = cpp::MakeNnUp<RequestExceptionHandler>(
      cpp::MakeNnUp<TypedEndpointHandler>(std::move(endpoint.expected_types),
                                          std::move(handler)));

  endpoint_handlers_.emplace(std::move(endpoint.endpoint),
                             std::move(decorated_handler));

  Ensures(!endpoint_handlers_.empty());
  return *this;
}
}  // namespace stonks::network