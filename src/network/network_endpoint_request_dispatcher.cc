#include "network_endpoint_request_dispatcher.h"

#include <polymorphic_value.h>

#include <functional>
#include <gsl/assert>
#include <optional>

#include "network_enums.h"

namespace stonks::network {
EndpointRequestDispatcher::EndpointRequestDispatcher(
    std::map<Endpoint, EndpointRequestHandler> endpoint_handlers)
    : endpoint_handlers_{std::move(endpoint_handlers)} {}

auto EndpointRequestDispatcher::operator()(const Endpoint &endpoint,
                                           RestRequestData data) const
    -> std::pair<Status, Result> {
  const auto endpoint_handler = endpoint_handlers_.find(endpoint);

  if (endpoint_handler == endpoint_handlers_.end()) {
    return {Status::kNotFound, std::nullopt};
  }

  const auto &handler = endpoint_handler->second;
  Expects(handler);

  return {Status::kOk, handler(std::move(data.params), std::move(*data.body))};
}
}  // namespace stonks::network