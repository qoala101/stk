#include "sqlite_query_builder.h"

#include <gsl/assert>
#include <iterator>
#include <magic_enum.hpp>
#include <optional>
#include <range/v3/algorithm/any_of.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/adaptor.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/view.hpp>
#include <string>
#include <utility>

#include "sqldb_enums_to_string.h"  // IWYU pragma: keep

namespace stonks::sqlite {
auto SqliteQueryBuilder::BuildCreateTableIfNotExistsQuery(
    const sqldb::TableDefinition &table_definition) const -> std::string {
  Expects(!table_definition.table.empty());
  Expects(!table_definition.columns.empty());

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

  Ensures(!query.empty());
  return query;
}

auto SqliteQueryBuilder::BuildDropTableQuery(const sqldb::Table &table) const
    -> std::string {
  Expects(!table.empty());

  auto query = "DROP TABLE \"" + table + "\"";

  Ensures(!query.empty());
  return query;
}

auto SqliteQueryBuilder::BuildSelectQuery(
    const sqldb::Table &table, const std::vector<sqldb::Column> *columns,
    std::string_view where_clause) const -> std::string {
  Expects(!table.empty());

  auto query = std::string{"SELECT "};

  if (const auto select_all_columns = columns == nullptr) {
    query += "*";
  } else {
    Expects(!columns->empty());
    query += ranges::accumulate(std::next(columns->begin()), columns->end(),
                                columns->front(),
                                [](const auto &query, const auto &column) {
                                  return query + ", " + column;
                                });
  }

  query += " FROM \"" + table + "\"";

  if (!where_clause.empty()) {
    query += std::string{" "} + where_clause.data();
  }

  Ensures(!query.empty());
  return query;
}

auto SqliteQueryBuilder::BuildInsertQuery(
    const sqldb::Table &table, const std::vector<sqldb::Column> &columns) const
    -> std::string {
  Expects(!table.empty());
  Expects(!columns.empty());

  auto column_names = std::string{};
  auto placeholders = std::string{};

  for (const auto &column : columns) {
    column_names += "\"" + column + "\"";
    placeholders += "?";

    if (const auto not_last_column = &column != &columns.back()) {
      column_names += ", ";
      placeholders += ", ";
    }
  }

  auto query = "INSERT INTO \"" + table + "\"(" + column_names + ") VALUES (" +
               placeholders + ")";

  Ensures(!query.empty());
  return query;
}

auto SqliteQueryBuilder::BuildUpdateQuery(
    const sqldb::Table &table, const std::vector<sqldb::Column> &columns,
    std::string_view where_clause) const -> std::string {
  Expects(!table.empty());
  Expects(!columns.empty());

  auto query = "UPDATE \"" + table + "\" SET ";

  for (const auto &column : columns) {
    query += "\"" + column + "\" = ?";

    if (const auto not_last_column = &column != &columns.back()) {
      query += ", ";
    }
  }

  if (!where_clause.empty()) {
    query += std::string{" "} + where_clause.data();
  }

  Ensures(!query.empty());
  return query;
}

auto SqliteQueryBuilder::BuildDeleteQuery(const sqldb::Table &table,
                                          std::string_view where_clause) const
    -> std::string {
  Expects(!table.empty());

  auto query = "DELETE FROM \"" + table + "\"";

  if (!where_clause.empty()) {
    query += std::string{" "} + where_clause.data();
  }

  Ensures(!query.empty());
  return query;
}
}  // namespace stonks::sqlite