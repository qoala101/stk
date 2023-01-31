#include "sqldb_qb_create.h"

#include <fmt/core.h>
#include <stdint.h>

#include <gsl/assert>
#include <not_null.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <type_traits>
#include <utility>
#include <variant>

#include "cpp_typed_struct.h"
#include "sqldb_qb_types.h"

namespace vh::sqldb::qb {
namespace {
auto ToString [[nodiscard]] (const DataTypeVariant &type) {
  return std::visit(
      [](const auto &v) {
        using V = typename std::remove_cvref_t<decltype(v)>::Type;

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
      *type);
}

auto BuildColumnsQuery
    [[nodiscard]] (const std::vector<CreateColumnData> &create_columns_data) {
  Expects(!create_columns_data.empty());

  auto query = ranges::accumulate(
      create_columns_data, std::string{}, [](auto &query, const auto &column) {
        if (!query.empty()) {
          query += ", ";
        }

        Expects(!column.name.empty());
        query +=
            fmt::format("{} {} NOT NULL", column.name, ToString(column.type));

        if (column.unique) {
          query += " UNIQUE";
        }

        return query;
      });

  return Query{std::move(query)};
}

auto BuildPrimaryKeysQuery
    [[nodiscard]] (const std::vector<PrimaryKeyData> &primary_keys_data) {
  auto query =
      ranges::accumulate(primary_keys_data, std::string{},
                         [](auto &query, const auto &primary_key) {
                           const auto first_key = query.empty();

                           query += ", ";

                           if (first_key) {
                             query += "PRIMARY KEY (";
                           }

                           Expects(!primary_key.column_name.empty());
                           query += fmt::format("{}", primary_key.column_name);

                           if (primary_key.auto_increment) {
                             query += " AUTOINCREMENT";
                           }

                           return query;
                         });

  if (!query.empty()) {
    query += ")";
  }

  return Query{std::move(query)};
}

auto BuildForeignKeysQuery
    [[nodiscard]] (const std::vector<ForeignKeyData> &foreign_keys_data) {
  auto query = ranges::accumulate(
      foreign_keys_data, std::string{},
      [](auto &query, const auto &foreign_key) {
        Expects(!foreign_key.column_name.empty());
        Expects(!foreign_key.target_table_name.empty());
        Expects(!foreign_key.target_column_name.empty());

        return query +
               fmt::format(
                   ", FOREIGN KEY ({}) REFERENCES {} ({}) ON DELETE CASCADE",
                   foreign_key.column_name, foreign_key.target_table_name,
                   foreign_key.target_column_name);
      });

  return Query{std::move(query)};
}
}  // namespace

auto Create::IfNotExists() -> Create & {
  Expects(if_not_exists_query_->empty());
  *if_not_exists_query_ = " IF NOT EXISTS";
  Ensures(!if_not_exists_query_->empty());
  return *this;
}

auto Create::Build() const -> Query {
  return {fmt::format("CREATE TABLE{} {} ({}{}{})", *if_not_exists_query_,
                      *table_name_, *columns_query_, *primary_keys_query_,
                      *foreign_keys_query_)};
}

Create::Create(std::string table_name,
               const std::vector<CreateColumnData> &create_columns_data,
               const std::vector<PrimaryKeyData> &primary_keys_data,
               const std::vector<ForeignKeyData> &foreign_keys_data)
    : table_name_{(Expects(!table_name.empty()), std::move(table_name))},
      columns_query_{[&create_columns_data]() {
        auto query = BuildColumnsQuery(create_columns_data);
        Expects(!query->empty());
        return query;
      }()},
      primary_keys_query_{BuildPrimaryKeysQuery(primary_keys_data)},
      foreign_keys_query_{BuildForeignKeysQuery(foreign_keys_data)} {
  Ensures(!table_name_->empty());
  Ensures(!columns_query_->empty());
}
}  // namespace vh::sqldb::qb