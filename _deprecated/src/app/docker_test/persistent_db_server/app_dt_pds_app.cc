#include "app_dt_pds_app.h"

#include <utility>

namespace stonks::app::dt::pds {
App::App(Db db) : db_{std::move(db)} {}

void App::RecordSymbolPrice(const SymbolPriceRecord &record) {
  return db_.InsertSymbolPriceRecord(record);
}
}  // namespace stonks::app::dt::pds