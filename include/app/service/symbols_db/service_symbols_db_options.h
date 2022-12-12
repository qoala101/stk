#ifndef STONKS_APP_SERVICE_SYMBOLS_DB_SERVICE_SYMBOLS_DB_OPTIONS_H_
#define STONKS_APP_SERVICE_SYMBOLS_DB_SERVICE_SYMBOLS_DB_OPTIONS_H_

#include <string>

#include "cli_option.h"
#include "cli_options.h"

namespace stonks::service {
/**
 * @brief Command line options required to connect to Symbols DB service.
 */
struct SymbolsDbOptions {
  explicit SymbolsDbOptions(cli::Options &options);

  cli::Option<std::string> symbols_db_host;
  cli::Option<int> symbols_db_port;
};
}  // namespace stonks::service

#endif  // STONKS_APP_SERVICE_SYMBOLS_DB_SERVICE_SYMBOLS_DB_OPTIONS_H_
