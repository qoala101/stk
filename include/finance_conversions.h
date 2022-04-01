#ifndef STONKS_FINANCE_CONVERSIONS_H_
#define STONKS_FINANCE_CONVERSIONS_H_

#include <span>
#include <vector>

#include "binance_types.h"
#include "finance_types.h"

namespace stonks {
Candlestick ParseCandlestickFromBinanceKline(const binance::Kline &kline);
}

#endif  // STONKS_FINANCE_CONVERSIONS_H_
