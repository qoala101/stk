#include "binance_api.h"

#include <spdlog/spdlog.h>

#include "rest_request.h"

namespace
{
  std::optional<std::vector<stonks::binance::Symbol>> SymbolsFromExchangeInfo(const web::json::value &exchange_info)
  {
    if (!exchange_info.has_field("symbols"))
    {
      return std::nullopt;
    }

    const auto &symbols_value = exchange_info.at("symbols");

    if (!symbols_value.is_array())
    {
      return std::nullopt;
    }

    const auto &symbols_array = symbols_value.as_array();

    const auto has_usdt_quote_asset = [](const web::json::value &value)
    {
      if (!value.has_string_field("quoteAsset"))
      {
        return false;
      }

      return value.at("quoteAsset").as_string() == "USDT";
    };
    const auto has_symbol_field = [](const web::json::value &value)
    {
      return value.has_string_field("symbol");
    };
    const auto get_symbol_field = [](const web::json::value &value)
    {
      return value.at("symbol").as_string();
    };

    auto symbols_range = symbols_array | (std::views::filter(has_usdt_quote_asset) | std::views::filter(has_symbol_field) | std::views::transform(get_symbol_field));
    return std::vector<stonks::binance::Symbol>(symbols_range.begin(), symbols_range.end());
  }
}

namespace stonks::binance
{
  std::optional<std::vector<Symbol>> GetSymbols()
  {
    const auto exchange_info = rest::RestRequest{"https://testnet.binance.vision/api/v3/exchangeInfo"}.SendAndGetResponse();

    if (!exchange_info.has_value())
    {
      return std::nullopt;
    }

    const auto symbols = SymbolsFromExchangeInfo(exchange_info.value());

    if (!symbols.has_value())
    {
      return std::nullopt;
    }

    return symbols.value();
  }
}