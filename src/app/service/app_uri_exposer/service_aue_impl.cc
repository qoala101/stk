#include "service_aue_impl.h"

#include <fmt/core.h>

#include <coroutine>
#include <string>
#include <utility>
#include <vector>

#include "cpp_typed_struct.h"
#include "network_types.h"
#include "ngrok_types.h"
#include "not_null.hpp"

namespace stonks::service::aue {
Impl::Impl(PublicDb public_db, ngrok::NgrokApi ngrok_api,
           cpp::NnUp<log::ILogger> logger)
    : public_db_{std::move(public_db)},
      ngrok_api_{std::move(ngrok_api)},
      logger_{std::move(logger)} {}

auto Impl::ExposeNgrokUri() const -> cppcoro::task<> {
  const auto tunnels = co_await ngrok_api_.tunnels();

  if (tunnels->empty()) {
    logger_->LogStrangeEvent("Ngrok has no ports to expose");
    co_return;
  }

  const auto uri = network::Uri{(*tunnels)[0].public_url};
  logger_->LogImportantEvent(
      fmt::format("Updating public app URI {}...", *uri));

  co_await public_db_.InsertOrUpdateAppUri({(*tunnels)[0].public_url});
  logger_->LogImportantEvent(fmt::format("Updated public app URI {}", *uri));
}
}  // namespace stonks::service::aue
