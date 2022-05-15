#ifndef STONKS_SERVICES_MEAN_AVERAGE_STRATEGY_SERVICE_H_
#define STONKS_SERVICES_MEAN_AVERAGE_STRATEGY_SERVICE_H_

#include <cpprest/http_listener.h>
#include <pplx/pplxtasks.h>

#include <atomic>
#include <thread>

#include "finance_types.h"
#include "mean_average_strategy.h"

namespace stonks {
class MeanAverageStrategyService {
 public:
  explicit MeanAverageStrategyService(finance::Symbol symbol,
                                      double base_precision, double price_precision, double comission,
                                      double profit);

  auto Start() -> pplx::task<void>;
  auto Stop() -> pplx::task<void>;

 private:
  const finance::Symbol symbol_{};
  std::thread thread_{};
  std::atomic_bool service_state_{};
  finance::MeanAverageStrategy strategy_;
  web::http::experimental::listener::http_listener http_listener_{};
};
}  // namespace stonks

#endif  // STONKS_SERVICES_MEAN_AVERAGE_STRATEGY_SERVICE_H_
