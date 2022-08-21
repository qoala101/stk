#ifndef STONKS_STONKS_STONKS_DB_UPDATER_PRICE_TICKS_H_
#define STONKS_STONKS_STONKS_DB_UPDATER_PRICE_TICKS_H_

#include "ccutils_not_null.h"

#include "stonks_db.h"

namespace stonks {
class DbUpdaterPriceTicks {
 public:
  explicit DbUpdaterPriceTicks(ccutils::Sp<StonksDb> stonks_db);

  DbUpdaterPriceTicks(const DbUpdaterPriceTicks &) = delete;
  DbUpdaterPriceTicks(DbUpdaterPriceTicks &&) noexcept;

  auto operator=(const DbUpdaterPriceTicks &) -> DbUpdaterPriceTicks & = delete;
  auto operator=(DbUpdaterPriceTicks &&) noexcept -> DbUpdaterPriceTicks &;

  ~DbUpdaterPriceTicks();

 private:
  class Impl;
  ccutils::Up<Impl> impl_{};
};
}  // namespace stonks

#endif  // STONKS_STONKS_STONKS_DB_UPDATER_PRICE_TICKS_H_
