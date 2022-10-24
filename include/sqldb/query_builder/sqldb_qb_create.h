#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_CREATE_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_CREATE_H_

#include <gsl/assert>
#include <magic_enum.hpp>
#include <string>
#include <string_view>
#include <tuple>

#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "sqldb_data_type.h"
#include "sqldb_traits.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qb {
namespace detail {
[[nodiscard]] inline auto ToString(const DataTypeVariant &data_type) {
  return std::visit(
      [](const auto &v) {
        using V = decltype(v);

        if constexpr (cpp::DecaysTo<V, sqldb::DataType<bool>> ||
                      cpp::DecaysTo<V, sqldb::DataType<int>> ||
                      cpp::DecaysTo<V, sqldb::DataType<int64_t>>) {
          return "INTEGER";
        } else if constexpr (cpp::DecaysTo<V, sqldb::DataType<double>>) {
          return "REAL";
        } else if constexpr (cpp::DecaysTo<V, sqldb::DataType<std::string>>) {
          return "TEXT";
        } else {
          Expects(false);
        }
      },
      data_type.value);
}

template <typename T>
struct ColumnsTraitsInternal;

template <typename... Columns>
struct ColumnsTraitsInternal<std::tuple<Columns...>> {
  [[nodiscard]] static auto GetColumnsQuery() -> auto & {
    static const auto kConstant = [] {
      auto query = std::string{};
      GetColumnsQueryImpl<Columns...>(query);
      return query;
    }();
    return kConstant;
  }

  [[nodiscard]] static auto GetPrimaryKeyQuery() -> auto & {
    static const auto kConstant = [] {
      auto query = std::string{};
      GetPrimaryKeyQueryImpl<Columns...>(query);
      return query;
    }();
    return kConstant;
  }

  [[nodiscard]] static auto GetForeignKeysQuery() -> auto & {
    static const auto kConstant = [] {
      auto query = std::string{};
      GetForeignKeysQueryImpl<Columns...>(query);
      return query;
    }();
    return kConstant;
  }

 private:
  template <ColumnT Column, ColumnT... OtherColumns>
  static void GetColumnsQueryImpl(std::string &query) {
    if (!query.empty()) {
      query += ", ";
    }

    query += fmt::format(R"("{}" {} NOT NULL)", ColumnTraits<Column>::GetName(),
                         ToString(ColumnTraits<Column>::GetType()));

    if constexpr (::stonks::sqldb::detail::HasUnique<Column>()) {
      query += " UNIQUE";
    }

    if constexpr (sizeof...(OtherColumns) > 0) {
      GetColumnsQueryImpl<OtherColumns...>(query);
    }
  }

  template <ColumnT Column, ColumnT... OtherColumns>
  static void GetPrimaryKeyQueryImpl(std::string &query) {
    if constexpr (::stonks::sqldb::detail::HasPrimaryKey<Column>()) {
      const auto first_primary_key_column = query.empty();

      if (first_primary_key_column) {
        query += ", PRIMARY KEY(";
      } else {
        query += ", ";
      }

      query += fmt::format(R"("{}")", ColumnTraits<Column>::GetName());

      if (::stonks::sqldb::detail::HasAutoIncrement<Column>()) {
        query += " AUTOINCREMENT";
      }
    }

    if constexpr (sizeof...(OtherColumns) > 0) {
      GetPrimaryKeyQueryImpl<OtherColumns...>(query);
    } else if (!query.empty()) {
      query += ")";
    }
  }

  template <ColumnT Column, ColumnT... OtherColumns>
  static void GetForeignKeysQueryImpl(std::string &query) {
    if constexpr (::stonks::sqldb::detail::HasForeignKey<Column>()) {
      query += fmt::format(
          R"(, FOREIGN KEY("{}") REFERENCES "{}"("{}") ON DELETE CASCADE)",
          ColumnTraits<Column>::GetName(),
          TableTraits<typename Column::ForeignKey::Table>::GetName(),
          ColumnTraits<typename Column::ForeignKey>::GetName());
    }

    if constexpr (sizeof...(OtherColumns) > 0) {
      GetForeignKeysQueryImpl<OtherColumns...>(query);
    }
  }
};
}  // namespace detail

class Create {
 public:
  template <typename Table>
  explicit Create(Table * /*unused*/)
      : table_name_{TableTraits<Table>::GetName()},
        columns_clause_{detail::ColumnsTraitsInternal<
            typename Table::Columns>::GetColumnsQuery()},
        primary_key_query_{detail::ColumnsTraitsInternal<
            typename Table::Columns>::GetPrimaryKeyQuery()},
        foreign_keys_query_{detail::ColumnsTraitsInternal<
            typename Table::Columns>::GetForeignKeysQuery()} {
    Ensures(!table_name_.empty());
    Ensures(!columns_clause_.empty());
  }

  [[nodiscard]] auto IfNotExists() -> Create &;

  [[nodiscard]] auto Build() const -> Query;

 private:
  std::string if_not_exists_clause_{};
  std::string table_name_{};
  std::string columns_clause_{};
  std::string primary_key_query_{};
  std::string foreign_keys_query_{};
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_CREATE_H_
