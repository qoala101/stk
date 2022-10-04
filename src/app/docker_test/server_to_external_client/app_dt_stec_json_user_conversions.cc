#include <absl/time/time.h>

#include <string>

#include "app_dt_stec_binance_types.h"
#include "network_json_basic_conversions.h"
#include "network_json_conversions_facades.h"

namespace stonks::network {
template <>
auto JsonParser<app::dt::stec::binance::AvgPrice>::operator()(
    const IJson &json) const -> Type {
  return {.mins = ParseFromJsonChild<int>(json, "mins"),
          .price = std::stod(ParseFromJsonChild<std::string>(json, "price"))};
}
}  // namespace stonks::network
