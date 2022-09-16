#include "app_dt_stec_app.h"

#include <utility>

#include "app_dt_stec_binance_types.h"

namespace stonks::app::dt::stec {
App::App(binance::Client binance_client)
    : binance_client_{std::move(binance_client)} {}

auto App::GetAveragePrice(const Symbol &symbol) const -> Price {
  return {binance_client_.GetCurrentAveragePrice(symbol).price};
}
}  // namespace stonks::app::dt::stec