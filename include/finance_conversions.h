#ifndef STONKS_FINANCE_CONVERSIONS_H_
#define STONKS_FINANCE_CONVERSIONS_H_

#include <span>
#include <vector>

#include "binance_types.h"
#include "finance_types.h"

namespace stonks::finance {
Candle ParseCandleFromBinanceKline(const binance::Kline &kline);

OrderInfo ParseOrderInfoFromBinanceOrderInfo(
    const binance::OrderInfo &order_info);

OrderError ParseOrderErrorFromBinanceApiError(
    const binance::ApiError &api_error);

binance::OrderType ConvertOrderTypeToBinanceOrderType(const OrderType &order_type);
}  // namespace stonks::finance

#endif  // STONKS_FINANCE_CONVERSIONS_H_
