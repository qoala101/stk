#ifndef STONKS_APP_SERVICE_SYMBOL_PRICES_STREAMS_CONTROLLER_SERVICE_SPSC_OPTIONS_H_
#define STONKS_APP_SERVICE_SYMBOL_PRICES_STREAMS_CONTROLLER_SERVICE_SPSC_OPTIONS_H_

#include <string>

#include "cli_option.h"
#include "cli_options.h"

namespace stonks::service::spsc {
/**
 * @brief Command line options required to connect to symbol prices streams
 * controller service.
 */
struct Options {
  explicit Options(cli::Options &options);

  cli::Option<std::string> symbol_prices_streams_controller_host;
  cli::Option<int> symbol_prices_streams_controller_port;
};
}  // namespace stonks::service::spsc

#endif  // STONKS_APP_SERVICE_SYMBOL_PRICES_STREAMS_CONTROLLER_SERVICE_SPSC_OPTIONS_H_
