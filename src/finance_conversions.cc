#include "finance_conversions.h"

#include "utils.h"

namespace stonks::finance {
Candle ParseCandleFromBinanceKline(const binance::Kline &kline) {
  return Candle{.open_time = kline.open_time,
                .close_time = kline.close_time,
                .data = Candle::Data{.open_price = kline.open_price,
                                     .high_price = kline.high_price,
                                     .low_price = kline.low_price,
                                     .close_price = kline.close_price,
                                     .volume = kline.volume}};
}

OrderInfo ParseOrderInfoFromBinanceOrderInfo(
    const binance::OrderInfo &order_info) {
  return OrderInfo{
      .uuid = utils::ParseUuidFromString(order_info.client_order_id),
      .status = order_info.status};
}
}  // namespace stonks::finance