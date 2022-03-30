#ifndef STONKS_BINANCE_ENUM_CONVERSIONS_H_
#define STONKS_BINANCE_ENUM_CONVERSIONS_H_

#include <magic_enum.hpp>
#include <string_view>

#include "enums.h"

template <>
constexpr std::string_view
magic_enum::customize::enum_name<stonks::binance::Side>(
    stonks::binance::Side value) noexcept {
  switch (value) {
    case stonks::binance::Side::kBuy:
      return "BUY";
    case stonks::binance::Side::kSell:
      return "SELL";
    default:
      break;
  }

  return {};
}

template <>
constexpr std::string_view
magic_enum::customize::enum_name<stonks::binance::Status>(
    stonks::binance::Status value) noexcept {
  switch (value) {
    case stonks::binance::Status::kNew:
      return "NEW";
    case stonks::binance::Status::kPartiallyFilled:
      return "PARTIALLY_FILLED";
    case stonks::binance::Status::kFilled:
      return "FILLED";
    case stonks::binance::Status::kCancelled:
      return "CANCELED";
    case stonks::binance::Status::kPendingCancel:
      return "PENDING_CANCEL";
    case stonks::binance::Status::kRejected:
      return "REJECTED";
    case stonks::binance::Status::kExpired:
      return "EXPIRED";
    default:
      break;
  }

  return {};
}

template <>
constexpr std::string_view
magic_enum::customize::enum_name<stonks::binance::TimeInForce>(
    stonks::binance::TimeInForce value) noexcept {
  switch (value) {
    case stonks::binance::TimeInForce::kGoodTillCanceled:
      return "GTC";
    case stonks::binance::TimeInForce::kImmediateOrCancel:
      return "IOC";
    case stonks::binance::TimeInForce::kFillOrKill:
      return "FOK";
    default:
      break;
  }

  return {};
}

template <>
constexpr std::string_view
magic_enum::customize::enum_name<stonks::binance::Type>(
    stonks::binance::Type value) noexcept {
  switch (value) {
    case stonks::binance::Type::kLimit:
      return "LIMIT";
    case stonks::binance::Type::kMarket:
      return "MARKET";
    case stonks::binance::Type::kStopLoss:
      return "STOP_LOSS";
    case stonks::binance::Type::kStopLossLimit:
      return "STOP_LOSS_LIMIT";
    case stonks::binance::Type::kTakeProfit:
      return "TAKE_PROFIT";
    case stonks::binance::Type::kTakeProfitLimit:
      return "TAKE_PROFIT_LIMIT";
    case stonks::binance::Type::kLimitMaker:
      return "LIMIT_MARKET";
    default:
      break;
  }

  return {};
}

template <>
constexpr std::string_view
magic_enum::customize::enum_name<stonks::binance::CandlestickInterval>(
    stonks::binance::CandlestickInterval value) noexcept {
  switch (value) {
    case stonks::binance::CandlestickInterval::k1Minute:
      return "1m";
    case stonks::binance::CandlestickInterval::k3Minutes:
      return "3m";
    case stonks::binance::CandlestickInterval::k5Minutes:
      return "5m";
    case stonks::binance::CandlestickInterval::k15Minutes:
      return "15m";
    case stonks::binance::CandlestickInterval::k30Minutes:
      return "30m";
    case stonks::binance::CandlestickInterval::k1Hour:
      return "1h";
    case stonks::binance::CandlestickInterval::k2Hours:
      return "2h";
    case stonks::binance::CandlestickInterval::k4Hours:
      return "4h";
    case stonks::binance::CandlestickInterval::k6Hours:
      return "6h";
    case stonks::binance::CandlestickInterval::k8Hours:
      return "8h";
    case stonks::binance::CandlestickInterval::k12Hours:
      return "12h";
    case stonks::binance::CandlestickInterval::k1Day:
      return "1d";
    case stonks::binance::CandlestickInterval::k3Days:
      return "3d";
    case stonks::binance::CandlestickInterval::k1Week:
      return "1w";
    case stonks::binance::CandlestickInterval::k1Month:
      return "1M";
    default:
      break;
  }

  return {};
}

#endif  // STONKS_BINANCE_ENUM_CONVERSIONS_H_
