#ifndef STONKS_STONKS_STONKS_DB_UPDATER_PRICE_TICKS_H_
#define STONKS_STONKS_STONKS_DB_UPDATER_PRICE_TICKS_H_

#include "cpp_not_null.h"

#include "stonks_db.h"

namespace stonks {
class DbUpdaterPriceTicks {
 public:
  explicit DbUpdaterPriceTicks(cpp::Sp<StonksDb> stonks_db);

  DbUpdaterPriceTicks(const DbUpdaterPriceTicks &) = delete;
  DbUpdaterPriceTicks(DbUpdaterPriceTicks &&) noexcept;

  auto operator=(const DbUpdaterPriceTicks &) -> DbUpdaterPriceTicks & = delete;
  auto operator=(DbUpdaterPriceTicks &&) noexcept -> DbUpdaterPriceTicks &;

  ~DbUpdaterPriceTicks();

 private:
  class Impl;
  cpp::Up<Impl> impl_{};
};
}  // namespace stonks

#endif  // STONKS_STONKS_STONKS_DB_UPDATER_PRICE_TICKS_H_
