#ifndef STONKS_SERVICES_BREAKOUT_STRATEGY_SERVICE_H_
#define STONKS_SERVICES_BREAKOUT_STRATEGY_SERVICE_H_

#include <pplx/pplxtasks.h>

#include <thread>

#include "finance_types.h"

namespace stonks {
class BreakoutStrategyService {
 public:
  explicit BreakoutStrategyService(finance::Symbol symbol);

  pplx::task<void> Start();
  pplx::task<void> Stop();

 private:
  finance::Symbol symbol_;
  std::thread thread_{};
  bool service_state_{};
};
}  // namespace stonks

#endif  // STONKS_SERVICES_BREAKOUT_STRATEGY_SERVICE_H_
