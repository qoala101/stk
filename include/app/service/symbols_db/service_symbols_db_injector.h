#ifndef STONKS_APP_SERVICE_SYMBOLS_DB_SERVICE_SYMBOLS_DB_INJECTOR_H_
#define STONKS_APP_SERVICE_SYMBOLS_DB_SERVICE_SYMBOLS_DB_INJECTOR_H_

#include <fmt/format.h>

#include "core_i_symbols_db.h"
#include "di_bind_interface_to_implementation.h"
#include "di_bind_type_to_value.h"
#include "network_types.h"
#include "service_symbols_db.h"
#include "service_symbols_db_options.h"

namespace stonks::service {
inline auto CreateSymbolsDbInjector
    [[nodiscard]] (const SymbolsDbOptions &options) {
  return di::MakeInjector(
      stonks::di::BindTypeToValue<stonks::network::Uri>(stonks::network::Uri{
          fmt::format("http://{}:{}", *options.symbols_db_host,
                      *options.symbols_db_port)}),
      stonks::di::BindInterfaceToImplementation<stonks::core::ISymbolsDb,
                                                stonks::service::SymbolsDb>());
}
}  // namespace stonks::service

#endif  // STONKS_APP_SERVICE_SYMBOLS_DB_SERVICE_SYMBOLS_DB_INJECTOR_H_
