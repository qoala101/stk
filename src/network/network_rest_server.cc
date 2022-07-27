#include "network_rest_server.h"

#include <gsl/assert>

#include "network_endpoint_request_dispatcher.h"

namespace stonks::network {
RestServer::RestServer(cpp::not_null<std::unique_ptr<IFactory>> network_factory)
    : network_factory_{std::move(network_factory)} {}

auto RestServer::On(std::string base_uri) -> RestServer& {
  Expects(!base_uri_.has_value());
  base_uri_.emplace(std::move(base_uri));
  Ensures(base_uri_.has_value());
  return *this;
}

auto RestServer::Handling(Endpoint endpoint, AutoParsableRequestHandler handler)
    -> RestServer& {
  endpoint_handlers_.emplace(std::move(endpoint), std::move(handler));
  Ensures(!endpoint_handlers_.empty());
  return *this;
}

auto RestServer::Start()
    -> cpp::not_null<std::unique_ptr<IRestRequestReceiver>> {
  Expects(base_uri_.has_value());
  Expects(!endpoint_handlers_.empty());
  return network_factory_->CreateRestRequestReceiver(
      std::move(*base_uri_),
      EndpointRequestDispatcher{std::move(endpoint_handlers_)});
}
}  // namespace stonks::network