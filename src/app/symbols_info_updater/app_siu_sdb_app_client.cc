#include "app_siu_sdb_app_client.h"

#include <utility>

#include "app_endpoints.h"
#include "app_json_user_conversions.h"
#include "core_types.h"
#include "network_rest_client.h"
#include "network_rest_client_request_builder.h"

namespace stonks::app::siu {
SdbAppClient::SdbAppClient(network::RestClient rest_client)
    : rest_client_{std::move(rest_client)} {}

void SdbAppClient::InsertOrUpdateSymbolInfo(core::SymbolInfo info) {
  rest_client_.Call(endpoints::InsertOrUpdateSymbolInfo())
      .WithBody(std::move(info))
      .DiscardingResult();
}
}  // namespace stonks::app::siu