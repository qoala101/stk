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
  [[nodiscard]] static auto SelectAssets(cpp::NnSp<App> app);

  /**
   * @copydoc endpoints::UpdateAssets
   */
  [[nodiscard]] static auto UpdateAssets(cpp::NnSp<App> app);

  /**
   * @copydoc endpoints::SelectSymbolsWithPriceRecords
   */
  [[nodiscard]] static auto SelectSymbolsWithPriceRecords(cpp::NnSp<App> app);

  /**
   * @copydoc endpoints::SelectSymbolInfo
   */
  [[nodiscard]] static auto SelectSymbolInfo(cpp::NnSp<App> app);

  /**
   * @copydoc endpoints::SelectSymbolsInfo
   */
  [[nodiscard]] static auto SelectSymbolsInfo(cpp::NnSp<App> app);

  /**
   * @copydoc endpoints::UpdateSymbolsInfo
   */
  [[nodiscard]] static auto UpdateSymbolsInfo(cpp::NnSp<App> app);

  /**
   * @copydoc endpoints::SelectSymbolPriceRecords
   */
  [[nodiscard]] static auto SelectSymbolPriceRecords(cpp::NnSp<App> app);

  /**
   * @copydoc endpoints::InsertSymbolPriceRecord
   */
  [[nodiscard]] static auto InsertSymbolPriceRecord(cpp::NnSp<App> app);

  /**
   * @copydoc endpoints::DeleteSymbolPriceRecords
   */
  [[nodiscard]] static auto DeleteSymbolPriceRecords(cpp::NnSp<App> app);

  network::RestServer rest_server_;
};
}  // namespace stonks::app::sdb

#endif  // STONKS_APP_SYMBOLS_DB_APP_SDB_APP_SERVER_H_
