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
   * @copydoc endpoints::SelectAssets
   */
  static auto SelectAssets [[nodiscard]] (cpp::NnSp<App> app);

  /**
   * @copydoc endpoints::UpdateAssets
   */
  static auto UpdateAssets [[nodiscard]] (cpp::NnSp<App> app);

  /**
   * @copydoc endpoints::SelectSymbolsWithPriceRecords
   */
  static auto SelectSymbolsWithPriceRecords [[nodiscard]] (cpp::NnSp<App> app);

  /**
   * @copydoc endpoints::SelectSymbolInfo
   */
  static auto SelectSymbolInfo [[nodiscard]] (cpp::NnSp<App> app);

  /**
   * @copydoc endpoints::SelectSymbolsInfo
   */
  static auto SelectSymbolsInfo [[nodiscard]] (cpp::NnSp<App> app);

  /**
   * @copydoc endpoints::UpdateSymbolsInfo
   */
  static auto UpdateSymbolsInfo [[nodiscard]] (cpp::NnSp<App> app);

  /**
   * @copydoc endpoints::SelectSymbolPriceRecords
   */
  static auto SelectSymbolPriceRecords [[nodiscard]] (cpp::NnSp<App> app);

  /**
   * @copydoc endpoints::InsertSymbolPriceRecord
   */
  static auto InsertSymbolPriceRecord [[nodiscard]] (cpp::NnSp<App> app);

  /**
   * @copydoc endpoints::DeleteSymbolPriceRecords
   */
  static auto DeleteSymbolPriceRecords [[nodiscard]] (cpp::NnSp<App> app);

  network::RestServer rest_server_;
};
}  // namespace stonks::app::sdb

#endif  // STONKS_APP_SYMBOLS_DB_APP_SDB_APP_SERVER_H_
