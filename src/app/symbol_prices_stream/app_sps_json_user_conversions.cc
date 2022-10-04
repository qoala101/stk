#include <string>

#include "app_sps_types.h"
#include "network_i_json.h"
#include "network_json_basic_conversions.h"
#include "network_json_conversions_facades.h"

namespace stonks::network {
template <>
auto JsonParser<app::sps::BinanceSymbolBookTick>::operator()(
    const IJson &json) const -> Type {
  return {
      .best_bid_price = std::stod(ParseFromJsonChild<std::string>(json, "b")),
      .best_ask_price = std::stod(ParseFromJsonChild<std::string>(json, "a"))};
}
}  // namespace stonks::network
