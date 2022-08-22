#ifndef STONKS_STONKS_SERVERS_STONKS_SERVER_DB_H_
#define STONKS_STONKS_SERVERS_STONKS_SERVER_DB_H_

#include <polymorphic_value.h>

#include <map>
#include "ccutils_not_null.h"

#include "network_endpoint_request_dispatcher.h"
#include "network_enums.h"
#include "network_i_factory.h"
#include "network_i_json.h"
#include "network_i_rest_request_receiver.h"
#include "network_types.h"
#include "not_null.hpp"
#include "stonks_i_db.h"

namespace stonks::server {
class Db {
 public:
  explicit Db(ccutils::NnUp<IDb> db,
              std::string_view local_uri,
              const network::IFactory &network_factory);

  // std::variant<std::function<void(NoData)>,
  //              std::function<void(NoReplyRequest &)>,
  //              std::function<AutoConvert(NoDataExpectsReply)>,
  //              std::function<AutoConvert(ExpectsReply &)>>
  //     handler_{};

 private:
  auto SelectAssets(network::RequestData &request) -> std::vector<std::string>;
  void UpdateAssets(network::RequestData &request);
  auto SelectSymbols(network::RequestData &request) -> std::vector<SymbolName>;
  auto SelectSymbolsInfo(network::RequestData &request)
      -> std::vector<SymbolInfo>;
  void UpdateSymbolsInfo(network::RequestData &request);
  auto SelectSymbolPriceTicks(network::RequestData &request)
      -> std::vector<SymbolPriceTick>;
  void InsertSymbolPriceTick(network::RequestData &request);

  ccutils::NnUp<IDb> db_;
  ccutils::NnUp<network::IRestRequestReceiver>
      request_receiver_;
};
}  // namespace stonks::server

#endif  // STONKS_STONKS_SERVERS_STONKS_SERVER_DB_H_
