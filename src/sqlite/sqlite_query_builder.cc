#include "sqlite_query_builder.h"

#include <gsl/assert>
#include <magic_enum.hpp>
#include <range/v3/algorithm/any_of.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/adaptor.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/subrange.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <string>
#include <utility>

#include "cpp_format.h"
#include "cpp_optional.h"
#include "sqldb_enums_to_string.h"  // IWYU pragma: keep

namespace stonks::sqlite {
namespace {
auto IsColumnPrimaryKey(const sqldb::ColumnDefinition &column) -> bool {
  return column.primary_key;
}

auto IsColumnForeignKey(const sqldb::ColumnDefinition &column) -> bool {
  return column.foreign_key.has_value();
}
}  // namespace

auto QueryBuilder::BuildCreateTableIfNotExistsQuery(
    const sqldb::TableDefinition &table_definition) const -> sqldb::Query {
  Expects(!table_definition.table.value.empty());
  Expects(!table_definition.columns.empty());

  const auto has_primary_keys =
      ranges::any_of(table_definition.columns, &IsColumnPrimaryKey);
  const auto has_foreign_keys =
      ranges::any_of(table_definition.columns, &IsColumnForeignKey);
  const auto has_keys = has_primary_keys || has_foreign_keys;

  auto query = cpp::Format(R"(CREATE TABLE IF NOT EXISTS "{}"(
)",
                           table_definition.table.value);

  for (const auto &column_def : table_definition.columns) {
    query += cpp::Format(R"(	"{}" {} NOT NULL)", column_def.column.value,
                         magic_enum::enum_name(column_def.data_type));

    if (column_def.unique) {
      query += " UNIQUE";
    }

    if (const auto query_not_done =
            (&column_def != &table_definition.columns.back()) || has_keys) {
      query += R"(,
)";
    }
  }

  if (has_primary_keys) {
    query += "PRIMARY KEY(";
    
    auto primary_key_columns =
        table_definition.columns | ranges::views::filter(&IsColumnPrimaryKey);

    for (const auto &column_def : primary_key_columns) {
      query += cpp::Format(R"("{}")", column_def.column.value);

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
      query += R"(,
)";
    }
  }

  if (has_foreign_keys) {
    auto foreign_key_columns =
        table_definition.columns | ranges::views::filter(&IsColumnForeignKey);

    for (const auto &column_def : foreign_key_columns) {
      const auto &foreign_key = *column_def.foreign_key;
      query += cpp::Format(
          R"(FOREIGN KEY("{}") REFERENCES "{}"("{}") ON DELETE CASCADE)",
          column_def.column.value, foreign_key.table.value,
          foreign_key.column.value);

      if (const auto query_not_done =
              &column_def != &foreign_key_columns.back()) {
        query += R"(,
)";
      }
    }
  }

  query += R"(
))";

  Ensures(!query.empty());
  return {std::move(query)};
}

auto QueryBuilder::BuildDropTableQuery(const sqldb::Table &table) const
    -> sqldb::Query {
  Expects(!table.value.empty());

  auto query = cpp::Format(R"(DROP TABLE "{}")", table.value);

  Ensures(!query.empty());
  return {std::move(query)};
}

auto QueryBuilder::BuildSelectQuery(const sqldb::Table &table,
                                    const std::vector<sqldb::Column> *columns,
                                    std::string_view where_clause) const
    -> sqldb::Query {
  Expects(!table.value.empty());

  auto query = std::string{"SELECT "};

  if (const auto select_all_columns = columns == nullptr) {
    query += "*";
  } else {
    Expects(!columns->empty());
    const auto column_values =
        *columns | ranges::views::transform(
                       [](const auto &column) { return column.value; });
    query += ranges::accumulate(column_values | ranges::views::drop(1),
                                column_values.front(),
                                [](const auto &query, const auto &column) {
                                  return cpp::Format("{}, {}", query, column);
                                });
  }

  query += cpp::Format(R"( FROM "{}")", table.value);

  if (!where_clause.empty()) {
    query += cpp::Format(" {}", where_clause.data());
  }

  Ensures(!query.empty());
  return {std::move(query)};
}

auto QueryBuilder::BuildInsertQuery(
    const sqldb::Table &table, const std::vector<sqldb::Column> &columns) const
    -> sqldb::Query {
  Expects(!table.value.empty());
  Expects(!columns.empty());

  auto column_names = std::string{};
  auto placeholders = std::string{};

  for (const auto &column : columns) {
    column_names += cpp::Format(R"("{}")", column.value);
    placeholders += "?";

    if (const auto not_last_column = &column != &columns.back()) {
      column_names += ", ";
      placeholders += ", ";
    }
  }

  auto query = cpp::Format(R"(INSERT INTO "{}"({}) VALUES ({}))", table.value,
                           column_names, placeholders);

  Ensures(!query.empty());
  return {std::move(query)};
}

auto QueryBuilder::BuildUpdateQuery(const sqldb::Table &table,
                                    const std::vector<sqldb::Column> &columns,
                                    std::string_view where_clause) const
    -> sqldb::Query {
  Expects(!table.value.empty());
  Expects(!columns.empty());

  auto query = cpp::Format(R"(UPDATE "{}" SET )", table.value);

  for (const auto &column : columns) {
    query += cpp::Format(R"("{}" = ?)", column.value);

    if (const auto not_last_column = &column != &columns.back()) {
      query += ", ";
    }
  }

  if (!where_clause.empty()) {
    query += cpp::Format(" {}", where_clause);
  }

  Ensures(!query.empty());
  return {std::move(query)};
}

auto QueryBuilder::BuildDeleteQuery(const sqldb::Table &table,
                                    std::string_view where_clause) const
    -> sqldb::Query {
  Expects(!table.value.empty());

  auto query = cpp::Format(R"(DELETE FROM "{}")", table.value);

  if (!where_clause.empty()) {
    query += cpp::Format(" {}", where_clause);
  }

  Ensures(!query.empty());
  return {std::move(query)};
}
}  // namespace stonks::sqlite