#ifndef STONKS_APP_SYMBOLS_DB_APP_SDB_APP_H_
#define STONKS_APP_SYMBOLS_DB_APP_SDB_APP_H_

#include "core_types.h"

namespace stonks::app::sdb {
class App {
 public:
  App();

  void InsertSymbolPriceRecord(const core::SymbolPriceRecord &record);
};
}  // namespace stonks::app::sdb

#endif  // STONKS_APP_SYMBOLS_DB_APP_SDB_APP_H_
