#include "app_sps_json_user_conversions.h"

#include <string>

#include "network_json_basic_conversions.h"
#include "network_json_conversions_facades.h"

namespace stonks::network {
template <>
auto ParseFromJson(const IJson &json) -> app::sps::BinanceSymbolBookTick {
  return {
      .best_bid_price = std::stod(ParseFromJsonChild<std::string>(json, "b")),
      .best_ask_price = std::stod(ParseFromJsonChild<std::string>(json, "a"))};
}
}  // namespace stonks::network
