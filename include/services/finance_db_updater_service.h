#ifndef STONKS_SERVICES_FINANCE_DB_UPDATER_SERVICE_H_
#define STONKS_SERVICES_FINANCE_DB_UPDATER_SERVICE_H_

#include <cpprest/http_listener.h>
#include <pplx/pplxtasks.h>

#include "finance_db.h"

namespace stonks {
class FinanceDbUpdaterService {
 public:
  explicit FinanceDbUpdaterService();

  auto Start() -> pplx::task<void>;
  auto Stop() -> pplx::task<void>;

 private:
  web::http::experimental::listener::http_listener http_listener_{};

  finance::FinanceDb finance_db_{};
};
}  // namespace stonks

#endif  // STONKS_SERVICES_FINANCE_DB_UPDATER_SERVICE_H_
