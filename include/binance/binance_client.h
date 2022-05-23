#ifndef STONKS_BINANCE_BINANCE_CLIENT_H_
#define STONKS_BINANCE_BINANCE_CLIENT_H_

#include "client.h"

namespace stonks::binance {
class BinanceClient {
 public:
  explicit BinanceClient();

  [[nodiscard]] auto GetExchangeInfo() -> ExchangeInfo;

 private:
  network::Client client_;
};
}  // namespace stonks::binance

#endif  // STONKS_BINANCE_BINANCE_CLIENT_H_
