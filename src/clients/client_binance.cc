#include "client_binance.h"

#include <string>

#include "binance_settings.h"
#include "binance_types.h"
#include "endpoint.h"
#include "endpoints_binance.h"
#include "uri.h"

namespace stonks::client {
Binance::Binance()
    : client_{network::ExternalUri{binance::settings::GetBaseRestUri()}} {}

auto Binance::GetExchangeInfo() const -> binance::ExchangeInfo {
  return client_.Execute(endpoints::Binance::GetExchangeInfo())
      .Take<binance::ExchangeInfo>();
}
}  // namespace stonks::client