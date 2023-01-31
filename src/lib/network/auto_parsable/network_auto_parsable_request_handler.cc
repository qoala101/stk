#include "network_auto_parsable_request_handler.h"

#include <coroutine>

namespace vh::network {
auto AutoParsableRequestHandler::HandleRequestAndGiveResponse(
    RestRequest request) -> cppcoro::task<RestResponse> {
  co_return co_await handler_(request);
}
}  // namespace vh::network