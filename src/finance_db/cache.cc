#include "cache.h"

#include <gsl/assert>
#include <utility>
#include <vector>

#include "prepared_statements.h"
#include "sqldb_rows.h"
#include "sqldb_select_statement.h"

namespace stonks::finance {
namespace {
template <typename Key, typename Value>
[[nodiscard]] auto GetValue(const std::map<Key, Value> &map, const Key &key)
    -> Value {
  auto value = map.find(key);
  Expects(value != map.end());
  return value->second;
}
}  // namespace

Cache::Cache(
    cpp::not_null<std::shared_ptr<PreparedStatements>> prepared_statements)
    : prepared_statements_{std::move(prepared_statements)} {}

[[nodiscard]] auto Cache::GetAssetIdByAsset(const std::string &asset) const
    -> int64_t {
  return GetValue(asset_to_asset_id_map_, asset);
}

[[nodiscard]] auto Cache::GetSymbolIdBySymbol(const SymbolName &symbol) const
    -> int64_t {
  return GetValue(symbol_to_symbol_id_map_, symbol);
}

[[nodiscard]] auto Cache::GetSymbolBySymbolId(int64_t symbol_id) const
    -> SymbolName {
  return GetValue(symbol_id_to_symbol_map_, symbol_id);
}

void Cache::Update() {
  UpdateAssetMap();
  UpdateSymbolMaps();
}

void Cache::UpdateAssetMap() {
  auto rows = prepared_statements_->SelectAssetsWithIds().Execute();
  auto &ids = rows.GetColumnValues("id");
  auto &names = rows.GetColumnValues("name");

  asset_to_asset_id_map_.clear();

  for (auto i = 0; i < rows.GetSize(); ++i) {
    asset_to_asset_id_map_[std::move(names[i].GetString())] = ids[i].GetInt64();
  }
}

void Cache::UpdateSymbolMaps() {
  auto rows = prepared_statements_->SelectSymbolsWithIds().Execute();
  auto &ids = rows.GetColumnValues("id");
  auto &names = rows.GetColumnValues("name");

  symbol_to_symbol_id_map_.clear();
  symbol_id_to_symbol_map_.clear();

  for (auto i = 0; i < rows.GetSize(); ++i) {
    const auto id = ids[i].GetInt64();
    auto &name = names[i].GetString();

    symbol_to_symbol_id_map_[name] = id;
    symbol_id_to_symbol_map_[id] = std::move(name);
  }
}
}  // namespace stonks::finance