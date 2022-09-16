#include "app_dt_stec_binance_client.h"

#include <string>
#include <utility>

#include "app_dt_stec_binance_endpoints.h"
#include "app_dt_stec_binance_types.h"
#include "app_dt_stec_types.h"
#include "cpp_not_null.h"
#include "network_rest_client_request_builder.h"

namespace stonks::app::dt::stec::binance {
Client::Client(cpp::NnSp<network::IRestRequestSender> request_sender)
    : rest_client_{std::move(request_sender),
                   {"https://api.binance.com/api/v3"}} {}

auto Client::GetCurrentAveragePrice(const Symbol &symbol) const -> AvgPrice {
  return rest_client_.Call(endpoints::GetCurrentAveragePrice())
      .WithParam("symbol", symbol)
      .AndReceive<AvgPrice>();
}
}  // namespace stonks::app::dt::stec::binance