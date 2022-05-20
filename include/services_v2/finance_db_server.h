#ifndef STONKS_SERVICES_V2_FINANCE_DB_SERVER_H_
#define STONKS_SERVICES_V2_FINANCE_DB_SERVER_H_

#include <string_view>
#include "finance_db.h"
#include "server.h"

namespace stonks::finance {
class FinanceDbServer : public stonks::network::Server {
 public:
  explicit FinanceDbServer(std::string_view base_uri, std::string_view db_uri);

 private:
  [[nodiscard]] auto SelectAssetsHandler() -> stonks::network::EndpointHandler;
  [[nodiscard]] auto SelectSymbolsHandler() -> stonks::network::EndpointHandler;
  [[nodiscard]] auto InsertSymbolPriceTickHandler()
      -> stonks::network::EndpointHandler;
  [[nodiscard]] auto SelectSymbolPriceTicksHandler()
      -> stonks::network::EndpointHandler;

  FinanceDb finance_db_{};
};
}  // namespace stonks::finance

#endif  // STONKS_SERVICES_V2_FINANCE_DB_SERVER_H_
