#include "network_ts_rest_request_sender.h"

#include <mutex>

namespace stonks::network::ts {
RestRequestSender::RestRequestSender(
    cpp::NnSp<IRestRequestSender> request_sender)
    : request_sender_{std::move(request_sender)} {}

auto RestRequestSender::SendRequestAndGetResponse(RestRequest request)
    -> cppcoro::task<RestResponse> {
  const auto lock = std::lock_guard{mutex_};
  co_return co_await request_sender_->SendRequestAndGetResponse(
      std::move(request));
}
}  // namespace stonks::network::ts