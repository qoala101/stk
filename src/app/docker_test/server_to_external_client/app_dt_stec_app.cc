#include "app_dt_stec_app.h"

#include <utility>

#include "app_dt_stec_binance_types.h"
#include "cpp_chrono.h"

namespace stonks::app::dt::stec {
App::App(binance::Client binance_client, PdsAppClient pds_app_client)
    : binance_client_{std::move(binance_client)},
      pds_app_client_{std::move(pds_app_client)} {}

auto App::GetAveragePrice(Symbol symbol) const -> Price {
  auto price = Price{binance_client_.GetCurrentAveragePrice(symbol).price};

  pds_app_client_.RecordSymbolPrice({.time = cpp::GetCurrentTime(),
                                     .symbol = std::move(symbol),
                                     .price = price});

  return price;
}
}  // namespace stonks::app::dt::stec