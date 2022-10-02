#include "app_sps_sdb_client.h"

#include "app_endpoints.h"
#include "app_json_user_conversions.h"
#include "network_rest_client.h"

namespace stonks::app::sps {
SdbClient::SdbClient(network::RestClient rest_client)
    : rest_client_{std::move(rest_client)} {}

void SdbClient::InsertSymbolPriceRecord(const core::SymbolPriceRecord &record) {
  rest_client_.Call(endpoints::InsertSymbolPriceRecord())
      .WithBody(record)
      .DiscardingResult();
}
}  // namespace stonks::app::sps