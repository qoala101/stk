#include "network_typed_endpoint_handler.h"

#include <utility>

#include "network_typed_endpoint.h"

namespace stonks::network {
TypedEndpointHandler::TypedEndpointHandler(EndpointTypes endpoint_types,
                                           RestRequestHandler handler)
    : endpoint_types_{std::move(endpoint_types)},
      handler_{std::move(handler)} {}

auto TypedEndpointHandler::operator()(RestRequest request) const
    -> RestResponse {
  return handler_(std::move(request));
}
}  // namespace stonks::network