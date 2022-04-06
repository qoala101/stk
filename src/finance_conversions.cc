#include "finance_conversions.h"

namespace stonks::finance {
Candlestick ParseCandlestickFromBinanceKline(const binance::Kline &kline) {
  return Candlestick{.open_time = kline.open_time,
                     .open_price = kline.open_price,
                     .high_price = kline.high_price,
                     .low_price = kline.low_price,
                     .close_price = kline.close_price,
                     .close_time = kline.close_time,
                     .volume = kline.volume};
};
}  // namespace stonks