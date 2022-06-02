#include "client_binance.h"

#include "binance_settings.h"
#include "binance_types.h"
#include "endpoints_binance.h"

namespace stonks {
BinanceClient::BinanceClient()
    : client_{network::ExternalUri{binance::settings::GetBaseRestUri()}} {}

auto BinanceClient::GetExchangeInfo() const -> binance::ExchangeInfo {
  return client_.Execute(BinanceEndpoints::GetExchangeInfo())
      .Get<binance::ExchangeInfo>();
}
}  // namespace stonks