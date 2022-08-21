#include "network_request_exception_handler.h"

#include <memory>
#include <utility>

#include "network_types.h"

namespace stonks::network {
RequestExceptionHandler::RequestExceptionHandler(
    ccutils::NnUp<IRestRequestHandler> handler)
    : handler_{std::move(handler)} {}

auto RequestExceptionHandler::HandleRequestAndGiveResponse(
    RestRequest request) const -> RestResponse {
  return handler_->HandleRequestAndGiveResponse(std::move(request));
}
}  // namespace stonks::network