#ifndef STONKS_SERVICES_PRICE_TICKS_SERVICE_H_
#define STONKS_SERVICES_PRICE_TICKS_SERVICE_H_

#include <cpprest/http_listener.h>
#include <pplx/pplxtasks.h>

#include <atomic>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "finance_db.h"
#include "finance_types.h"

namespace stonks {
class PriceTicksService {
 public:
  explicit PriceTicksService();

  auto Start() -> pplx::task<void>;
  auto Stop() -> pplx::task<void>;

 private:
  web::http::experimental::listener::http_listener http_listener_{};

  std::vector<finance::PriceTicksServiceSubscribeRequest> subscribers_{};
  mutable std::mutex subscribers_mutex_{};

  std::thread thread_{};
  finance::FinanceDb finance_db_{};

  std::atomic_bool keep_alive_{};
};
}  // namespace stonks

#endif  // STONKS_SERVICES_PRICE_TICKS_SERVICE_H_
