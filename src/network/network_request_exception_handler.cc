#include "network_request_exception_handler.h"

#include <bits/exception.h>

#include <memory>
#include <utility>

#include "cpp_polymorphic_value.h"
#include "network_enums.h"
#include "network_json_basic_conversions.h"
#include "network_types.h"

namespace stonks::network {
RequestExceptionHandler::RequestExceptionHandler(
    cpp::NnUp<IRestRequestHandler> handler)
    : handler_{std::move(handler)} {}

auto RequestExceptionHandler::HandleRequestAndGiveResponse(
    RestRequest request) const -> RestResponse {
  try {
    return handler_->HandleRequestAndGiveResponse(std::move(request));
  } catch (const std::exception &e) {
    return {.status = Status::kInternalError, .result = ConvertToJson(e)};
  }
}
}  // namespace stonks::network