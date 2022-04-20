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
  return OrderInfo{.order_status = order_info.status};
}

OrderError ParseOrderErrorFromBinanceApiError(
    const binance::ApiError &api_error) {
  return OrderError{.message = std::to_string(api_error.code) + ": " +
                               api_error.message};
}

binance::OrderType ConvertOrderTypeToBinanceOrderType(
    const OrderType &order_type) {
  return binance::OrderType::kInvalid;
}
}  // namespace stonks::finance