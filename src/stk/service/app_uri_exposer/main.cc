#include <absl/time/time.h>

#include <boost/di.hpp>
#include <cstdint>
#include <string>
#include <string_view>
#include <variant>

#include "cli_app.h"
#include "cli_option.h"
#include "cli_options.h"
#include "di_make_injector.h"
#include "service_app_uri_exposer.h"
#include "service_aue_impl.h"
#include "service_inj_kvdb_aws.h"
#include "service_inj_log_spdlog.h"
#include "service_inj_network_restsdk.h"

namespace vh::stk::service::aue {
void Main(int argc, const char *const *argv) {
  auto options = cli::Options{};

  const auto expose_uri_interval = options.AddOption(
      "--expose_uri_interval", absl::ToInt64Milliseconds(absl::Minutes(1)));
  const auto reattempt_interval = options.AddOption(
      "--reattempt_interval", absl::ToInt64Milliseconds(absl::Minutes(1)));

  const auto app = cli::App{argc, argv, options};
  const auto injector = di::MakeInjector(inj::CreateNetworkRestsdkInjector(),
                                         inj::CreateLogSpdlogInjector(),
                                         inj::CreateKvdbAwsInjector());

  app.Run([&injector, &expose_uri_interval, &reattempt_interval]() {
    return AppUriExposer{
        {.exposer = injector.template create<Impl>(),
         .expose_uri_interval = absl::Milliseconds(*expose_uri_interval),
         .reattempt_interval = absl::Milliseconds(*reattempt_interval)}};
  });
}
}  // namespace vh::stk::service::aue

auto main(int argc, const char *const *argv) -> int {
  vh::stk::service::aue::Main(argc, argv);
}