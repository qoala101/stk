#include "restsdk_factory.h"

#include <polymorphic_value.h>

#include <memory>

#include "network_json.h"
#include "not_null.hpp"
#include "restsdk_json.h"
#include "restsdk_rest_request_receiver.h"
#include "restsdk_rest_request_sender.h"

namespace stonks::restsdk {
auto Factory::CreateRestRequestSender(network::Endpoint endpoint) const
    -> cpp::not_null<std::unique_ptr<network::IRestRequestSender>> {
  return cpp::assume_not_null(std::make_unique<RestRequestSender>(endpoint));
}

auto Factory::CreateRestRequestReceiver(
    std::string_view local_uri,
    std::function<std::pair<network::Status, network::Result>(
        network::Endpoint, network::RestRequestData)>
        handler) const
    -> cpp::not_null<std::unique_ptr<network::IRestRequestReceiver>> {
  return cpp::assume_not_null(
      std::make_unique<RestRequestReceiver>(local_uri, std::move(handler)));
}

auto Factory::CreateJson() const
    -> isocpp_p0201::polymorphic_value<network::IJson> {
  return isocpp_p0201::make_polymorphic_value<network::IJson, Json>();
}
}  // namespace stonks::restsdk