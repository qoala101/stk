#ifndef STONKS_APP_SYMBOLS_DB_APP_SDB_APP_SERVER_H_
#define STONKS_APP_SYMBOLS_DB_APP_SDB_APP_SERVER_H_

#include "app_sdb_app.h"
#include "cpp_not_null.h"
#include "network_rest_server.h"
#include "network_rest_server_builder.h"

namespace stonks::app::sdb {
class AppServer {
 public:
  AppServer(App app, network::RestServerBuilder rest_server_builder);

 private:
  /**
   * @copydoc endpoints::InsertOrUpdateSymbolInfo
   */
  [[nodiscard]] static auto InsertOrUpdateSymbolInfo(cpp::NnSp<App> app);

  /**
   * @copydoc endpoints::InsertSymbolPriceRecord
   */
  [[nodiscard]] static auto InsertSymbolPriceRecord(cpp::NnSp<App> app);

  network::RestServer rest_server_;
};
}  // namespace stonks::app::sdb

#endif  // STONKS_APP_SYMBOLS_DB_APP_SDB_APP_SERVER_H_
