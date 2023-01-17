#include "network_rest_server.h"

#include <utility>

namespace stonks::network {
RestServer::RestServer(cpp::meta::PrivateTo<RestServerBuilder> /*unused*/,
                       cpp::NnUp<IRestRequestReceiver> request_receiver)
    : request_receiver_{std::move(request_receiver)} {}
}  // namespace stonks::network