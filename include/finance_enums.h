#ifndef STONKS_FINANCE_ENUMS_H_
#define STONKS_FINANCE_ENUMS_H_

#include "binance_enums.h"

namespace stonks::finance {
using Interval = binance::CandleInterval;
using BuyOrSell = binance::OrderSide;

enum class StrategyResult { kInvalid, kOne, kMinusOne, kZero };
}  // namespace stonks::finance

#endif  // STONKS_FINANCE_ENUMS_H_
