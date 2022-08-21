#ifndef STONKS_ENDPOINTS_ENDPOINTS_PROXY_H_
#define STONKS_ENDPOINTS_ENDPOINTS_PROXY_H_

#include "endpoint.h"

namespace stonks::endpoints {
class Proxy {
 public:
  [[nodiscard]] static auto GetEndpointPort() -> network::EndpointDesc;
  [[nodiscard]] static auto RegisterEndpoint() -> network::EndpointDesc;
};
}  // namespace stonks::endpoints

#endif  // STONKS_ENDPOINTS_ENDPOINTS_PROXY_H_
