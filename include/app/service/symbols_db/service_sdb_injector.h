#ifndef STONKS_APP_SERVICE_SYMBOLS_DB_SERVICE_SDB_INJECTOR_H_
#define STONKS_APP_SERVICE_SYMBOLS_DB_SERVICE_SDB_INJECTOR_H_

#include <fmt/format.h>

#include "core_i_symbols_db.h"
#include "di_bind_interface_to_implementation.h"
#include "di_bind_type_to_value.h"
#include "network_types.h"
#include "service_sdb_options.h"
#include "service_symbols_db.h"

namespace stonks::service::sdb {
inline auto CreateInjector [[nodiscard]] (const Options &options) {
  return di::MakeInjector(
      stonks::di::BindTypeToValue<stonks::networkx::Uri<core::ISymbolsDb>>(
          stonks::networkx::Uri<core::ISymbolsDb>{
              fmt::format("http://{}:{}", *options.symbols_db_host,
                          *options.symbols_db_port)}),
      stonks::di::BindInterfaceToImplementation<stonks::core::ISymbolsDb,
                                                stonks::service::SymbolsDb>());
}
}  // namespace stonks::service::sdb

#endif  // STONKS_APP_SERVICE_SYMBOLS_DB_SERVICE_SDB_INJECTOR_H_
