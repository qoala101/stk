/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "service_aue_impl.h"

#include <fmt/core.h>

#include <coroutine>
#include <exception>
#include <gsl/assert>
#include <not_null.hpp>
#include <string>
#include <utility>
#include <vector>

#include "cpp_typed_struct.h"
#include "network_types.h"
#include "ngrok_types.h"

namespace vh::stk::service::aue {
Impl::Impl(PublicDb public_db, ngrok::NgrokApi ngrok_api,
           cpp::NnUp<log::ILogger> logger)
    : public_db_{std::move(public_db)},
      ngrok_api_{std::move(ngrok_api)},
      logger_{std::move(logger)} {}

auto Impl::ExposeNgrokUriIfChanged() -> cppcoro::task<> {
  const auto tunnels = co_await ngrok_api_.tunnels();

  if (tunnels->empty()) {
    logger_->LogStrangeEvent("Ngrok has no ports to expose");
    co_return;
  }

  auto new_uri = network::Uri{tunnels->front().public_url};

  if (new_uri == last_exposed_uri_) {
    logger_->LogImportantEvent(
        fmt::format("Public app URI hasn't changed {}", *new_uri));
    co_return;
  }

  logger_->LogImportantEvent(
      fmt::format("Updating public app URI {}...", *new_uri));

  try {
    co_await public_db_.InsertOrUpdateAppUri(new_uri);
  } catch (const std::exception &e) {
    logger_->LogErrorCondition(
        fmt::format("Couldn't update public app URI: {}", e.what()));
    throw;
  }

  logger_->LogImportantEvent(
      fmt::format("Updated public app URI {}", *new_uri));

  last_exposed_uri_ = std::move(new_uri);
  Ensures(!last_exposed_uri_->empty());
}
}  // namespace vh::stk::service::aue
