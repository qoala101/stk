#include "finance_api.h"

#include <spdlog/spdlog.h>

#include <range/v3/to_container.hpp>
#include <range/v3/view/transform.hpp>

#include "binance_api.h"
#include "finance_conversions.h"

namespace stonks {
std::optional<std::vector<Candlestick>> GetCandlesticks(
    std::string_view symbol) {
  const auto klines =
      binance::GetKlines(symbol, binance::CandlestickInterval::k1Day,
                         std::nullopt, std::nullopt, 10);

  if (!klines.has_value()) {
    spdlog::error("Cannot get candlesticks");
    return std::nullopt;
  }

  return *klines | ranges::views::transform(ParseCandlestickFromBinanceKline) |
         ranges::to_vector;
}
}  // namespace stonks