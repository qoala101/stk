#ifndef VH_STK_SERVICE_SDB_TRAITS_H_
#define VH_STK_SERVICE_SDB_TRAITS_H_

#include "core_i_symbols_db.h"
#include "networkx_common.h"
#include "networkx_types.h"

namespace vh::networkx {
template <>
struct EndpointFunctionTraits<&stk::core::ISymbolsDb::SelectAssets> {
  static constexpr auto kMethod = network::Method::kGet;
};

template <>
struct EndpointFunctionTraits<&stk::core::ISymbolsDb::UpdateAssets> {
  static constexpr auto kMethod = network::Method::kPost;
  static constexpr auto kParams = ParamList(RequestBody{});
};

template <>
struct EndpointFunctionTraits<
    &stk::core::ISymbolsDb::SelectSymbolsWithPriceRecords> {
  static constexpr auto kMethod = network::Method::kGet;
};

template <>
struct EndpointFunctionTraits<&stk::core::ISymbolsDb::SelectSymbolInfo> {
  static constexpr auto kMethod = network::Method::kGet;
  static constexpr auto kParams = ParamList("symbol");
};

template <>
struct EndpointFunctionTraits<&stk::core::ISymbolsDb::SelectSymbolsInfo> {
  static constexpr auto kMethod = network::Method::kGet;
};

template <>
struct EndpointFunctionTraits<&stk::core::ISymbolsDb::UpdateSymbolsInfo> {
  static constexpr auto kMethod = network::Method::kPost;
  static constexpr auto kParams = ParamList(RequestBody{});
};

template <>
struct EndpointFunctionTraits<&stk::core::ISymbolsDb::SelectSymbolPriceRecords> {
  static constexpr auto kMethod = network::Method::kGet;
  static constexpr auto kParams =
      ParamList("symbol", "order", "start_time", "end_time", "limit");
};

template <>
struct EndpointFunctionTraits<&stk::core::ISymbolsDb::InsertSymbolPriceRecord> {
  static constexpr auto kMethod = network::Method::kPost;
  static constexpr auto kParams = ParamList(RequestBody{});
};

template <>
struct EndpointFunctionTraits<&stk::core::ISymbolsDb::DeleteSymbolPriceRecords> {
  static constexpr auto kMethod = network::Method::kDelete;
  static constexpr auto kParams = ParamList("start_time", "end_time");
};

template <>
struct ClientServerTypeTraits<stk::core::ISymbolsDb> {
  static constexpr auto kName = "symbols_db";
  static constexpr auto kDefaultPort = "30000";
  static constexpr auto kEndpointFunctions = EndpointFunctionList(
      &stk::core::ISymbolsDb::SelectAssets, &stk::core::ISymbolsDb::UpdateAssets,
      &stk::core::ISymbolsDb::SelectSymbolsWithPriceRecords,
      &stk::core::ISymbolsDb::SelectSymbolInfo, &stk::core::ISymbolsDb::SelectSymbolsInfo,
      &stk::core::ISymbolsDb::UpdateSymbolsInfo,
      &stk::core::ISymbolsDb::SelectSymbolPriceRecords,
      &stk::core::ISymbolsDb::InsertSymbolPriceRecord,
      &stk::core::ISymbolsDb::DeleteSymbolPriceRecords);
};
}  // namespace vh::networkx

#endif  // VH_STK_SERVICE_SDB_TRAITS_H_
