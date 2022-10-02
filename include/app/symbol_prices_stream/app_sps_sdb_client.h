#ifndef STONKS_APP_SYMBOL_PRICES_STREAM_APP_SPS_SDB_CLIENT_H_
#define STONKS_APP_SYMBOL_PRICES_STREAM_APP_SPS_SDB_CLIENT_H_

#include "core_types.h"
#include "network_rest_client.h"

namespace stonks::app::sps {
/**
 * @copydoc sdb::App
 */
class SdbClient {
 public:
  explicit SdbClient(network::RestClient rest_client);

  /**
   * @copydoc sdb::App::InsertSymbolPriceRecord
   */
  void InsertSymbolPriceRecord(core::SymbolPriceRecord record);

 private:
  network::RestClient rest_client_;
};
}  // namespace stonks::app::sps

#endif  // STONKS_APP_SYMBOL_PRICES_STREAM_APP_SPS_SDB_CLIENT_H_
