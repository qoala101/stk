#include "app_dt_stec_json_user_conversions.h"

#include <string>

#include "network_json_basic_conversions.h"
#include "network_json_conversions_facades.h"

namespace stonks::network {
template <>
auto ParseFromJson(const IJson &json) -> app::dt::stec::binance::AvgPrice {
  return {.mins = ParseFromJsonChild<int>(json, "mins"),
          .price = std::stod(ParseFromJsonChild<std::string>(json, "price"))};
}
}  // namespace stonks::network
