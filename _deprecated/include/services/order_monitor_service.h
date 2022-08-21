#ifndef STONKS_SERVICES_ORDER_MONITOR_SERVICE_H_
#define STONKS_SERVICES_ORDER_MONITOR_SERVICE_H_

#include <cpprest/http_listener.h>
#include <pplx/pplxtasks.h>

#include "stonks_order_monitor.h"

namespace stonks {
class OrderMonitorService {
 public:
  explicit OrderMonitorService();

  pplx::task<void> Start();
  pplx::task<void> Stop();

 private:
  web::http::experimental::listener::http_listener http_listener_{};
  finance::OrderMonitor order_monitor_;
};
}  // namespace stonks

#endif  // STONKS_SERVICES_ORDER_MONITOR_SERVICE_H_
