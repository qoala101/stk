#include "ngrok_api.h"

#include <fmt/core.h>

#include <coroutine>
#include <not_null.hpp>
#include <utility>

#include "networkx_client.h"
#include "ngrok_api_traits.h"  // IWYU pragma: keep
#include "ngrok_types.h"

namespace stonks::ngrok {
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

NgrokApi::NgrokApi(
    std::string_view host,
    di::Factory<network::IRestRequestSender> request_sender_factory)
    : impl_{cpp::MakeNnUp<NgrokApi::Impl>(
          networkx::Client<NgrokApi>{{fmt::format("https://{}:4040/api", host)},
                                     std::move(request_sender_factory)})} {}

auto NgrokApi::tunnels() const -> cppcoro::task<Tunnels> {
  co_return co_await impl_->GetClient().Call<&NgrokApi::tunnels>();
}
}  // namespace stonks::ngrok