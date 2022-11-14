#include <polymorphic_value.h>

#include <string>
#include <vector>

#include "core_siu_types.h"
#include "network_i_json.h"
#include "network_json_base_conversions.h"
#include "network_json_common_conversions.h"
#include "network_json_conversions_facades.h"

namespace stonks::network {
template <>
auto JsonParser<core::siu::BinanceSymbolExchangeInfo>::operator()(
    const IJson &json) const -> Type {
  auto value = MakeFromJson<Type>(json, "symbol", "baseAsset", "quoteAsset");
  auto filters = json.GetChild("filters");

  for (auto i = 0; i < filters->GetSize(); ++i) {
    auto filter = filters->GetChild(i);
    const auto type = ParseFromJsonChild<std::string>(*filter, "filterType");

    if (type == "LOT_SIZE") {
      value.min_quantity = ParseFromJsonChild<std::string>(*filter, "minQty");
      value.step_size = ParseFromJsonChild<std::string>(*filter, "stepSize");
      continue;
    }

    if (type == "MIN_NOTIONAL") {
      value.min_notional =
          ParseFromJsonChild<std::string>(*filter, "minNotional");
      continue;
    }

    if (type == "PRICE_FILTER") {
      value.tick_size = ParseFromJsonChild<std::string>(*filter, "tickSize");
      continue;
    }
  }

  return value;
}

template <>
auto JsonParser<core::siu::BinanceExchangeInfo>::operator()(
    const IJson &json) const -> Type {
  return {ParseFromJsonChild<std::vector<core::siu::BinanceSymbolExchangeInfo>>(
      json, "symbols")};
}
}  // namespace stonks::network
