#include "sqlite_query_builder.h"

#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <string>

namespace stonks::db::sqlite {
namespace {
template <typename T>
[[nodiscard]] auto BuildInsertQuery1(const Table &table, const T &columns)
    -> std::string {
  auto column_names = std::string{};
  auto placeholders = std::string{};

  const auto num_columns = columns.size();

  for (auto i = 0; i < num_columns; ++i) {
    column_names += "\"" + columns[i] + "\"";
    placeholders += "?" + std::to_string(i + 1);

    if (const auto not_last_column = i < (num_columns - 1)) {
      column_names += ", ";
      placeholders += ", ";
    }
  }

  return "INSERT INTO \"" + table + "\"(" + column_names + ") VALUES (" +
         placeholders + ")";
}
}  // namespace

auto SqliteQueryBuilder::BuildInsertQuery(
    const TableDefinition &table_definition) const -> std::string {
  return BuildInsertQuery1(
      table_definition.table,
      table_definition.columns |
          ranges::views::transform([](const auto &column_definition) {
            return column_definition.column;
          }));
}

auto SqliteQueryBuilder::BuildInsertQuery(
    const Table &table, const std::vector<Column> &columns) const
    -> std::string {
  return BuildInsertQuery1(table, columns);
}

auto SqliteQueryBuilder::BuildSelectQuery(const Table &table) const
    -> std::string {
  return "SELECT * FROM \"" + table + "\"";
}
}  // namespace stonks::db::sqlite