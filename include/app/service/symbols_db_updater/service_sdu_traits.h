#ifndef STONKS_APP_SERVICE_SYMBOLS_DB_UPDATER_SERVICE_SDU_TRAITS_H_
#define STONKS_APP_SERVICE_SYMBOLS_DB_UPDATER_SERVICE_SDU_TRAITS_H_

#include "core_i_symbols_db_updater.h"
#include "networkx_common.h"
#include "networkx_types.h"

namespace stonks::networkx {
template <>
struct EndpointFunctionTraits<
    &core::ISymbolsDbUpdater::GetUpdateSymbolsInfoInterval> {
  static constexpr auto kMethod = network::Method::kGet;
};

template <>
struct ClientServerTypeTraits<core::ISymbolsDbUpdater> {
  static constexpr auto kName = "symbols_db_updater";
  static constexpr auto kDefaultPort = "30003";
  static constexpr auto kEndpointFunctions = EndpointFunctionList(
      &core::ISymbolsDbUpdater::GetUpdateSymbolsInfoInterval);
};
}  // namespace stonks::networkx

#endif  // STONKS_APP_SERVICE_SYMBOLS_DB_UPDATER_SERVICE_SDU_TRAITS_H_
