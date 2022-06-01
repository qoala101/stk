#ifndef STONKS_CLIENTS_CLIENT_BINANCE_H_
#define STONKS_CLIENTS_CLIENT_BINANCE_H_

#include "client.h"

namespace stonks {
class BinanceClient {
 public:
  explicit BinanceClient();

  [[nodiscard]] auto GetExchangeInfo() const -> binance::ExchangeInfo;

 private:
  network::Client client_;
};
}  // namespace stonks

#endif  // STONKS_CLIENTS_CLIENT_BINANCE_H_
