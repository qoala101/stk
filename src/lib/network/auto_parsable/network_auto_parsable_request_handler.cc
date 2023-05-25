/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "network_auto_parsable_request_handler.h"

#include <coroutine>

namespace vh::network {
auto AutoParsableRequestHandler::HandleRequestAndGiveResponse(
    RestRequest request) -> cppcoro::task<RestResponse> {
  co_return co_await handler_(request);
}
}  // namespace vh::network