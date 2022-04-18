#ifndef STONKS_BINANCE_BINANCE_ENUMS_H_
#define STONKS_BINANCE_BINANCE_ENUMS_H_

namespace stonks::binance {
enum class OrderSide { kInvalid, kBuy, kSell };

enum class OrderStatus {
  kInvalid,
  kNew,
  kPartiallyFilled,
  kFilled,
  kCancelled,
  kPendingCancel,
  kRejected,
  kExpired,
};

enum class OrderTimeInForce {
  kInvalid,
  kGoodTillCanceled,
  kImmediateOrCancel,
  kFillOrKill
};

enum class OrderType {
  kInvalid,
  kLimit,
  kMarket,
  kStopLoss,
  kStopLossLimit,
  kTakeProfit,
  kTakeProfitLimit,
  kLimitMaker
};

enum class NewOrderResponseType {
  kInvalid,
  kAcknowledgement,
  kResult,
  kFull
};

enum class CandleInterval {
  kInvalid,
  k1Minute,
  k3Minutes,
  k5Minutes,
  k15Minutes,
  k30Minutes,
  k1Hour,
  k2Hours,
  k4Hours,
  k6Hours,
  k8Hours,
  k12Hours,
  k1Day,
  k3Days,
  k1Week,
  k1Month
};
}  // namespace stonks::binance

#endif  // STONKS_BINANCE_BINANCE_ENUMS_H_
