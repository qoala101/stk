// #include "stonks_server_db.h"

// #include <functional>
// #include "ccutils_not_null.h"

// #include "network_endpoint_request_dispatcher.h"
// #include "network_json_basic_conversions.h"
// #include "network_types.h"
// #include "restsdk/restsdk_factory.h"
// #include "stonks_endpoints_db.h"
// #include "stonks_types.h"

// namespace stonks::server {
// Db::Db(ccutils::NnUp<IDb>> db, std::string_view local_uri,
//        const network::IFactory &network_factory)
//     : db_{std::move(db)},
//       request_receiver_{network_factory.CreateRestRequestReceiver(
//           local_uri,
//           network::EndpointRequestDispatcher{
//               {{endpoints::db::SelectAssets(),
//                 std::bind_front(&Db::SelectAssets, this)},
//                {endpoints::db::UpdateAssets(),
//                 std::bind_front(&Db::UpdateAssets, this)},
//                {endpoints::db::SelectSymbols(),
//                 std::bind_front(&Db::SelectSymbols, this)},
//                {endpoints::db::SelectSymbolsInfo(),
//                 std::bind_front(&Db::SelectSymbolsInfo, this)},
//                {endpoints::db::UpdateSymbolsInfo(),
//                 std::bind_front(&Db::UpdateSymbolsInfo, this)},
//                {endpoints::db::SelectSymbolPriceTicks(),
//                 std::bind_front(&Db::SelectSymbolPriceTicks, this)},
//                {endpoints::db::InsertSymbolPriceTick(),
//                 std::bind_front(&Db::InsertSymbolPriceTick, this)}}})} {}

// void Db::SelectAssets(network::Request &request) {
//   request.Return(db_->SelectAssets());
// }

// void Db::UpdateAssets(network::Request &request) {
//   db_->UpdateAssets(request.Body());
// }

// void Db::SelectSymbols(network::Request &request) {
//   request.Return(db_->SelectSymbols());
// }

// void Db::SelectSymbolsInfo(network::Request &request) {
//   request.Return(db_->SelectSymbolsInfo());
// }

// void Db::UpdateSymbolsInfo(network::Request &request) {
//   db_->UpdateSymbolsInfo(request.Body());
// }

// void Db::SelectSymbolPriceTicks(network::Request &request) {
//   // const auto symbol =
//   //     network::ParseFromJson<std::optional<SymbolName>>(params.at("symbol"));
//   // const auto period =
//   //     network::ParseFromJson<std::optional<Period>>(params.at("period"));
//   // const auto limit =
//   //     network::ParseFromJson<std::optional<int>>(params.at("limit"));

//   // return network::ConvertToJson(db_->SelectSymbolPriceTicks(
//   //     symbol ? &symbol : nullptr, period ? &period : nullptr,
//   //     limit ? &limit : nullptr));
// }

// void Db::InsertSymbolPriceTick(network::Request &request) {
//   db_->InsertSymbolPriceTick(request.Body());
// }
// }  // namespace stonks::server