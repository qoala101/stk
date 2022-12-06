#include <string>
#include <vector>

#include "binance_types.h"
#include "cpp_polymorphic_value.h"
#include "network_i_json.h"
#include "network_json_base_conversions.h"
#include "network_json_common_conversions.h"
#include "network_json_conversions_facades.h"

namespace stonks::network {
template <>
auto JsonParser<binance::SymbolExchangeInfo>::operator()(
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
auto JsonParser<binance::ExchangeInfo>::operator()(const IJson &json) const
    -> Type {
  return {ParseFromJsonChild<std::vector<binance::SymbolExchangeInfo>>(
      json, "symbols")};
}

template <>
auto JsonParser<binance::BookTick>::operator()(const IJson &json) const
    -> Type {
  return {.best_bid_price = ParseFromJsonChild<std::string>(json, "b"),
          .best_ask_price = ParseFromJsonChild<std::string>(json, "a")};
}
}  // namespace stonks::network
