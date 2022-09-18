#include "app_dt_stec_pds_app_client.h"

#include <string>
#include <utility>

#include "app_dt_pds_app_endpoints.h"
#include "app_dt_stec_binance_endpoints.h"
#include "app_dt_stec_binance_types.h"
#include "app_dt_types.h"
#include "cpp_not_null.h"
#include "network_rest_client_request_builder.h"

namespace stonks::app::dt::stec {
PdsAppClient::PdsAppClient(network::RestClient rest_client)
    : rest_client_{std::move(rest_client)} {}

void PdsAppClient::RecordSymbolPrice(const SymbolPriceRecord &record) {
  return rest_client_.Call(pds::endpoints::RecordSymbolPrice())
      .WithBody(record)
      .DiscardingResult();
}
}  // namespace stonks::app::dt::stec