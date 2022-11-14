#include "service_symbols_db.h"

#include <fmt/core.h>

#include <string>
#include <utility>

#include "network_auto_parsable.h"
#include "network_enums.h"
#include "network_json_base_conversions.h"
#include "network_json_common_conversions.h"
#include "network_json_conversions_facades.h"
#include "network_rest_client_request_builder.h"
#include "network_te_endpoint_types_from.h"
#include "service_json_conversions.h"

namespace stonks::service {
template <>
auto Endpoint<&core::ISymbolsDb::SelectAssets>::operator()()
    -> network::TypedEndpoint {
  return network::te::TypedEndpointFrom<network::Method::kGet, Function>();
}

auto Connection<core::ISymbolsDb>::SelectAssets() const
    -> std::vector<core::Asset> {
  return rest_client_.Call(Endpoint<&core::ISymbolsDb::SelectAssets>{}())
      .AndReceive<std::vector<core::Asset>>();
}

template <>
auto Handler<&core::ISymbolsDb::SelectAssets>::operator()() -> Result {
  return parent_.SelectAssets();
}

template <>
auto Endpoint<&core::ISymbolsDb::UpdateAssets>::operator()()
    -> network::TypedEndpoint {
  return network::te::TypedEndpointFrom<network::Method::kPost, Function>(
      network::te::Body{});
}

void Connection<core::ISymbolsDb>::UpdateAssets(
    std::vector<core::Asset> assets) {
  rest_client_.Call(Endpoint<&core::ISymbolsDb::UpdateAssets>{}())
      .WithBody(assets)
      .DiscardingResult();
}

template <>
auto Handler<&core::ISymbolsDb::UpdateAssets>::operator()() -> Result {
  parent_.UpdateAssets(request_.Body());
}

template <>
auto Endpoint<&core::ISymbolsDb::SelectSymbolsWithPriceRecords>::operator()()
    -> network::TypedEndpoint {
  return network::te::TypedEndpointFrom<network::Method::kPost, Function>();
}

auto Connection<core::ISymbolsDb>::SelectSymbolsWithPriceRecords() const
    -> std::vector<core::Symbol> {
  return rest_client_
      .Call(Endpoint<&core::ISymbolsDb::SelectSymbolsWithPriceRecords>{}())
      .AndReceive<std::vector<core::Symbol>>();
}

template <>
auto Handler<&core::ISymbolsDb::SelectSymbolsWithPriceRecords>::operator()()
    -> Result {
  return parent_.SelectSymbolsWithPriceRecords();
}

template <>
auto Endpoint<&core::ISymbolsDb::SelectSymbolInfo>::operator()()
    -> network::TypedEndpoint {
  return network::te::TypedEndpointFrom<network::Method::kPost, Function>(
      "symbol");
}

auto Connection<core::ISymbolsDb>::SelectSymbolInfo(core::Symbol symbol) const
    -> cpp::Opt<core::SymbolInfo> {
  return rest_client_.Call(Endpoint<&core::ISymbolsDb::SelectSymbolInfo>{}())
      .WithParam("symbol", symbol)
      .AndReceive<cpp::Opt<core::SymbolInfo>>();
}

template <>
auto Handler<&core::ISymbolsDb::SelectSymbolInfo>::operator()() -> Result {
  return parent_.SelectSymbolInfo(request_.Body());
}

template <>
auto Endpoint<&core::ISymbolsDb::SelectSymbolsInfo>::operator()()
    -> network::TypedEndpoint {
  return network::te::TypedEndpointFrom<network::Method::kPost, Function>();
}

auto Connection<core::ISymbolsDb>::SelectSymbolsInfo() const
    -> std::vector<core::SymbolInfo> {
  return rest_client_.Call(Endpoint<&core::ISymbolsDb::SelectSymbolsInfo>{}())
      .AndReceive<std::vector<core::SymbolInfo>>();
}

template <>
auto Handler<&core::ISymbolsDb::SelectSymbolsInfo>::operator()() -> Result {
  return parent_.SelectSymbolsInfo();
}

template <>
auto Endpoint<&core::ISymbolsDb::UpdateSymbolsInfo>::operator()()
    -> network::TypedEndpoint {
  return network::te::TypedEndpointFrom<network::Method::kPost, Function>(
      network::te::Body{});
}

void Connection<core::ISymbolsDb>::UpdateSymbolsInfo(
    std::vector<core::SymbolInfo> infos) {
  rest_client_.Call(Endpoint<&core::ISymbolsDb::UpdateSymbolsInfo>{}())
      .WithBody(infos)
      .DiscardingResult();
}

template <>
auto Handler<&core::ISymbolsDb::UpdateSymbolsInfo>::operator()() -> Result {
  parent_.UpdateSymbolsInfo(request_.Body());
}

template <>
auto Endpoint<&core::ISymbolsDb::SelectSymbolPriceRecords>::operator()()
    -> network::TypedEndpoint {
  // return network::te::TypedEndpointFrom<network::Method::kPost, Function>(
  //     "symbol", "start_time", "end_time", "limit");
  return {};
}

auto Connection<core::ISymbolsDb>::SelectSymbolPriceRecords(
    const SelectSymbolPriceRecordsArgs &args) const
    -> std::vector<core::SymbolPriceRecord> {
  return rest_client_
      .Call(Endpoint<&core::ISymbolsDb::SelectSymbolPriceRecords>{}())
      .WithParam("symbol", args.symbol)
      // .WithParam("start_time", args.start_time)
      // .WithParam("end_time", args.end_time)
      .WithParam("limit", args.limit)
      .AndReceive<std::vector<core::SymbolPriceRecord>>();
}

template <>
auto Handler<&core::ISymbolsDb::SelectSymbolPriceRecords>::operator()()
    -> Result {
  return parent_.SelectSymbolPriceRecords(
      core::ISymbolsDb::SelectSymbolPriceRecordsArgs{
          .symbol = request_.Param("symbol"),
          .start_time = request_.Param("start_time"),
          .end_time = request_.Param("end_time"),
          .limit = request_.Param("limit")});
}

template <>
auto Endpoint<&core::ISymbolsDb::InsertSymbolPriceRecord>::operator()()
    -> network::TypedEndpoint {
  return network::te::TypedEndpointFrom<network::Method::kPost, Function>(
      network::te::Body{});
}

void Connection<core::ISymbolsDb>::InsertSymbolPriceRecord(
    core::SymbolPriceRecord record) {
  rest_client_.Call(Endpoint<&core::ISymbolsDb::InsertSymbolPriceRecord>{}())
      .WithBody(record)
      .DiscardingResult();
}

template <>
auto Handler<&core::ISymbolsDb::InsertSymbolPriceRecord>::operator()()
    -> Result {
  parent_.InsertSymbolPriceRecord(request_.Body());
}

template <>
auto Endpoint<&core::ISymbolsDb::DeleteSymbolPriceRecords>::operator()()
    -> network::TypedEndpoint {
  return network::te::TypedEndpointFrom<network::Method::kPost, Function>(
      "before_time");
}

void Connection<core::ISymbolsDb>::DeleteSymbolPriceRecords(
    absl::Time before_time) {
  rest_client_
      .Call(Endpoint<&core::ISymbolsDb::DeleteSymbolPriceRecords>{}())
      // .WithParam("before_time", before_time)
      .DiscardingResult();
}

template <>
auto Handler<&core::ISymbolsDb::DeleteSymbolPriceRecords>::operator()()
    -> Result {
  parent_.DeleteSymbolPriceRecords(request_.Param("before_time"));
}
}  // namespace stonks::service