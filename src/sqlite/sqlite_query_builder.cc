#include "sqlite_query_builder.h"

#include <ext/alloc_traits.h>
#include <magic_enum.hpp>
#include <optional>
#include <range/v3/algorithm/any_of.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/view/adaptor.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/view.hpp>
#include <string>
#include <utility>

#include "sqldb_enum_conversions.h"  // IWYU pragma: keep

namespace stonks::sqlite {
auto SqliteQueryBuilder::BuildCreateTableIfNotExistsQuery(
    const sqldb::TableDefinition &table_definition) const -> std::string {
  const auto column_is_primary_key = [](const sqldb::ColumnDefinition &column) {
    return column.primary_key;
  };
  const auto has_primary_keys =
      ranges::any_of(table_definition.columns, column_is_primary_key);

  const auto column_is_foreign_key = [](const sqldb::ColumnDefinition &column) {
    return column.foreign_key.has_value();
  };
  const auto has_foreign_keys =
      ranges::any_of(table_definition.columns, column_is_foreign_key);

  const auto has_keys = has_primary_keys || has_foreign_keys;

  auto query = std::string{"CREATE TABLE IF NOT EXISTS \"" +
                           table_definition.table + "\"(\n"};

  for (const auto &column_def : table_definition.columns) {
    query += "\t\"" + column_def.column + "\" " +
             std::string{magic_enum::enum_name(column_def.data_type)} +
             " NOT NULL";

    if (column_def.unique) {
      query += " UNIQUE";
    }

    if (const auto query_not_done =
            (&column_def != &table_definition.columns.back()) || has_keys) {
      query += ",\n";
    }
  }

  if (has_primary_keys) {
    query += "PRIMARY KEY(";
    auto primary_key_columns =
        table_definition.columns | ranges::views::filter(column_is_primary_key);

    for (const auto &column_def : primary_key_columns) {
      query += "\"" + column_def.column + "\"";

      if (column_def.auto_increment) {
        query += " AUTOINCREMENT";
      }

      if (const auto query_not_done =
              &column_def != &primary_key_columns.back()) {
        query += ", ";
      }
    }

    query += ")";

    if (has_foreign_keys) {
      query += ",\n";
    }
  }

  if (has_foreign_keys) {
    auto foreign_key_columns =
        table_definition.columns | ranges::views::filter(column_is_foreign_key);

    for (const auto &column_def : foreign_key_columns) {
      const auto &foreign_key = *column_def.foreign_key;
      query += "FOREIGN KEY(\"" + column_def.column + "\") REFERENCES \"" +
               foreign_key.table + "\"(\"" + foreign_key.column +
               "\") ON DELETE CASCADE";

      if (const auto query_not_done =
              &column_def != &foreign_key_columns.back()) {
        query += ",\n";
      }
    }
  }

  query += "\n)";

  return query;
}

auto SqliteQueryBuilder::BuildDropTableQuery(const sqldb::Table &table)
    -> std::string {
  return "DROP TABLE \"" + table + "\";";
}

auto SqliteQueryBuilder::BuildSelectQuery(const sqldb::Table &table,
                                          std::string_view where_clause) const
    -> std::string {
  auto query = "SELECT * FROM \"" + table + "\"";

  if (!where_clause.empty()) {
    query += std::string{" "} + where_clause.data();
  }

  return query;
}

auto SqliteQueryBuilder::BuildInsertQuery(
    const sqldb::Table &table, const std::vector<sqldb::Column> &columns) const
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

auto SqliteQueryBuilder::BuildUpdateQuery(
    const sqldb::Table &table, const std::vector<sqldb::Column> &columns,
    std::string_view where_clause) -> std::string {
  auto query = "UPDATE \"" + table + "\" SET ";

  const auto num_columns = columns.size();

  for (auto i = 0; i < num_columns; ++i) {
    query += "\"" + columns[i] + "\" = ?" + std::to_string(i + 1);

    if (const auto not_last_column = i < (num_columns - 1)) {
      query += ", ";
    }
  }

  if (!where_clause.empty()) {
    query += std::string{" "} + where_clause.data();
  }

  return query;
}

auto SqliteQueryBuilder::BuildDeleteQuery(const sqldb::Table &table,
                                          std::string_view where_clause)
    -> std::string {
  return "DELETE FROM \"" + table + "\" " + where_clause.data();
}
}  // namespace stonks::sqlite