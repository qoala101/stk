#include "app_siu_json_user_conversions.h"

#include <vector>

#include "app_siu_types.h"
#include "network_json_basic_conversions.h"
#include "network_json_conversions_facades.h"

namespace stonks::network {
template <>
auto ParseFromJson(const IJson &json) -> app::siu::BinanceSymbolExchangeInfo {
  auto data = MakeFromJson<app::siu::BinanceSymbolExchangeInfo>(
      json, "symbol", "baseAsset", "quoteAsset");
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
[[nodiscard]] auto ParseFromJson(const IJson &json)
    -> std::vector<app::siu::BinanceSymbolExchangeInfo> {
  return {app::siu::BinanceSymbolExchangeInfo{.symbol = "btcusdt"},
          app::siu::BinanceSymbolExchangeInfo{.symbol = "ethusdt"}};
}
}  // namespace stonks::network
