#include "network_response_exception_handler.h"

#include <memory>
#include <utility>

#include "network_types.h"

namespace stonks::network {

ResponseExceptionHandler::ResponseExceptionHandler(
    cpp::NnSp<IRestRequestSender> request_sender)
    : request_sender_{std::move(request_sender)} {}

auto ResponseExceptionHandler::SendRequestAndGetResponse(
    RestRequest request) const -> RestResponse {
  return request_sender_->SendRequestAndGetResponse(std::move(request));
}
}  // namespace stonks::network