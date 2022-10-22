#include "app_dt_pds_db.h"

#include <absl/time/time.h>

#include <memory>
#include <not_null.hpp>
#include <utility>

#include "app_dt_pds_tables.h"
#include "di_factory.h"
#include "sqldb_as_values.h"
#include "sqldb_qb_insert_query_builder.h"
#include "sqldb_query_builder.h"
#include "sqldb_types.h"

namespace stonks::app::dt::pds {
Db::Db(sqldb::IDb &db)
    : insert_symbol_price_record_statement_{[&db]() {
        db.PrepareStatement(
              query_builder_factory.Create()->BuildCreateTableIfNotExistsQuery(
                  tables::SymbolPriceRecord()))
            ->Execute();

        auto statement = db.PrepareStatement(
            sqldb::query_builder{std::move(query_builder_factory)}
                .Insert()
                .IntoColumns({{"time"}, {"symbol"}, {"price"}})
                .IntoTable(tables::SymbolPriceRecord())
                .Build());
        return statement;
      }()} {}

void Db::InsertSymbolPriceRecord(const SymbolPriceRecord &record) {
  insert_symbol_price_record_statement_->Execute(sqldb::AsValues(
      absl::ToUnixMillis(record.time), record.symbol, record.price));
}
}  // namespace stonks::app::dt::pds