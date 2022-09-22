#include "app_dt_pds_db.h"

#include <memory>

#include "app_dt_pds_tables.h"
#include "cpp_chrono.h"
#include "not_null.hpp"
#include "sqldb_qbf_insert_query_builder.h"
#include "sqldb_query_builder_facade.h"
#include "sqldb_types.h"
#include "sqldb_value.h"
#include "sqldb_value_alias_v.h"

namespace stonks::app::dt::pds {
Db::Db(sqldb::IDb &db, const cpp::NnSp<sqldb::IQueryBuilder> &query_builder)
    : insert_symbol_price_record_statement_{[&db, &query_builder]() {
        db.PrepareStatement(query_builder->BuildCreateTableIfNotExistsQuery(
                                tables::SymbolPriceRecord()))
            ->Execute();

        auto statement = db.PrepareStatement(
            sqldb::QueryBuilderFacade{query_builder}
                .Insert()
                .IntoColumns({{"time"}, {"symbol"}, {"price"}})
                .IntoTable(tables::SymbolPriceRecord())
                .Build());
        return statement;
      }()} {}

void Db::InsertSymbolPriceRecord(const SymbolPriceRecord &record) {
  insert_symbol_price_record_statement_->Execute(
      {V{record.time.count()}, V{record.symbol}, V{record.price}});
}
}  // namespace stonks::app::dt::pds