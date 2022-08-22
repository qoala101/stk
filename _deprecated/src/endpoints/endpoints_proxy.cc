#include "endpoints_proxy.h"

#include <cpprest/http_msg.h>

#include <optional>
#include <string>




namespace stonks {
auto endpoints::Proxy::GetEndpointPort() -> network::EndpointDesc {
  return {.method = web::http::methods::GET,
          .relative_uri = "/GetEndpointPort"};
}

auto endpoints::Proxy::RegisterEndpoint() -> network::EndpointDesc {
  return {.method = web::http::methods::POST,
          .relative_uri = "/RegisterEndpoint"};
}
}  // namespace stonks