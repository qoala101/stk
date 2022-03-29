#ifndef STONKS_BINANCE_ENUMS_H_
#define STONKS_BINANCE_ENUMS_H_

namespace stonks::binance {
enum class Side { kInvalid, kBuy, kSell };

enum class Status {
  kInvalid,
  kNew,
  kPartiallyFilled,
  kFilled,
  kCancelled,
  kPendingCancel,
  kRejected,
  kExpired,
};

enum class TimeInForce {
  kInvalid,
  kGoodTillCanceled,
  kImmediateOrCancel,
  kFillOrKill
};

enum class Type {
  kInvalid,
  kLimit,
  kMarket,
  kStopLoss,
  kStopLossLimit,
  kTakeProfit,
  kTakeProfitLimit,
  kLimitMaker
};
}  // namespace stonks::binance

#endif  // STONKS_BINANCE_ENUMS_H_
