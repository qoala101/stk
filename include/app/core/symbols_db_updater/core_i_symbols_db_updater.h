#ifndef STONKS_APP_CORE_SYMBOLS_DB_UPDATER_CORE_I_SYMBOLS_DB_UPDATER_H_
#define STONKS_APP_CORE_SYMBOLS_DB_UPDATER_CORE_I_SYMBOLS_DB_UPDATER_H_

#include <absl/time/time.h>

#include <cppcoro/task.hpp>

namespace stonks::core {
/**
 * @brief Periodically updates Symbols DB.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class ISymbolsDbUpdater {
 public:
  virtual ~ISymbolsDbUpdater() = default;

  /**
   * @brief Gives interval at which symbols info is updated.
   */
  virtual auto GetUpdateSymbolsInfoInterval [[nodiscard]] () const
      -> cppcoro::task<absl::Duration> = 0;
};
}  // namespace stonks::core

#endif  // STONKS_APP_CORE_SYMBOLS_DB_UPDATER_CORE_I_SYMBOLS_DB_UPDATER_H_
