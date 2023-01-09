#ifndef STONKS_APP_SERVICE_SYMBOLS_DB_UPDATER_SERVICE_SYMBOLS_DB_UPDATER_H_
#define STONKS_APP_SERVICE_SYMBOLS_DB_UPDATER_SERVICE_SYMBOLS_DB_UPDATER_H_

#include <absl/time/time.h>

#include <cppcoro/task.hpp>

#include "core_i_symbols_db_updater.h"
#include "service_client_base.h"
#include "service_sdu_traits.h"  // IWYU pragma: keep

namespace stonks::service {
/**
 * @copydoc core::ISymbolsDbUpdater
 */
class SymbolsDbUpdater : public ClientBase<core::ISymbolsDbUpdater> {
 public:
  using ClientBase::ClientBase;

  /**
   * @copydoc core::ISymbolsDbUpdater::GetUpdateSymbolsInfoInterval
   */
  auto GetUpdateSymbolsInfoInterval [[nodiscard]] () const
      -> cppcoro::task<absl::Duration> override;
};
}  // namespace stonks::service

#endif  // STONKS_APP_SERVICE_SYMBOLS_DB_UPDATER_SERVICE_SYMBOLS_DB_UPDATER_H_
