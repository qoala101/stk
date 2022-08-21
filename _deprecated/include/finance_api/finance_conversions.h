#ifndef STONKS_FINANCE_API_FINANCE_CONVERSIONS_H_
#define STONKS_FINANCE_API_FINANCE_CONVERSIONS_H_

#include <string_view>

#include "binance_enums.h"
#include "binance_types.h"
#include "finance_types.h"

namespace stonks::finance {
[[deprecated]] auto ParseSymbolFromBinanceSymbol(std::string_view symbol)
    -> Symbol;

Candle ParseCandleFromBinanceKline(const binance::Kline &kline);

auto ParseSymbolPriceTickFromBinanceSymbolBookTicker(
    const binance::SymbolBookTicker &symbol_book_ticker) -> SymbolPriceTick;

OrderInfo ParseOrderInfoFromBinanceOrderInfo(
    const binance::OrderInfo &order_info);

OrderError ParseOrderErrorFromBinanceApiError(
    const binance::ApiError &api_error);

binance::OrderType ConvertOrderTypeToBinanceOrderType(
    const OrderType &order_type);
}  // namespace stonks::finance

#endif  // STONKS_FINANCE_API_FINANCE_CONVERSIONS_H_
