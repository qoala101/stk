#include "network_endpoint_request_dispatcher.h"

#include <optional>
#include <utility>

#include "network_enums.h"
#include "network_types.h"

namespace stonks::network {
EndpointRequestDispatcher::EndpointRequestDispatcher(
    std::map<Endpoint, cpp::not_null<std::unique_ptr<IRestRequestHandler>>>
        endpoint_handlers)
    : endpoint_handlers_{std::move(endpoint_handlers)} {}

auto EndpointRequestDispatcher::HandleRequestAndGiveResponse(
    RestRequest request) const -> RestResponse {
  const auto endpoint_handler = endpoint_handlers_.find(request.endpoint);

  if (endpoint_handler == endpoint_handlers_.end()) {
    return {Status::kNotFound, std::nullopt};
  }

  return endpoint_handler->second->HandleRequestAndGiveResponse(
      std::move(request));
}
}  // namespace stonks::network