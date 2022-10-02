#ifndef STONKS_APP_SYMBOLS_DB_APP_SDB_APP_H_
#define STONKS_APP_SYMBOLS_DB_APP_SDB_APP_H_

#include "core_types.h"
#include "cpp_not_null.h"
#include "sqldb_i_db.h"
#include "sqldb_i_query_builder.h"

namespace stonks::app::sdb {
/**
 * @brief SQL DB manager which provides API to operate with symbols
 * and prices related data.
 */
class App {
 public:
  App(cpp::NnUp<sqldb::IDb> db, cpp::NnUp<sqldb::IQueryBuilder> query_builder);

  /**
   * @brief Selects all symbols.
   */
  [[nodiscard]] auto SelectSymbols() const -> std::vector<core::Symbol>;

  /**
   * @brief Selects symbol info.
   */
  [[nodiscard]] auto SelectSymbolInfo(const core::Symbol &symbol) const
      -> std::vector<core::SymbolInfo>;

  /**
   * @brief Selects price records following the conditions.
   */
  [[nodiscard]] auto SelectSymbolPriceRecords(const core::Symbol &symbol,
                                              const absl::Time *start_time,
                                              const absl::Time *end_time,
                                              const int *limit) const
      -> std::vector<core::SymbolPriceRecord>;

  /**
   * @brief Records symbol price.
   */
  void InsertSymbolPriceRecord(const core::SymbolPriceRecord &record);

 private:
  void CreateTablesIfNotExist();

  cpp::NnUp<sqldb::IDb> db_;
  cpp::NnUp<sqldb::IQueryBuilder> query_builder_;
};
}  // namespace stonks::app::sdb

#endif  // STONKS_APP_SYMBOLS_DB_APP_SDB_APP_H_
