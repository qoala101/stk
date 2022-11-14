#ifndef STONKS_APP_CORE_SYMBOL_PRICES_STREAM_CORE_SPS_TYPES_H_
#define STONKS_APP_CORE_SYMBOL_PRICES_STREAM_CORE_SPS_TYPES_H_

#include <string>

namespace stonks::app::sps {
struct BinanceBookTick {
  std::string best_bid_price{};
  std::string best_ask_price{};
};
}  // namespace stonks::app::sps

#endif  // STONKS_APP_CORE_SYMBOL_PRICES_STREAM_CORE_SPS_TYPES_H_
