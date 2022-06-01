#include "endpoints_proxy.h"

namespace stonks {
auto ProxyEndpoints::GetEndpointPort() -> network::EndpointDesc {
  return {.method = web::http::methods::GET,
          .relative_uri = "/GetEndpointPort",
          .params = {{"endpoint", json::Type<std::string>{}}},
          .response_body = json::Type<std::optional<int>>{}};
}

auto ProxyEndpoints::RegisterEndpoint() -> network::EndpointDesc {
  return {.method = web::http::methods::POST,
          .relative_uri = "/RegisterEndpoint",
          .params = {{"endpoint", json::Type<std::string>{}}}};
}
}  // namespace stonks