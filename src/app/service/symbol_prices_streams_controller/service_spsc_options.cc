#include "service_spsc_options.h"

namespace stonks::service::spsc {
Options::Options(cli::Options &options)
    : symbol_prices_streams_controller_host{options.AddOption(
          "--symbol_prices_streams_controller_host", "0.0.0.0")},
      symbol_prices_streams_controller_port{options.AddOption(
          "--symbol_prices_streams_controller_port", 30002)} {}

}  // namespace stonks::service::spsc