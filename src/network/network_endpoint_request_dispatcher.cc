#include "network_endpoint_request_dispatcher.h"

#include <functional>
#include <gsl/assert>

#include "network_enums.h"

namespace stonks::network {
EndpointRequestDispatcher::EndpointRequestDispatcher(
    std::map<Endpoint, EndpointRequestHandler> endpoint_handlers,
    EndpointRequestHandler not_found_handler)
    : endpoint_handlers_{std::move(endpoint_handlers)},
      not_found_handler_{[&not_found_handler]() {
        Expects(not_found_handler);
        return std::move(not_found_handler);
      }()} {
  Ensures(not_found_handler_);
}

auto EndpointRequestDispatcher::operator()(const Endpoint &endpoint,
                                           RestRequestData data) const
    -> std::pair<Status, Result> {
  const auto endpoint_handler = endpoint_handlers_.find(endpoint);

  if (endpoint_handler == endpoint_handlers_.end()) {
    return {Status::kNotFound,
            not_found_handler_(std::move(data.params), std::move(*data.body))};
  }

  const auto &handler = endpoint_handler->second;
  Expects(handler);

  return {Status::kOk, handler(std::move(data.params), std::move(*data.body))};
}
}  // namespace stonks::network