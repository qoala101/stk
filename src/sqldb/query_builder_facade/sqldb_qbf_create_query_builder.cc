#include "sqldb_qbf_create_query_builder.h"

#include <bits/ranges_algo.h>

#include <gsl/assert>
#include <magic_enum.hpp>
#include <range/v3/algorithm/any_of.hpp>
#include <range/v3/view/filter.hpp>
#include "sqldb_enums_to_string.h"

namespace stonks::sqldb::qbf {
namespace {
[[nodiscard]] auto IsColumnPrimaryKey(const sqldb::ColumnDefinition &column) {
  return column.primary_key;
}

[[nodiscard]] auto IsColumnForeignKey(const sqldb::ColumnDefinition &column) {
  return column.foreign_key.has_value();
}
}  // namespace

auto CreateQueryBuilderTemplate::IfNotExists() -> CreateQueryBuilderTemplate & {
  Expects(if_not_exists_clause_.empty());
  if_not_exists_clause_ = " IF NOT EXISTS";
  Ensures(!if_not_exists_clause_.empty());
  return *this;
}

auto CreateQueryBuilderTemplate::Build() const -> Query {
  Expects(!table_definition.table.value.empty());
  Expects(!table_definition.columns.empty());

  const auto has_primary_keys =
      ranges::any_of(table_definition.columns, &IsColumnPrimaryKey);
  const auto has_foreign_keys =
      ranges::any_of(table_definition.columns, &IsColumnForeignKey);
  const auto has_keys = has_primary_keys || has_foreign_keys;

  auto query = fmt::format(R"(CREATE TABLE{} "{}"(
)",
                           if_not_exists_clause_, table_definition.table.value);

  for (const auto &column_def : table_definition.columns) {
    query += fmt::format(R"(	"{}" {} NOT NULL)", column_def.column.value,
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
      query += fmt::format(R"("{}")", column_def.column.value);

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
      Expects(column_def.foreign_key.has_value());
      const auto &foreign_key = *column_def.foreign_key;

      query += fmt::format(
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
}  // namespace stonks::sqldb::qbf