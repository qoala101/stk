#include "stonks_endpoints_db.h"

#include <string>

#include "network_enums.h"

namespace stonks::endpoints::db {
auto SelectAssets() -> const network::Endpoint& {
  static auto endpoint = network::Endpoint{.method = network::Method::kGet,
                                           .uri = "/SelectAssets"};
  return endpoint;
}

auto UpdateAssets() -> const network::Endpoint& {
  static auto endpoint = network::Endpoint{.method = network::Method::kPost,
                                           .uri = "/UpdateAssets"};
  return endpoint;
}

auto SelectSymbols() -> const network::Endpoint& {
  static auto endpoint = network::Endpoint{.method = network::Method::kGet,
                                           .uri = "/SelectSymbols"};
  return endpoint;
}

auto SelectSymbolsInfo() -> const network::Endpoint& {
  static auto endpoint = network::Endpoint{.method = network::Method::kGet,
                                           .uri = "/SelectSymbolsInfo"};
  return endpoint;
}

auto UpdateSymbolsInfo() -> const network::Endpoint& {
  static auto endpoint = network::Endpoint{.method = network::Method::kPost,
                                           .uri = "/UpdateSymbolsInfo"};
  return endpoint;
}

auto SelectSymbolPriceTicks() -> const network::Endpoint& {
  static auto endpoint = network::Endpoint{.method = network::Method::kGet,
                                           .uri = "/SelectSymbolPriceTicks"};
  return endpoint;
}

auto InsertSymbolPriceTick() -> const network::Endpoint& {
  static auto endpoint = network::Endpoint{.method = network::Method::kPost,
                                           .uri = "/InsertSymbolPriceTick"};
  return endpoint;
}
}  // namespace stonks::endpoints::db