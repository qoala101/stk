#ifndef STONKS_FINANCE_DB_FINANCE_DB_H_
#define STONKS_FINANCE_DB_FINANCE_DB_H_

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "cache.h"
#include "finance_types.h"
#include "not_null.hpp"
#include "prepared_statements.h"
#include "sqldb_db.h"
#include "sqldb_db_factory.h"
#include "sqldb_query_builder.h"
#include "stonks_db.h"

namespace stonks::finance {
/**
 * @copydoc StonksDb
 */
class FinanceDb : public StonksDb {
 public:
  /**
   * @brief Reads DB from the specified file.
   * @param db_factory SQL DB implementation.
   */
  explicit FinanceDb(const sqldb::IDbFactory &db_factory,
                     std::string_view file_path);

  [[deprecated]] explicit FinanceDb(std::string_view uri = "stonks.db");

  FinanceDb(const FinanceDb &) = delete;
  FinanceDb(FinanceDb &&) noexcept;

  auto operator=(const FinanceDb &) -> FinanceDb & = delete;
  auto operator=(FinanceDb &&) noexcept -> FinanceDb &;

  /**
   * @brief Writes DB to the file specified at construction.
   */
  ~FinanceDb() override;

  /**
   * @copydoc StonksDb::SelectAssets
   */
  [[nodiscard]] auto SelectAssets() const -> std::vector<std::string> override;

  /**
   * @copydoc StonksDb::UpdateAssets
   */
  void UpdateAssets(std::vector<std::string> assets) override;

  /**
   * @copydoc StonksDb::SelectSymbols
   */
  [[nodiscard]] auto SelectSymbols() const -> std::vector<SymbolName> override;

  /**
   * @copydoc StonksDb::SelectSymbolsInfo
   */
  [[nodiscard]] auto SelectSymbolsInfo() const
      -> std::vector<SymbolInfo> override;

  /**
   * @copydoc StonksDb::UpdateSymbolsInfo
   */
  void UpdateSymbolsInfo(std::vector<SymbolInfo> symbols_info) override;

  /**
   * @copydoc StonksDb::SelectSymbolPriceTicks
   */
  [[nodiscard]] auto SelectSymbolPriceTicks(const SymbolName *symbol,
                                            const Period *period,
                                            const int *limit) const
      -> std::vector<SymbolPriceTick> override;

  /**
   * @copydoc StonksDb::InsertSymbolPriceTick
   */
  void InsertSymbolPriceTick(const SymbolPriceTick &symbol_price_tick) override;

 private:
  void CreateTablesIfNotExist();
  void InsertSymbolInfo(const SymbolInfo &symbol_info);
  void UpdateSymbolInfo(const SymbolInfo &symbol_info);

  cpp::not_null<std::shared_ptr<sqldb::IDb>> db_;
  cpp::not_null<std::shared_ptr<sqldb::IQueryBuilder>> query_builder_;
  cpp::not_null<std::shared_ptr<PreparedStatements>> prepared_statements_;
  Cache cache_;
};
}  // namespace stonks::finance

#endif  // STONKS_FINANCE_DB_FINANCE_DB_H_
