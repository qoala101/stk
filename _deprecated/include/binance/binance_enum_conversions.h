#ifndef STONKS_BINANCE_BINANCE_ENUM_CONVERSIONS_H_
#define STONKS_BINANCE_BINANCE_ENUM_CONVERSIONS_H_

#include <magic_enum.hpp>
#include <string_view>

#include "binance_enums.h"

template <>
constexpr std::string_view
magic_enum::customize::enum_name<stonks::binance::OrderSide>(
    stonks::binance::OrderSide value) noexcept {
  switch (value) {
    case stonks::binance::OrderSide::kBuy:
      return "BUY";
    case stonks::binance::OrderSide::kSell:
      return "SELL";
    default:
      break;
  }

  return {};
}

template <>
constexpr std::string_view
magic_enum::customize::enum_name<stonks::binance::OrderStatus>(
    stonks::binance::OrderStatus value) noexcept {
  switch (value) {
    case stonks::binance::OrderStatus::kNew:
      return "NEW";
    case stonks::binance::OrderStatus::kPartiallyFilled:
      return "PARTIALLY_FILLED";
    case stonks::binance::OrderStatus::kFilled:
      return "FILLED";
    case stonks::binance::OrderStatus::kCancelled:
      return "CANCELED";
    case stonks::binance::OrderStatus::kPendingCancel:
      return "PENDING_CANCEL";
    case stonks::binance::OrderStatus::kRejected:
      return "REJECTED";
    case stonks::binance::OrderStatus::kExpired:
      return "EXPIRED";
    default:
      break;
  }

  return {};
}

template <>
constexpr std::string_view
magic_enum::customize::enum_name<stonks::binance::OrderTimeInForce>(
    stonks::binance::OrderTimeInForce value) noexcept {
  switch (value) {
    case stonks::binance::OrderTimeInForce::kGoodTillCanceled:
      return "GTC";
    case stonks::binance::OrderTimeInForce::kImmediateOrCancel:
      return "IOC";
    case stonks::binance::OrderTimeInForce::kFillOrKill:
      return "FOK";
    default:
      break;
  }

  return {};
}

template <>
constexpr std::string_view
magic_enum::customize::enum_name<stonks::binance::OrderType>(
    stonks::binance::OrderType value) noexcept {
  switch (value) {
    case stonks::binance::OrderType::kLimit:
      return "LIMIT";
    case stonks::binance::OrderType::kMarket:
      return "MARKET";
    case stonks::binance::OrderType::kStopLoss:
      return "STOP_LOSS";
    case stonks::binance::OrderType::kStopLossLimit:
      return "STOP_LOSS_LIMIT";
    case stonks::binance::OrderType::kTakeProfit:
      return "TAKE_PROFIT";
    case stonks::binance::OrderType::kTakeProfitLimit:
      return "TAKE_PROFIT_LIMIT";
    case stonks::binance::OrderType::kLimitMaker:
      return "LIMIT_MARKET";
    default:
      break;
  }

  return {};
}

template <>
constexpr std::string_view
magic_enum::customize::enum_name<stonks::binance::NewOrderResponseType>(
    stonks::binance::NewOrderResponseType value) noexcept {
  switch (value) {
    case stonks::binance::NewOrderResponseType::kAcknowledgement:
      return "ACK";
    case stonks::binance::NewOrderResponseType::kResult:
      return "RESULT";
    case stonks::binance::NewOrderResponseType::kFull:
      return "FULL";
    default:
      break;
  }

  return {};
}

template <>
constexpr std::string_view
magic_enum::customize::enum_name<stonks::binance::CandleInterval>(
    stonks::binance::CandleInterval value) noexcept {
  switch (value) {
    case stonks::binance::CandleInterval::k1Minute:
      return "1m";
    case stonks::binance::CandleInterval::k3Minutes:
      return "3m";
    case stonks::binance::CandleInterval::k5Minutes:
      return "5m";
    case stonks::binance::CandleInterval::k15Minutes:
      return "15m";
    case stonks::binance::CandleInterval::k30Minutes:
      return "30m";
    case stonks::binance::CandleInterval::k1Hour:
      return "1h";
    case stonks::binance::CandleInterval::k2Hours:
      return "2h";
    case stonks::binance::CandleInterval::k4Hours:
      return "4h";
    case stonks::binance::CandleInterval::k6Hours:
      return "6h";
    case stonks::binance::CandleInterval::k8Hours:
      return "8h";
    case stonks::binance::CandleInterval::k12Hours:
      return "12h";
    case stonks::binance::CandleInterval::k1Day:
      return "1d";
    case stonks::binance::CandleInterval::k3Days:
      return "3d";
    case stonks::binance::CandleInterval::k1Week:
      return "1w";
    case stonks::binance::CandleInterval::k1Month:
      return "1M";
    default:
      break;
  }

  return {};
}

#endif  // STONKS_BINANCE_BINANCE_ENUM_CONVERSIONS_H_
