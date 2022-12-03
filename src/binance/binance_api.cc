#include "binance_api.h"

#include "binance_api_traits.h"
#include "network_rest_client.h"
#include "network_types.h"
#include "networkx_client.h"

namespace stonks::binance {
struct BinanceApi::Impl {
  explicit Impl(di::Factory<network::IRestRequestSender> request_sender_factory)
      : client{{network::Uri{"https://api.binance.com/api/v3"},
                std::move(request_sender_factory)}} {}

  networkx::Client<BinanceApi> client;
};

BinanceApi::BinanceApi(BinanceApi &&) noexcept = default;

auto BinanceApi::operator=(BinanceApi &&) noexcept -> BinanceApi & = default;

BinanceApi::~BinanceApi() = default;

BinanceApi::BinanceApi(
    di::Factory<network::IRestRequestSender> request_sender_factory)
    : impl_{
          cpp::MakeNnUp<BinanceApi::Impl>(std::move(request_sender_factory))} {}

auto BinanceApi::exchangeInfo() const -> cppcoro::task<ExchangeInfo> {
  co_return co_await impl_->client
      .template Call<&binance::BinanceApi::exchangeInfo>();
}
}  // namespace stonks::binance