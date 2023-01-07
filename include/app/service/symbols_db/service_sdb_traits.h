#ifndef STONKS_APP_SERVICE_SYMBOLS_DB_SERVICE_SDB_TRAITS_H_
#define STONKS_APP_SERVICE_SYMBOLS_DB_SERVICE_SDB_TRAITS_H_

#include "core_i_symbols_db.h"
#include "networkx_common.h"
#include "networkx_types.h"

namespace stonks::networkx {
template <>
struct EndpointFunctionTraits<&core::ISymbolsDb::SelectAssets> {
  static constexpr auto kMethod = network::Method::kGet;
};

template <>
struct EndpointFunctionTraits<&core::ISymbolsDb::UpdateAssets> {
  static constexpr auto kMethod = network::Method::kPost;
  static constexpr auto kParams = ParamList(RequestBody{});
};

template <>
struct EndpointFunctionTraits<&core::ISymbolsDb::SelectSymbolInfo> {
  static constexpr auto kMethod = network::Method::kGet;
  static constexpr auto kParams = ParamList("symbol");
};

template <>
struct EndpointFunctionTraits<&core::ISymbolsDb::SelectSymbolsInfo> {
  static constexpr auto kMethod = network::Method::kGet;
};

template <>
struct EndpointFunctionTraits<&core::ISymbolsDb::UpdateSymbolsInfo> {
  static constexpr auto kMethod = network::Method::kPost;
  static constexpr auto kParams = ParamList(RequestBody{});
};

template <>
struct EndpointFunctionTraits<&core::ISymbolsDb::SelectSymbolPriceRecords> {
  static constexpr auto kMethod = network::Method::kGet;
  static constexpr auto kParams =
      ParamList("symbol", "order", "start_time", "end_time", "limit");
};

template <>
struct EndpointFunctionTraits<&core::ISymbolsDb::InsertSymbolPriceRecord> {
  static constexpr auto kMethod = network::Method::kPost;
  static constexpr auto kParams = ParamList(RequestBody{});
};

template <>
struct EndpointFunctionTraits<&core::ISymbolsDb::DeleteSymbolPriceRecords> {
  static constexpr auto kMethod = network::Method::kDelete;
  static constexpr auto kParams = ParamList("start_time", "end_time");
};

template <>
struct ClientServerTypeTraits<core::ISymbolsDb> {
  static constexpr auto kName = "symbols_db";
  static constexpr auto kDefaultPort = "30000";
  static constexpr auto kSynchronized = true;
  static constexpr auto kEndpointFunctions = EndpointFunctionList(
      &core::ISymbolsDb::SelectAssets, &core::ISymbolsDb::UpdateAssets,
      &core::ISymbolsDb::SelectSymbolInfo, &core::ISymbolsDb::SelectSymbolsInfo,
      &core::ISymbolsDb::UpdateSymbolsInfo,
      &core::ISymbolsDb::SelectSymbolPriceRecords,
      &core::ISymbolsDb::InsertSymbolPriceRecord,
      &core::ISymbolsDb::DeleteSymbolPriceRecords);
};
}  // namespace stonks::networkx

#endif  // STONKS_APP_SERVICE_SYMBOLS_DB_SERVICE_SDB_TRAITS_H_
