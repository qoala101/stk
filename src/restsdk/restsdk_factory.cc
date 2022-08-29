#include "restsdk_factory.h"

#include <utility>

#include "cpp_not_null.h"
#include "restsdk_rest_request_receiver.h"
#include "restsdk_rest_request_sender.h"

namespace stonks::restsdk {
auto Factory::CreateRestRequestSender() const
    -> cpp::NnUp<network::IRestRequestSender> {
  return cpp::MakeNnUp<RestRequestSender>();
}

auto Factory::CreateRestRequestReceiver(
    std::string local_uri,
    cpp::NnUp<network::IRestRequestHandler> handler) const
    -> cpp::NnUp<network::IRestRequestReceiver> {
  return cpp::MakeNnUp<RestRequestReceiver>(local_uri, std::move(handler));
}
}  // namespace stonks::restsdk