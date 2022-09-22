#include "network_auto_parsable_request_handler.h"

#include <gsl/assert>
#include <utility>

namespace stonks::network {
auto AutoParsableRequestHandler::HandleRequestAndGiveResponse(
    RestRequest request) const -> RestResponse {
  return handler_(request);
}
}  // namespace stonks::network