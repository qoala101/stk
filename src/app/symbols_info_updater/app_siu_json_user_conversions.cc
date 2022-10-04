#include <vector>

#include "app_siu_types.h"
#include "cpp_typed_struct.h"
#include "network_i_json.h"
#include "network_json_base_conversions.h"
#include "network_json_common_conversions.h"
#include "network_json_conversions_facades.h"

namespace stonks::network {
template <>
auto JsonParser<app::siu::BinanceSymbolExchangeInfo>::operator()(
    const IJson &json) const -> Type {
  auto data = MakeFromJson<Type>(json, "symbol", "baseAsset", "quoteAsset");
  // {
  //     .symbol = ParseJsonElement<std::string>(json, "symbol"),
  //     .base_asset = ParseJsonElement<std::string>(json, "baseAsset"),
  //     .quote_asset = ParseJsonElement<std::string>(json, "quoteAsset")};

  // const auto& filters = json.at("filters").as_array();

  // for (const auto& filter : filters) {
  //   const auto type = ParseJsonElement<std::string>(filter, "filterType");

  //   if (type == "LOT_SIZE") {
  //     data.min_quantity = ParseJsonElement<double>(filter, "minQty");
  //     data.step_size = ParseJsonElement<double>(filter, "stepSize");
  //     continue;
  //   }

  //   if (type == "MIN_NOTIONAL") {
  //     data.min_notional = ParseJsonElement<double>(filter, "minNotional");
  //     continue;
  //   }

  //   if (type == "PRICE_FILTER") {
  //     data.tick_size = ParseJsonElement<double>(filter, "tickSize");
  //     continue;
  //   }
  // }

  return data;
}

template <>
auto JsonParser<std::vector<app::siu::BinanceSymbolExchangeInfo>>::operator()(
    const IJson &json) const -> Type {
  return {app::siu::BinanceSymbolExchangeInfo{.symbol = "btcusdt"},
          app::siu::BinanceSymbolExchangeInfo{.symbol = "ethusdt"}};
}
}  // namespace stonks::network
