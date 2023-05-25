/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "network_request_exception_handler.h"

#include <coroutine>
#include <exception>
#include <not_null.hpp>
#include <utility>

#include "network_exception.h"
#include "network_json_common_conversions.h"
#include "network_types.h"

namespace vh::network {
RequestExceptionHandler::RequestExceptionHandler(
    cpp::NnUp<IRestRequestHandler> handler)
    : handler_{std::move(handler)} {}

auto RequestExceptionHandler::HandleRequestAndGiveResponse(RestRequest request)
    -> cppcoro::task<RestResponse> {
  try {
    co_return co_await handler_->HandleRequestAndGiveResponse(
        std::move(request));
  } catch (const Exception &e) {
    co_return RestResponse{.status = Status::kBadRequest,
                           .result = ConvertToJson(e)};
  } catch (const std::exception &e) {
    co_return RestResponse{.status = Status::kInternalError,
                           .result = ConvertToJson(e)};
  }
}
}  // namespace vh::network