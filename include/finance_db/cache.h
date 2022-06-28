#ifndef STONKS_FINANCE_DB_CACHE_H_
#define STONKS_FINANCE_DB_CACHE_H_

#include <map>
#include <memory>
#include <string>

#include "finance_types.h"
#include "prepared_statements.h"
#include "stonks_db.h"

namespace stonks::finance {
/**
 * @brief Cache used by DB in order to speedup often used lookups.
 */
class Cache {
 public:
  explicit Cache(std::shared_ptr<PreparedStatements> prepared_statements);

  [[nodiscard]] auto GetAssetIdByAsset(const std::string &asset) const
      -> int64_t;

  [[nodiscard]] auto GetSymbolIdBySymbol(const SymbolName &symbol) const
      -> int64_t;

  [[nodiscard]] auto GetSymbolBySymbolId(int64_t symbol_id) const -> SymbolName;

  void Update();

 private:
  void UpdateAssetMap();
  void UpdateSymbolMaps();

  std::shared_ptr<PreparedStatements> prepared_statements_;
  std::map<std::string, int64_t> asset_to_asset_id_map_{};
  std::map<SymbolName, int64_t> symbol_to_symbol_id_map_{};
  std::map<int64_t, SymbolName> symbol_id_to_symbol_map_{};
};
}  // namespace stonks::finance

#endif  // STONKS_FINANCE_DB_CACHE_H_
