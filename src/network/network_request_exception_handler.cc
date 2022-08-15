#include "network_request_exception_handler.h"

#include <utility>

#include "network_types.h"

namespace stonks::network {
RequestExceptionHandler::RequestExceptionHandler(RestRequestHandler handler)
    : handler_{std::move(handler)} {}

auto RequestExceptionHandler::operator()(RestRequest request) const
    -> RestResponse {
  return handler_(std::move(request));
}
}  // namespace stonks::network