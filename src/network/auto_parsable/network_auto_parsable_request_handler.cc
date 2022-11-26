#include "network_auto_parsable_request_handler.h"

namespace stonks::network {
auto AutoParsableRequestHandler::HandleRequestAndGiveResponse(
    RestRequest request) const -> cppcoro::task<RestResponse> {
  return handler_(request);
}
}  // namespace stonks::network