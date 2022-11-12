#include "app_con_sdb_app.h"

#include <utility>

#include "app_json_user_conversions.h"
#include "app_sdb_i_app.h"
// #include "endpoints.h"
#include "network_json_base_conversions.h"
#include "network_json_common_conversions.h"
#include "network_rest_client_request_builder.h"
#include "network_te_endpoint_types_from.h"

namespace stonks::app::a {
template <>
auto Endpoint<&sdb::IApp::SelectAssets>::operator()()
    -> network::TypedEndpoint {
  return network::te::TypedEndpointFrom<network::Method::kGet, Function>();
}

auto Connection<sdb::IApp>::SelectAssets() const -> std::vector<core::Asset> {
  return rest_client_.Call(Endpoint<&sdb::IApp::SelectAssets>{}())
      .AndReceive<std::vector<core::Asset>>();
}

template <>
auto Handler<&sdb::IApp::SelectAssets>::operator()() -> Result {
  return parent_.SelectAssets();
}

template <>
auto Endpoint<&sdb::IApp::UpdateAssets>::operator()()
    -> network::TypedEndpoint {
  return network::te::TypedEndpointFrom<network::Method::kPost, Function>(
      network::te::Body{});
}

void Connection<sdb::IApp>::UpdateAssets(std::vector<core::Asset> assets) {
  rest_client_.Call(Endpoint<&sdb::IApp::UpdateAssets>{}())
      .WithBody(assets)
      .DiscardingResult();
}

template <>
auto Handler<&sdb::IApp::UpdateAssets>::operator()() -> Result {
  parent_.UpdateAssets(request_.Body());
}

template <>
auto Endpoint<&sdb::IApp::SelectSymbolsWithPriceRecords>::operator()()
    -> network::TypedEndpoint {
  return network::te::TypedEndpointFrom<network::Method::kPost, Function>();
}

auto Connection<sdb::IApp>::SelectSymbolsWithPriceRecords() const
    -> std::vector<core::Symbol> {
  return rest_client_
      .Call(Endpoint<&sdb::IApp::SelectSymbolsWithPriceRecords>{}())
      .AndReceive<std::vector<core::Symbol>>();
}

template <>
auto Handler<&sdb::IApp::SelectSymbolsWithPriceRecords>::operator()()
    -> Result {
  return parent_.SelectSymbolsWithPriceRecords();
}

template <>
auto Endpoint<&sdb::IApp::SelectSymbolInfo>::operator()()
    -> network::TypedEndpoint {
  return network::te::TypedEndpointFrom<network::Method::kPost, Function>(
      "symbol");
}

auto Connection<sdb::IApp>::SelectSymbolInfo(core::Symbol symbol) const
    -> cpp::Opt<core::SymbolInfo> {
  return rest_client_.Call(Endpoint<&sdb::IApp::SelectSymbolInfo>{}())
      .WithParam("symbol", symbol)
      .AndReceive<cpp::Opt<core::SymbolInfo>>();
}

template <>
auto Handler<&sdb::IApp::SelectSymbolInfo>::operator()() -> Result {
  return parent_.SelectSymbolInfo(request_.Body());
}

template <>
auto Endpoint<&sdb::IApp::SelectSymbolsInfo>::operator()()
    -> network::TypedEndpoint {
  return network::te::TypedEndpointFrom<network::Method::kPost, Function>();
}

auto Connection<sdb::IApp>::SelectSymbolsInfo() const
    -> std::vector<core::SymbolInfo> {
  return rest_client_.Call(Endpoint<&sdb::IApp::SelectSymbolsInfo>{}())
      .AndReceive<std::vector<core::SymbolInfo>>();
}

template <>
auto Handler<&sdb::IApp::SelectSymbolsInfo>::operator()() -> Result {
  return parent_.SelectSymbolsInfo();
}

template <>
auto Endpoint<&sdb::IApp::UpdateSymbolsInfo>::operator()()
    -> network::TypedEndpoint {
  return network::te::TypedEndpointFrom<network::Method::kPost, Function>(
      network::te::Body{});
}

void Connection<sdb::IApp>::UpdateSymbolsInfo(
    std::vector<core::SymbolInfo> infos) {
  rest_client_.Call(Endpoint<&sdb::IApp::UpdateSymbolsInfo>{}())
      .WithBody(infos)
      .DiscardingResult();
}

template <>
auto Handler<&sdb::IApp::UpdateSymbolsInfo>::operator()() -> Result {
  parent_.UpdateSymbolsInfo(request_.Body());
}

template <>
auto Endpoint<&sdb::IApp::SelectSymbolPriceRecords>::operator()()
    -> network::TypedEndpoint {
  // return network::te::TypedEndpointFrom<network::Method::kPost, Function>(
  //     "symbol", "start_time", "end_time", "limit");
  return {};
}

auto Connection<sdb::IApp>::SelectSymbolPriceRecords(
    const SelectSymbolPriceRecordsArgs &args) const
    -> std::vector<core::SymbolPriceRecord> {
  return rest_client_.Call(Endpoint<&sdb::IApp::SelectSymbolPriceRecords>{}())
      .WithParam("symbol", args.symbol)
      // .WithParam("start_time", args.start_time)
      // .WithParam("end_time", args.end_time)
      .WithParam("limit", args.limit)
      .AndReceive<std::vector<core::SymbolPriceRecord>>();
}

template <>
auto Handler<&sdb::IApp::SelectSymbolPriceRecords>::operator()() -> Result {
  return parent_.SelectSymbolPriceRecords(
      sdb::IApp::SelectSymbolPriceRecordsArgs{
          .symbol = request_.Param("symbol"),
          .start_time = request_.Param("start_time"),
          .end_time = request_.Param("end_time"),
          .limit = request_.Param("limit")});
}

template <>
auto Endpoint<&sdb::IApp::InsertSymbolPriceRecord>::operator()()
    -> network::TypedEndpoint {
  return network::te::TypedEndpointFrom<network::Method::kPost, Function>(
      network::te::Body{});
}

void Connection<sdb::IApp>::InsertSymbolPriceRecord(
    core::SymbolPriceRecord record) {
  rest_client_.Call(Endpoint<&sdb::IApp::InsertSymbolPriceRecord>{}())
      .WithBody(record)
      .DiscardingResult();
}

template <>
auto Handler<&sdb::IApp::InsertSymbolPriceRecord>::operator()() -> Result {
  parent_.InsertSymbolPriceRecord(request_.Body());
}

template <>
auto Endpoint<&sdb::IApp::DeleteSymbolPriceRecords>::operator()()
    -> network::TypedEndpoint {
  return network::te::TypedEndpointFrom<network::Method::kPost, Function>(
      "before_time");
}

void Connection<sdb::IApp>::DeleteSymbolPriceRecords(absl::Time before_time) {
  rest_client_
      .Call(Endpoint<&sdb::IApp::DeleteSymbolPriceRecords>{}())
      // .WithParam("before_time", before_time)
      .DiscardingResult();
}

template <>
auto Handler<&sdb::IApp::DeleteSymbolPriceRecords>::operator()() -> Result {
  parent_.DeleteSymbolPriceRecords(request_.Param("before_time"));
}
}  // namespace stonks::app::a