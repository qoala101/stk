#include "network_endpoint_request_dispatcher.h"

#include <memory>
#include <utility>

#include "network_enums.h"
#include "network_types.h"
#include "not_null.hpp"

namespace stonks::network {
EndpointRequestDispatcher::EndpointRequestDispatcher(
    std::map<Endpoint, cpp::NnSp<IRestRequestHandler>> endpoint_handlers)
    : endpoint_handlers_{std::move(endpoint_handlers)} {}

auto EndpointRequestDispatcher::HandleRequestAndGiveResponse(
    RestRequest request) const -> RestResponse {
  const auto endpoint_handler = endpoint_handlers_.find(request.endpoint);

  if (endpoint_handler == endpoint_handlers_.end()) {
    return {.status = Status::kNotFound};
  }

  return endpoint_handler->second->HandleRequestAndGiveResponse(
      std::move(request));
}
}  // namespace stonks::network