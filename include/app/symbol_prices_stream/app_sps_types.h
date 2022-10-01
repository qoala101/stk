#ifndef STONKS_APP_SYMBOL_PRICES_STREAM_APP_SPS_TYPES_H_
#define STONKS_APP_SYMBOL_PRICES_STREAM_APP_SPS_TYPES_H_

namespace stonks::app::sps {
struct BinanceSymbolBookTick {
  double best_bid_price{};
  double best_ask_price{};
};
}  // namespace stonks::app::sps

#endif  // STONKS_APP_SYMBOL_PRICES_STREAM_APP_SPS_TYPES_H_
