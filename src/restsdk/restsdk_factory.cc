#include "restsdk_factory.h"

#include <memory>
#include <utility>

#include "ccutils_not_null.h"
#include "network_i_json.h"
#include "restsdk_json.h"
#include "restsdk_rest_request_receiver.h"
#include "restsdk_rest_request_sender.h"

namespace stonks::restsdk {
auto Factory::CreateRestRequestSender() const
    -> ccutils::NnUp<network::IRestRequestSender> {
  return ccutils::MakeNnUp<RestRequestSender>();
}

auto Factory::CreateRestRequestReceiver(
    std::string local_uri,
    ccutils::NnUp<network::IRestRequestHandler> handler) const
    -> ccutils::NnUp<network::IRestRequestReceiver> {
  return ccutils::MakeNnUp<RestRequestReceiver>(local_uri, std::move(handler));
}

auto Factory::CreateJson() const -> ccutils::Pv<network::IJson> {
  return ccutils::MakePv<network::IJson, Json>();
}
}  // namespace stonks::restsdk