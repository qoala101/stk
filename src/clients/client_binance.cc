#include "client_binance.h"

#include "binance_settings.h"
#include "binance_types.h"
#include "endpoints_binance.h"

namespace stonks::client {
Binance::Binance()
    : client_{network::ExternalUri{binance::settings::GetBaseRestUri()}} {}

auto Binance::GetExchangeInfo() const -> binance::ExchangeInfo {
  return client_.Execute(endpoints::Binance::GetExchangeInfo())
      .Get<binance::ExchangeInfo>();
}
}  // namespace stonks::client