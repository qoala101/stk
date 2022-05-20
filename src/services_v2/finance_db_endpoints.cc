#include "finance_db_endpoints.h"

namespace stonks::finance {
auto SelectAssetsEndpoint() -> stonks::network::EndpointDesc {
  return stonks::network::EndpointDesc{
      .method = web::http::methods::GET,
      .relative_uri = "/SelectAssets",
      .response_body = stonks::json::AnyDesc{
          .converter = stonks::json::JsonConverter<
              std::optional<std::vector<std::string>>>{},
          .optional = stonks::json::OptionalFlag::kOptional}};
}

auto SelectSymbolsEndpoint() -> stonks::network::EndpointDesc {
  return stonks::network::EndpointDesc{
      .method = web::http::methods::GET,
      .relative_uri = "/SelectSymbols",
      .response_body = stonks::json::AnyDesc{
          .converter =
              stonks::json::JsonConverter<std::optional<std::vector<Symbol>>>{},
          .optional = stonks::json::OptionalFlag::kOptional}};
}

auto InsertSymbolPriceTickEndpoint() -> stonks::network::EndpointDesc {
  return stonks::network::EndpointDesc{
      .method = web::http::methods::POST,
      .relative_uri = "/InsertSymbolPriceTick",
      .request_body =
          stonks::json::AnyDesc{
              .converter = stonks::json::JsonConverter<SymbolPriceTick>{},
              .optional = stonks::json::OptionalFlag::kMandatory},
      .response_body = stonks::json::AnyDesc{
          .converter = stonks::json::JsonConverter<bool>{},
          .optional = stonks::json::OptionalFlag::kMandatory}};
}

auto SelectSymbolPriceTicksEndpoint() -> stonks::network::EndpointDesc {
  return stonks::network::EndpointDesc{
      .method = web::http::methods::GET,
      .relative_uri = "/SelectSymbolPriceTicks",
      .params = {{"limit",
                  stonks::json::AnyDesc{
                      .converter =
                          stonks::json::JsonConverter<std::optional<int>>{},
                      .optional = stonks::json::OptionalFlag::kOptional}},
                 {"period",
                  stonks::json::AnyDesc{
                      .converter =
                          stonks::json::JsonConverter<std::optional<Period>>{},
                      .optional = stonks::json::OptionalFlag::kOptional}},
                 {"symbols",
                  stonks::json::AnyDesc{
                      .converter = stonks::json::JsonConverter<
                          std::optional<std::vector<Symbol>>>{},
                      .optional = stonks::json::OptionalFlag::kOptional}}},
      .response_body = stonks::json::AnyDesc{
          .converter = stonks::json::JsonConverter<
              std::optional<std::vector<SymbolPriceTick>>>{},
          .optional = stonks::json::OptionalFlag::kOptional}};
}
}  // namespace stonks::finance