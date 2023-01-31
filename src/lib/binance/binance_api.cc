#include "binance_api.h"

#include <coroutine>
#include <not_null.hpp>
#include <utility>

#include "binance_api_traits.h"  // IWYU pragma: keep
#include "networkx_client.h"

namespace vh::binance {
class BinanceApi::Impl {
 public:
  explicit Impl(networkx::Client<BinanceApi> client)
      : client_{std::move(client)} {}

  auto GetClient [[nodiscard]] () -> auto & { return client_; }

 private:
  networkx::Client<BinanceApi> client_;
};

BinanceApi::BinanceApi(BinanceApi &&) noexcept = default;

auto BinanceApi::operator=(BinanceApi &&) noexcept -> BinanceApi & = default;

BinanceApi::~BinanceApi() = default;

BinanceApi::BinanceApi(cpp::NnUp<network::IRestRequestSender> request_sender)
    : impl_{cpp::MakeNnUp<BinanceApi::Impl>(networkx::Client<BinanceApi>{
          {"https://api.binance.com/api/v3"}, std::move(request_sender)})} {}

auto BinanceApi::exchangeInfo() const -> cppcoro::task<ExchangeInfo> {
  co_return co_await impl_->GetClient()
      .Call<&binance::BinanceApi::exchangeInfo>();
}
}  // namespace vh::binance