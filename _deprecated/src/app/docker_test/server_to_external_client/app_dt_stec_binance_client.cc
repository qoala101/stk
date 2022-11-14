#include "app_dt_stec_binance_client.h"

#include <string>
#include <utility>

#include "app_dt_stec_binance_endpoints.h"
#include "app_dt_stec_binance_types.h"
#include "app_dt_types.h"
#include "cpp_typed_struct.h"
#include "network_json_common_conversions.h"
#include "network_rest_client_request_builder.h"

namespace stonks::app::dt::stec::binance {
Client::Client(di::Factory<network::IRestRequestSender> request_sender_factory)
    : rest_client_{{"https://api.binance.com/api/v3"},
                   std::move(request_sender_factory)} {}

auto Client::GetCurrentAveragePrice(const Symbol &symbol) const -> AvgPrice {
  return rest_client_.Call(endpoints::GetCurrentAveragePrice())
      .WithParam("symbol", symbol)
      .AndReceive<AvgPrice>();
}
}  // namespace stonks::app::dt::stec::binance