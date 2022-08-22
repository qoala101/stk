#ifndef STONKS_STONKS_SERVERS_STONKS_CLIENT_DB_H_
#define STONKS_STONKS_SERVERS_STONKS_CLIENT_DB_H_

#include <polymorphic_value.h>

#include <map>
#include "ccutils_not_null.h"

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

 private:
  auto SelectAssets(network::Params params, network::Body body)
      -> network::Result;
  auto UpdateAssets(network::Params params, network::Body body)
      -> network::Result;
  auto SelectSymbols(network::Params params, network::Body body)
      -> network::Result;
  auto SelectSymbolsInfo(network::Params params, network::Body body)
      -> network::Result;
  auto UpdateSymbolsInfo(network::Params params, network::Body body)
      -> network::Result;
  auto SelectSymbolPriceTicks(network::Params params, network::Body body)
      -> network::Result;
  auto InsertSymbolPriceTick(network::Params params, network::Body body)
      -> network::Result;

  ccutils::NnUp<IDb> db_;
  ccutils::NnUp<network::IRestRequestReceiver>
      request_handler_;
};
}  // namespace stonks::server

#endif  // STONKS_STONKS_SERVERS_STONKS_CLIENT_DB_H_
