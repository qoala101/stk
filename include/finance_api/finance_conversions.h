#ifndef STONKS_FINANCE_CONVERSIONS_H_
#define STONKS_FINANCE_CONVERSIONS_H_

#include <span>
#include <string_view>
#include <vector>

#include "binance_types.h"
#include "finance_db.h"
#include "finance_types.h"

namespace stonks::finance {
Symbol ParseSymbolFromBinanceSymbol(std::string_view symbol,
                                    const FinanceDb &finance_db);

Candle ParseCandleFromBinanceKline(const binance::Kline &kline);

SymbolPriceTick ParseSymbolPriceTickFromBinanceSymbolBookTicker(
    const binance::SymbolBookTicker &symbol_book_ticker,
    const FinanceDb &finance_db);

OrderInfo ParseOrderInfoFromBinanceOrderInfo(
    const binance::OrderInfo &order_info);

OrderError ParseOrderErrorFromBinanceApiError(
    const binance::ApiError &api_error);

binance::OrderType ConvertOrderTypeToBinanceOrderType(
    const OrderType &order_type);
}  // namespace stonks::finance

#endif  // STONKS_FINANCE_CONVERSIONS_H_
