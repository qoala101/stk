#include "core_siu_binance_client.h"

#include <memory>
#include <utility>

#include "core_siu_endpoints.h"
#include "network_rest_client.h"
#include "network_rest_client_request_builder.h"

namespace stonks::core::siu {
BinanceClient::BinanceClient(
    di::Factory<network::IRestRequestSender> request_sender_factory)
    : rest_client_{{"https://api.binance.com/api/v3"},
                   std::move(request_sender_factory)} {}

auto BinanceClient::BinanceExchangeInfo() const -> struct BinanceExchangeInfo {
  return rest_client_.Call(endpoints::BinanceExchangeInfo())
      .AndReceive<struct BinanceExchangeInfo>();
}
}  // namespace stonks::core::siu