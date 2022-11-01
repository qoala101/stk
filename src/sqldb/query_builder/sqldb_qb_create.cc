#include "sqldb_qb_create.h"

#include <gsl/assert>

namespace stonks::sqldb::qb {
namespace {
[[nodiscard]] constexpr auto ToString(const DataTypeVariant &type) {
  return std::visit(
      [](const auto &v) {
        using V = typename std::decay_t<decltype(v)>::Type;

        if constexpr (std::is_same_v<V, bool> || std::is_same_v<V, int> ||
                      std::is_same_v<V, int64_t>) {
          return "INTEGER";
        } else if constexpr (std::is_same_v<V, double>) {
          return "REAL";
        } else if constexpr (std::is_same_v<V, std::string>) {
          return "TEXT";
        } else {
          Expects(false);
        }
      },
      type.value);
}

[[nodiscard]] auto BuildColumnsQuery(
    const std::vector<ColumnDefinition> &column_definitions) {
  Expects(!column_definitions.empty());

  auto query = std::string{};

  for (const auto &column : column_definitions) {
    if (!query.empty()) {
      query += ", ";
    }

    Expects(!column.name.empty());
    query +=
        fmt::format(R"("{}" {} NOT NULL)", column.name, ToString(column.type));

    if (column.unique) {
      query += " UNIQUE";
    }
  }

  return query;
}

[[nodiscard]] auto BuildPrimaryKeysQuery(
    const std::vector<PrimaryKey> &primary_keys) {
  auto query = std::string{};

  for (const auto &primary_key : primary_keys) {
    const auto first_key = query.empty();

    query += ", ";

    if (first_key) {
      query += "PRIMARY KEY(";
    }

    Expects(!primary_key.column_name.empty());
    query += fmt::format(R"("{}")", primary_key.column_name);

    if (primary_key.auto_increment) {
      query += " AUTOINCREMENT";
    }
  }

  if (!query.empty()) {
    query += ")";
  }

  return query;
}

[[nodiscard]] auto BuildForeignKeysQuery(
    const std::vector<ForeignKey> &foreign_keys) {
  auto query = std::string{};

  for (const auto &foreign_key : foreign_keys) {
    Expects(!foreign_key.column_name.empty());
    Expects(!foreign_key.target_table_name.empty());
    Expects(!foreign_key.target_column_name.empty());

    query += fmt::format(
        R"(, FOREIGN KEY("{}") REFERENCES "{}"("{}") ON DELETE CASCADE)",
        foreign_key.column_name, foreign_key.target_table_name,
        foreign_key.target_column_name);
  }

  return query;
}
}  // namespace

auto Create::IfNotExists() -> Create & {
  Expects(if_not_exists_query_.value.empty());
  if_not_exists_query_.value = " IF NOT EXISTS";
  Ensures(!if_not_exists_query_.value.empty());
  return *this;
}

auto Create::Build() const -> Query {
  return {fmt::format(R"(CREATE TABLE{} "{}"({}{}{}))",
                      if_not_exists_query_.value, table_name_.value,
                      columns_query_.value, primary_keys_query_.value,
                      foreign_keys_query_.value)};
}

Create::Create(std::string table_name,
               const std::vector<ColumnDefinition> &column_definitions,
               const std::vector<PrimaryKey> &primary_keys,
               const std::vector<ForeignKey> &foreign_keys)
    : table_name_{[&table_name]() {
        Expects(!table_name.empty());
        return std::move(table_name);
      }()},
      columns_query_{[&column_definitions]() {
        Expects(!column_definitions.empty());
        return BuildColumnsQuery(column_definitions);
      }()},
      primary_keys_query_{BuildPrimaryKeysQuery(primary_keys)},
      foreign_keys_query_{BuildForeignKeysQuery(foreign_keys)} {
  Ensures(!table_name_.value.empty());
  Ensures(!columns_query_.value.empty());
}
}  // namespace stonks::sqldb::qb