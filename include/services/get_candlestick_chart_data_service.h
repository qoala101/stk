#ifndef STONKS_SERVICES_GET_CANDLESTICK_CHART_DATA_SERVICE_H_
#define STONKS_SERVICES_GET_CANDLESTICK_CHART_DATA_SERVICE_H_

#include <cpprest/http_listener.h>
#include <pplx/pplxtasks.h>

namespace stonks {
class GetCandlestickChartDataService {
 public:
  pplx::task<void> Start();
  pplx::task<void> Stop();

 private:
  web::http::experimental::listener::http_listener http_listener_{};
};
}  // namespace stonks

#endif  // STONKS_SERVICES_GET_CANDLESTICK_CHART_DATA_SERVICE_H_
