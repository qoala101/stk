#include "ngrok_client.h"

#include <cpprest/http_msg.h>

#include "cpp_not_null.h"
#include <string_view>

#include "endpoint.h"


#include "uri.h"

namespace stonks::ngrok {
NgrokClient::NgrokClient() : client_{network::LocalUri{4040, "/api"}} {}

[[nodiscard]] auto NgrokClient::GetTunnel() const -> Tunnel {
  return client_
      .Execute(network::EndpointDesc{.method = web::http::methods::GET,
                                     .relative_uri = "/tunnels"})
      .Parse<Tunnel>();
}
}  // namespace stonks::ngrok