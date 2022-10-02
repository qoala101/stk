#include "app_sps_sdb_app_client.h"

#include <utility>

#include "app_endpoints.h"
#include "app_json_user_conversions.h"
#include "network_rest_client.h"
#include "network_rest_client_request_builder.h"

namespace stonks::app::sps {
SdbAppClient::SdbAppClient(network::RestClient rest_client)
    : rest_client_{std::move(rest_client)} {}

void SdbAppClient::InsertSymbolPriceRecord(core::SymbolPriceRecord record) {
  rest_client_.Call(endpoints::InsertSymbolPriceRecord())
      .WithBody(std::move(record))
      .DiscardingResult();
}
}  // namespace stonks::app::sps