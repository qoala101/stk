#include "stonks_db_cache.h"

#include <gsl/assert>
#include <memory>
#include <utility>
#include <vector>

#include "cpp_typed_struct.h"
#include "not_null.hpp"
#include "sqldb_i_select_statement.h"
#include "sqldb_rows.h"
#include "stonks_db_prepared_statements.h"
#include "stonks_types.h"

namespace stonks::db {
namespace {
template <typename Key, typename Value>
[[nodiscard]] auto GetValue(const std::map<Key, Value> &map, const Key &key) {
  auto value = map.find(key);
  Expects(value != map.end());
  return value->second;
}
}  // namespace

Cache::Cache(cpp::NnSp<PreparedStatements> prepared_statements)
    : prepared_statements_{std::move(prepared_statements)} {}

auto Cache::GetAssetIdByAsset(const std::string &asset) const -> int64_t {
  return GetValue(asset_to_asset_id_map_, asset);
}

auto Cache::GetSymbolIdBySymbol(const SymbolName &symbol) const -> int64_t {
  return GetValue(symbol_to_symbol_id_map_, symbol);
}

auto Cache::GetSymbolBySymbolId(int64_t symbol_id) const -> SymbolName {
  return GetValue(symbol_id_to_symbol_map_, symbol_id);
}

void Cache::Update() {
  UpdateAssetMap();
  UpdateSymbolMaps();
}

void Cache::UpdateAssetMap() {
  auto rows = prepared_statements_->SelectAssetsWithIds().Execute();
  auto &ids = rows.GetColumnValues({"id"});
  auto &names = rows.GetColumnValues({"name"});

  asset_to_asset_id_map_.clear();

  for (auto i = 0; i < rows.GetSize(); ++i) {
    asset_to_asset_id_map_.emplace(std::move(names[i].GetString()),
                                   ids[i].GetInt64());
  }
}

void Cache::UpdateSymbolMaps() {
  auto rows = prepared_statements_->SelectSymbolsWithIds().Execute();
  auto &ids = rows.GetColumnValues({"id"});
  auto &names = rows.GetColumnValues({"name"});

  symbol_to_symbol_id_map_.clear();
  symbol_id_to_symbol_map_.clear();

  for (auto i = 0; i < rows.GetSize(); ++i) {
    const auto id = ids[i].GetInt64();
    auto &name = names[i].GetString();

    symbol_to_symbol_id_map_.emplace(SymbolName{name}, id);
    symbol_id_to_symbol_map_.emplace(id, SymbolName{std::move(name)});
  }
}
}  // namespace stonks::db