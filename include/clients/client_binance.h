#ifndef STONKS_CLIENTS_CLIENT_BINANCE_H_
#define STONKS_CLIENTS_CLIENT_BINANCE_H_

#include "binance_types.h"
#include "client.h"

namespace stonks::client {
class Binance {
 public:
  explicit Binance();

  [[nodiscard]] auto GetExchangeInfo() const -> binance::ExchangeInfo;

 private:
  network::Client client_;
};
}  // namespace stonks::client

#endif  // STONKS_CLIENTS_CLIENT_BINANCE_H_
