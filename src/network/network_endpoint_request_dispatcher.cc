#include "network_endpoint_request_dispatcher.h"

#include <memory>
#include <not_null.hpp>
#include <utility>

#include "network_enums.h"
#include "network_types.h"

namespace stonks::network {
EndpointRequestDispatcher::EndpointRequestDispatcher(
    std::map<Endpoint, cpp::NnUp<IRestRequestHandler>> endpoint_handlers)
    : endpoint_handlers_{std::move(endpoint_handlers)} {}

auto EndpointRequestDispatcher::HandleRequestAndGiveResponse(
    RestRequest request) const -> cppcoro::task<RestResponse> {
  const auto endpoint_handler = endpoint_handlers_.find(request.endpoint);

  if (endpoint_handler == endpoint_handlers_.end()) {
    co_return RestResponse{.status = Status::kNotFound};
  }

  co_return co_await endpoint_handler->second->HandleRequestAndGiveResponse(
      std::move(request));
}
}  // namespace stonks::network