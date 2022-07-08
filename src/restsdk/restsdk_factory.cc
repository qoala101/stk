#include "restsdk_factory.h"

#include <memory>

#include "not_null.hpp"
#include "restsdk_rest_request_sender.h"

namespace stonks::restsdk {
auto Factory::CreateRestRequestSender(network::Endpoint endpoint) const
    -> cpp::not_null<std::unique_ptr<network::IRestRequestSender>> {
  return cpp::assume_not_null(std::make_unique<RestRequestSender>(endpoint));
}

auto Factory::CreateRestRequestHandler(std::string_view local_uri) const
    -> cpp::not_null<std::unique_ptr<network::IRestRequestHandler>> {
  // return std::make_unique<RestRequestHandler>(local_uri);
}
}  // namespace stonks::restsdk