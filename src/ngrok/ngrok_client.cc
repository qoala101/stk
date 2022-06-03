#include "ngrok_client.h"

#include <cpprest/http_msg.h>

#include <memory>
#include <string_view>

#include "endpoint.h"
#include "type.h"
#include "type_variant.h"
#include "uri.h"

namespace stonks::ngrok {
NgrokClient::NgrokClient() : client_{network::LocalUri{4040, "/api"}} {}

[[nodiscard]] auto NgrokClient::GetTunnel() const -> Tunnel {
  return client_
      .Execute(network::EndpointDesc{.method = web::http::methods::GET,
                                     .relative_uri = "/tunnels",
                                     .response_body = json::Type<Tunnel>{}})
      .Take<Tunnel>();
}
}  // namespace stonks::ngrok