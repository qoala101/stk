#include "ngrok_client.h"

namespace stonks::ngrok {
NgrokClient::NgrokClient() : client_{"http://localhost:4040/api"} {}

[[nodiscard]] auto NgrokClient::GetTunnel() const -> Tunnel {
  return std::any_cast<Tunnel>(client_.Execute(
      network::EndpointDesc{.method = web::http::methods::GET,
                            .relative_uri = "/tunnels",
                            .response_body = json::Type<Tunnel>{}}));
}
}  // namespace stonks::ngrok