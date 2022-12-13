#include "service_sdb_options.h"

namespace stonks::service::sdb {
Options::Options(cli::Options &options)
    : symbols_db_host{options.AddOption("--symbols_db_host", "0.0.0.0")},
      symbols_db_port{options.AddOption("--symbols_db_port", 6506)} {}

}  // namespace stonks::service::sdb