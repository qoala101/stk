#ifndef STONKS_NGROK_NGROK_CLIENT_H_
#define STONKS_NGROK_NGROK_CLIENT_H_

#include "client.h"
#include "ngrok_types.h"

/**
 * @brief Client for ngrok running on localhost.
 */
namespace stonks::ngrok {
class NgrokClient {
 public:
  explicit NgrokClient();

  [[nodiscard]] auto GetTunnel() const -> Tunnel;

 private:
  network::Client client_;
};
}  // namespace stonks::ngrok

#endif  // STONKS_NGROK_NGROK_CLIENT_H_
