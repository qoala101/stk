#ifndef STONKS_APP_SYMBOLS_INFO_UPDATER_APP_SIU_SDB_APP_CLIENT_H_
#define STONKS_APP_SYMBOLS_INFO_UPDATER_APP_SIU_SDB_APP_CLIENT_H_

#include "core_types.h"
#include "network_rest_client.h"

namespace stonks::app::siu {
/**
 * @copydoc sdb::App
 */
class SdbAppClient {
 public:
  explicit SdbAppClient(network::RestClient rest_client);

  /**
   * @copydoc endpoints::InsertOrUpdateSymbolInfo
   */
  void InsertOrUpdateSymbolInfo(core::SymbolInfo info);

 private:
  network::RestClient rest_client_;
};
}  // namespace stonks::app::siu

#endif  // STONKS_APP_SYMBOLS_INFO_UPDATER_APP_SIU_SDB_APP_CLIENT_H_
