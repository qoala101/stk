#include "network_rest_server.h"

#include <gsl/assert>

#include "network_endpoint_request_dispatcher.h"
#include "network_typed_endpoint.h"
#include "network_typed_endpoint_handler.h"
#include "network_types.h"

namespace stonks::network {
RestServer::RestServer(cpp::not_null<std::unique_ptr<IFactory>> network_factory)
    : network_factory_{std::move(network_factory)} {}

auto RestServer::On(std::string base_uri) -> RestServer& {
  Expects(!base_uri_.has_value());
  base_uri_.emplace(std::move(base_uri));
  Ensures(base_uri_.has_value());
  return *this;
}

auto RestServer::Start()
    const& -> cpp::not_null<std::unique_ptr<IRestRequestReceiver>> {
  Expects(base_uri_.has_value());
  Expects(!endpoint_handlers_.empty());
  return network_factory_->CreateRestRequestReceiver(
      *base_uri_, EndpointRequestDispatcher{endpoint_handlers_});
}

auto RestServer::Start() && -> cpp::not_null<
    std::unique_ptr<IRestRequestReceiver>> {
  Expects(base_uri_.has_value());
  Expects(!endpoint_handlers_.empty());
  auto result = network_factory_->CreateRestRequestReceiver(
      std::move(*base_uri_),
      EndpointRequestDispatcher{std::move(endpoint_handlers_)});
  base_uri_.reset();
  endpoint_handlers_.clear();
  Ensures(!base_uri_.has_value());
  Ensures(endpoint_handlers_.empty());
  return result;
}

auto RestServer::Handling(TypedEndpoint endpoint,
                          AutoParsableRequestHandler handler) -> RestServer& {
  endpoint_handlers_.emplace(
      std::move(endpoint.endpoint),
      TypedEndpointHandler{std::move(endpoint.expected_types),
                           std::move(handler)});
  Ensures(!endpoint_handlers_.empty());
  return *this;
}
}  // namespace stonks::network