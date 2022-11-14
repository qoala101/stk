#include <string>

#include "core_sps_types.h"
#include "network_i_json.h"
#include "network_json_base_conversions.h"
#include "network_json_conversions_facades.h"

namespace stonks::network {
template <>
auto JsonParser<core::sps::BinanceBookTick>::operator()(const IJson &json) const
    -> Type {
  return {.best_bid_price = ParseFromJsonChild<std::string>(json, "b"),
          .best_ask_price = ParseFromJsonChild<std::string>(json, "a")};
}
}  // namespace stonks::network
