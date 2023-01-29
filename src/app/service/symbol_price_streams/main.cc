#include <absl/time/time.h>

#include <boost/di.hpp>
#include <cstdint>
#include <iostream>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>
#include <string>
#include <variant>

#include "cli_app.h"
#include "cli_option.h"
#include "cli_options.h"
#include "core_i_symbols_db.h"
#include "core_symbol_price_streams.h"
#include "cpp_not_null.h"
#include "cpp_share.h"
#include "di_auto_injectable.h"
#include "di_bind_value_type_to_value.h"
#include "di_make_injector.h"
#include "service_client_options.h"
#include "service_inj_client_server.h"
#include "service_inj_log_spdlog.h"
#include "service_inj_network_restsdk.h"
#include "service_inj_symbols_db_client.h"
#include "service_sdb_traits.h"  // IWYU pragma: keep

namespace stonks::service::sps {
void Main(int argc, const char *const *argv) {
  auto options = cli::Options{};

  const auto symbols_db_client_options =
      ClientOptions<core::ISymbolsDb>{options};
  auto symbols = options.AddOption(
      "--symbols", std::vector<std::string>{"BTCUSDT", "ETHUSDT", "BNBUSDT"});
  const auto reattempt_interval = options.AddOption(
      "--reattempt_interval", absl::ToInt64Milliseconds(absl::Minutes(1)));

  const auto app = cli::App{argc, argv, options};
  const auto injector = cpp::Share(di::MakeInjector(
      inj::CreateNetworkRestsdkInjector(), inj::CreateLogSpdlogInjector(),
      inj::CreateSymbolsDbClientInjector(symbols_db_client_options),
      di::BindValueTypeToValue(absl::Milliseconds(*reattempt_interval))));

  app.Run([&injector, &symbols]() {
    auto auto_injectable = di::AutoInjectable{injector};

    return core::SymbolPriceStreams{
        *symbols | ranges::views::transform([](auto &symbol) {
          return core::Symbol{std::move(symbol)};
        }) | ranges::to_vector,
        auto_injectable, auto_injectable, auto_injectable};
  });
}
}  // namespace stonks::service::sps

auto main(int argc, const char *const *argv) -> int {
  stonks::service::sps::Main(argc, argv);
}