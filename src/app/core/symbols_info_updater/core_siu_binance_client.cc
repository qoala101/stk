#include "core_siu_binance_client.h"

#include <utility>

#include "core_siu_endpoints.h"
#include "network_rest_client.h"
#include "network_rest_client_request_builder.h"

namespace stonks::app::siu {
BinanceClient::BinanceClient(network::RestClient rest_client)
    : rest_client_{std::move(rest_client)} {}

auto BinanceClient::BinanceExchangeInfo() const -> struct BinanceExchangeInfo {
  return rest_client_.Call(endpoints::BinanceExchangeInfo())
      .AndReceive<struct BinanceExchangeInfo>();
}
}  // namespace stonks::app::siu