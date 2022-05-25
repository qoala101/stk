#include "finance_conversions.h"

#include "finance_types.h"
#include "utils.h"

namespace stonks::finance {
Symbol ParseSymbolFromBinanceSymbol(std::string_view symbol) {
  if (symbol.size() > 3) {
    return Symbol{.base_asset = std::string{symbol.substr(0, 3)},
                  .quote_asset = std::string{symbol.substr(3)}};
  }

  return {};
}

Candle ParseCandleFromBinanceKline(const binance::Kline &kline) {
  return Candle{.open_time = kline.open_time,
                .close_time = kline.close_time,
                .data = Candle::Data{.open_price = kline.open_price,
                                     .high_price = kline.high_price,
                                     .low_price = kline.low_price,
                                     .close_price = kline.close_price,
                                     .volume = kline.volume}};
}

auto ParseSymbolPriceTickFromBinanceSymbolBookTicker(
    const binance::SymbolBookTicker &symbol_book_ticker) -> SymbolPriceTick {
  return SymbolPriceTick{.symbol = symbol_book_ticker.symbol,
                         .time = utils::GetUnixTime(),
                         .buy_price = symbol_book_ticker.best_ask_price,
                         .sell_price = symbol_book_ticker.best_bid_price};
}

OrderInfo ParseOrderInfoFromBinanceOrderInfo(
    const binance::OrderInfo &order_info) {
  return OrderInfo{
      .order_status = order_info.status,
      .requested_amount = order_info.original_quantity,
      .executed_amount = order_info.executed_quantity,
      .price = order_info.price,
      .executed_quote_amount = order_info.cummulative_quote_quantity};
}

OrderError ParseOrderErrorFromBinanceApiError(
    const binance::ApiError &api_error) {
  return OrderError{.message = std::to_string(api_error.code) + ": " +
                               api_error.message};
}

binance::OrderType ConvertOrderTypeToBinanceOrderType(
    const OrderType &order_type) {
  return std::visit(
      [](const auto &variant) {
        using T = std::decay_t<decltype(variant)>;

        if constexpr (std::is_same_v<T, MarketOrderType>) {
          return binance::OrderType::kMarket;
        }

        if constexpr (std::is_same_v<T, LimitOrderType>) {
          return binance::OrderType::kLimit;
        }

        return binance::OrderType::kInvalid;
      },
      order_type.order_type);
}
}  // namespace stonks::finance