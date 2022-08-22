#ifndef STONKS_STONKS_DB_STONKS_DB_CACHE_H_
#define STONKS_STONKS_DB_STONKS_DB_CACHE_H_

#include <cstdint>
#include <map>
#include <string>

#include "cpp_not_null.h"
#include "stonks_prepared_statements.h"
#include "stonks_types.h"

namespace stonks::db {
/**
 * @brief Cache used by DB in order to speedup often used lookups.
 */
class Cache {
 public:
  explicit Cache(cpp::NnSp<PreparedStatements> prepared_statements);

  [[nodiscard]] auto GetAssetIdByAsset(const std::string &asset) const
      -> int64_t;

  [[nodiscard]] auto GetSymbolIdBySymbol(const SymbolName &symbol) const
      -> int64_t;

  [[nodiscard]] auto GetSymbolBySymbolId(int64_t symbol_id) const -> SymbolName;

  void Update();

 private:
  void UpdateAssetMap();
  void UpdateSymbolMaps();

  cpp::NnSp<PreparedStatements> prepared_statements_;

  std::map<std::string, int64_t> asset_to_asset_id_map_{};
  std::map<SymbolName, int64_t> symbol_to_symbol_id_map_{};
  std::map<int64_t, SymbolName> symbol_id_to_symbol_map_{};
};
}  // namespace stonks::db

#endif  // STONKS_STONKS_DB_STONKS_DB_CACHE_H_
