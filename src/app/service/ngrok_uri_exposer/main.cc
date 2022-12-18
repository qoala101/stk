#include <absl/time/time.h>

#include <boost/di.hpp>
#include <cstdint>
#include <string>
#include <string_view>
#include <variant>

#include "cli_app.h"
#include "cli_option.h"
#include "cli_options.h"
#include "core_app_uri_exposer.h"
#include "core_aue_impl.h"
#include "di_bind_type_to_value.h"
#include "di_make_injector.h"
#include "service_kvdb_aws_injector.h"
#include "service_log_spdlog_injector.h"
#include "service_network_restsdk_injector.h"

auto main(int argc, const char *const *argv) -> int {
  auto options = stonks::cli::Options{};

  const auto expose_uri_interval = options.AddOption(
      "--expose_uri_interval", absl::ToInt64Milliseconds(absl::Hours(1)));
  const auto reattempt_interval = options.AddOption(
      "--reattempt_interval", absl::ToInt64Milliseconds(absl::Minutes(1)));

  const auto app = stonks::cli::App{argc, argv, options};
  const auto injector = stonks::di::MakeInjector(
      stonks::service::injectors::CreateNetworkRestsdkInjector(),
      stonks::service::injectors::CreateLogSpdlogInjector(),
      stonks::service::injectors::CreateKvdbAwsInjector(),

      // TODO(vh): Remove this because Ngrok always runs on local host?
      stonks::di::BindTypeToValue<std::string_view>("0.0.0.0"));

  app.Run([&injector, &expose_uri_interval, &reattempt_interval]() {
    return stonks::core::AppUriExposer{
        absl::Milliseconds(*expose_uri_interval),
        injector.template create<stonks::core::aue::Impl>(),
        absl::Milliseconds(*reattempt_interval)};
  });
}