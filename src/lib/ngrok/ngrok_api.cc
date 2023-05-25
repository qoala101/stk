/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "ngrok_api.h"

#include <coroutine>
#include <not_null.hpp>
#include <utility>

#include "networkx_client.h"
#include "ngrok_api_traits.h"  // IWYU pragma: keep
#include "ngrok_types.h"

namespace vh::ngrok {
class NgrokApi::Impl {
 public:
  explicit Impl(networkx::Client<NgrokApi> client)
      : client_{std::move(client)} {}

  auto GetClient [[nodiscard]] () -> auto & { return client_; }

 private:
  networkx::Client<NgrokApi> client_;
};

NgrokApi::NgrokApi(NgrokApi &&) noexcept = default;

auto NgrokApi::operator=(NgrokApi &&) noexcept -> NgrokApi & = default;

NgrokApi::~NgrokApi() = default;

NgrokApi::NgrokApi(cpp::NnUp<network::IRestRequestSender> request_sender)
    : impl_{cpp::MakeNnUp<NgrokApi::Impl>(networkx::Client<NgrokApi>{
          {"http://0.0.0.0:4040/api"}, std::move(request_sender)})} {}

auto NgrokApi::tunnels() const -> cppcoro::task<Tunnels> {
  co_return co_await impl_->GetClient().Call<&NgrokApi::tunnels>();
}
}  // namespace vh::ngrok