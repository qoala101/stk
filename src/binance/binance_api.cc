#include "binance_api.h"

#include <coroutine>
#include <memory>
#include <utility>

#include "binance_api_traits.h"  // IWYU pragma: keep
#include "networkx_client.h"
#include "not_null.hpp"

namespace stonks::binance {
class BinanceApi::Impl {
 public:
  explicit Impl(networkx::Client<BinanceApi> client)
      : client{std::move(client)} {}

  auto GetClient [[nodiscard]] () -> auto & { return client; }

 private:
  networkx::Client<BinanceApi> client;
};

BinanceApi::BinanceApi(BinanceApi &&) noexcept = default;

auto BinanceApi::operator=(BinanceApi &&) noexcept -> BinanceApi & = default;

BinanceApi::~BinanceApi() = default;

BinanceApi::BinanceApi(
    di::Factory<network::IRestRequestSender> request_sender_factory)
    : impl_{cpp::MakeNnUp<BinanceApi::Impl>(
          networkx::Client<BinanceApi>{{{"https://api.binance.com/api/v3"},
                                        std::move(request_sender_factory)}})} {}

auto BinanceApi::exchangeInfo() const -> cppcoro::task<ExchangeInfo> {
  co_return co_await impl_->GetClient()
      .Call<&binance::BinanceApi::exchangeInfo>();
}
}  // namespace stonks::binance