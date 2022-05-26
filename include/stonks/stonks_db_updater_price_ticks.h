#ifndef STONKS_STONKS_STONKS_DB_UPDATER_PRICE_TICKS_H_
#define STONKS_STONKS_STONKS_DB_UPDATER_PRICE_TICKS_H_

#include <memory>
#include <string_view>

#include "stonks_db.h"

namespace stonks {
class DbUpdaterPriceTicks {
 public:
  explicit DbUpdaterPriceTicks(std::shared_ptr<StonksDb> stonks_db);

  DbUpdaterPriceTicks(const DbUpdaterPriceTicks &) = delete;
  DbUpdaterPriceTicks(DbUpdaterPriceTicks &&) noexcept;

  auto operator=(const DbUpdaterPriceTicks &) -> DbUpdaterPriceTicks & = delete;
  auto operator=(DbUpdaterPriceTicks &&) noexcept -> DbUpdaterPriceTicks &;

  ~DbUpdaterPriceTicks();

 private:
  class Impl;
  std::unique_ptr<Impl> impl_{};
};
}  // namespace stonks

#endif  // STONKS_STONKS_STONKS_DB_UPDATER_PRICE_TICKS_H_
