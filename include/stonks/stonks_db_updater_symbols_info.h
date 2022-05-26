#ifndef STONKS_STONKS_STONKS_DB_UPDATER_SYMBOLS_INFO_H_
#define STONKS_STONKS_STONKS_DB_UPDATER_SYMBOLS_INFO_H_

#include <memory>
#include <string_view>

#include "stonks_db.h"

namespace stonks {
class DbUpdaterSymbolsInfo {
 public:
  explicit DbUpdaterSymbolsInfo(std::shared_ptr<StonksDb> stonks_db);

  DbUpdaterSymbolsInfo(const DbUpdaterSymbolsInfo &) = delete;
  DbUpdaterSymbolsInfo(DbUpdaterSymbolsInfo &&) noexcept;

  auto operator=(const DbUpdaterSymbolsInfo &)
      -> DbUpdaterSymbolsInfo & = delete;
  auto operator=(DbUpdaterSymbolsInfo &&) noexcept -> DbUpdaterSymbolsInfo &;

  ~DbUpdaterSymbolsInfo();

 private:
  class Impl;
  std::unique_ptr<Impl> impl_{};
};
}  // namespace stonks

#endif  // STONKS_STONKS_STONKS_DB_UPDATER_SYMBOLS_INFO_H_
