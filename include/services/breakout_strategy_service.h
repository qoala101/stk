#ifndef STONKS_SERVICES_BREAKOUT_STRATEGY_SERVICE_H_
#define STONKS_SERVICES_BREAKOUT_STRATEGY_SERVICE_H_

#include <pplx/pplxtasks.h>

#include <thread>

namespace stonks {
class BreakoutStrategyService {
 public:
  pplx::task<void> Start();
  pplx::task<void> Stop();

 private:
  std::thread thread_{};
  bool service_state_{};
};
}  // namespace stonks

#endif  // STONKS_SERVICES_BREAKOUT_STRATEGY_SERVICE_H_
