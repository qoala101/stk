#ifndef STONKS_STONKS_DB_STONKS_DB_H_
#define STONKS_STONKS_DB_STONKS_DB_H_

#include <string>
#include <string_view>
#include <vector>

#include "cpp_not_null.h"
#include "sqldb_i_db.h"
#include "sqldb_i_factory.h"
#include "sqldb_i_query_builder.h"
#include "stonks_cache.h"
#include "stonks_i_db.h"
#include "stonks_prepared_statements.h"
#include "stonks_types.h"

namespace stonks {
/**
 * @copydoc IDb
 */
class Db : public IDb {
 public:
  /**
   * @brief Reads DB from the specified file.
   * @param db_factory SQL DB implementation.
   */
  explicit Db(std::string_view file_path, const sqldb::IFactory &db_factory);

  [[deprecated]] explicit Db(std::string_view uri = "stonks.db");

  Db(const Db &) = delete;
  Db(Db &&) noexcept;

  auto operator=(const Db &) -> Db & = delete;
  auto operator=(Db &&) noexcept -> Db &;

  /**
   * @brief Writes DB to the file specified at construction.
   */
  ~Db() override;

  /**
   * @copydoc IDb::SelectAssets
   */
  [[nodiscard]] auto SelectAssets() const -> std::vector<std::string> override;

  /**
   * @copydoc IDb::UpdateAssets
   */
  void UpdateAssets(const std::vector<std::string> &assets) override;

  /**
   * @copydoc IDb::SelectSymbols
   */
  [[nodiscard]] auto SelectSymbols() const -> std::vector<SymbolName> override;

  /**
   * @copydoc IDb::SelectSymbolsInfo
   */
  [[nodiscard]] auto SelectSymbolsInfo() const
      -> std::vector<SymbolInfo> override;

  /**
   * @copydoc IDb::UpdateSymbolsInfo
   */
  void UpdateSymbolsInfo(const std::vector<SymbolInfo> &symbols_info) override;

  /**
   * @copydoc IDb::SelectSymbolPriceTicks
   */
  [[nodiscard]] auto SelectSymbolPriceTicks(const SymbolName *symbol,
                                            const Period *period,
                                            const int *limit) const
      -> std::vector<SymbolPriceTick> override;

  /**
   * @copydoc IDb::InsertSymbolPriceTick
   */
  void InsertSymbolPriceTick(const SymbolPriceTick &symbol_price_tick) override;

 private:
  void CreateTablesIfNotExist();
  void InsertSymbolInfo(const SymbolInfo &symbol_info);
  void UpdateSymbolInfo(const SymbolInfo &symbol_info);

  cpp::NnSp<sqldb::IDb> db_;
  cpp::NnSp<sqldb::IQueryBuilder> query_builder_;
  cpp::NnSp<db::PreparedStatements> prepared_statements_;
  db::Cache cache_;
};
}  // namespace stonks

#endif  // STONKS_STONKS_DB_STONKS_DB_H_
