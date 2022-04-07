#ifndef STONKS_SERVICES_ORDER_PROXY_SERVICE_H_
#define STONKS_SERVICES_ORDER_PROXY_SERVICE_H_

#include <cpprest/http_listener.h>
#include <pplx/pplxtasks.h>

#include "order_proxy.h"

namespace stonks {
class OrderProxyService {
 public:
  pplx::task<void> Start();
  pplx::task<void> Stop();

 private:
  web::http::experimental::listener::http_listener http_listener_{};
  finance::OrderProxy order_proxy_;
};
}  // namespace stonks

#endif  // STONKS_SERVICES_ORDER_PROXY_SERVICE_H_
