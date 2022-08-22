#include "server_proxy.h"

#include <gsl/assert>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

#include "endpoints_proxy.h"
#include "uri.h"

namespace stonks::server {
Proxy::Proxy(cpp::Sp<network::Proxy> entity)
    : server_{network::LocalUri{kPort, kEndpoint},
              {GetEndpointPort(), RegisterEndpoint()}},
      entity_{std::move(entity)} {
  Expects(entity_ != nullptr);
}

auto Proxy::GetEndpointPort() -> network::Endpoint {
  return {endpoints::Proxy::GetEndpointPort(),
          network::HasResultTakesParams{[this](network::Params params) {
            return entity_->GetEndpointPort(
                params["endpoint"].Parse<std::string>());
          }}};
}

auto Proxy::RegisterEndpoint() -> network::Endpoint {
  return {endpoints::Proxy::RegisterEndpoint(),
          network::NoResultTakesParams{[this](network::Params params) {
            entity_->RegisterEndpoint(params["endpoint"].Parse<std::string>());
          }}};
}
}  // namespace stonks::server