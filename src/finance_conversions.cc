#include "finance_conversions.h"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

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
  return OrderInfo{.uuid = boost::lexical_cast<boost::uuids::uuid>(
                       order_info.client_order_id),
                   .status = order_info.status};
}
}  // namespace stonks::finance