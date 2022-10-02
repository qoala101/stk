#include "app_sdb_app.h"

#include <absl/time/time.h>
#include <fmt/core.h>

#include "app_sdb_tables.h"
#include "core_types.h"
#include "cpp_not_null.h"
#include "cpp_views.h"
#include "sqldb_types.h"

namespace stonks::app::sdb {
App::App(cpp::NnUp<sqldb::IDb> db,
         cpp::NnUp<sqldb::IQueryBuilder> query_builder)
    : db_{std::move(db)}, query_builder_{std::move(query_builder)} {
  CreateTablesIfNotExist();
}

void App::CreateTablesIfNotExist() {
  const auto tables = cpp::ConstView<sqldb::TableDefinition>{
      cpp::AssumeNn(&tables::Asset()), cpp::AssumeNn(&tables::SymbolInfo()),
      cpp::AssumeNn(&tables::SymbolPriceRecord())};

  for (const auto table : tables) {
    db_->PrepareStatement(
           query_builder_->BuildCreateTableIfNotExistsQuery(*table))
        ->Execute();
  }
}

void App::InsertSymbolPriceRecord(const core::SymbolPriceRecord &record) {
  fmt::print("Received: {} {} {}", absl::FormatTime(record.time),
             record.symbol.value, record.price);
}
}  // namespace stonks::app::sdb