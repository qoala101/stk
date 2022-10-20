#ifndef STONKS_SQLDB_SQLDB_TRAITS_H_
#define STONKS_SQLDB_SQLDB_TRAITS_H_

#include <fmt/format.h>

#include <gsl/assert>
#include <nameof.hpp>
#include <string>

#include "cpp_optional.h"
#include "sqldb_types.h"

/**
 * @file API to get info from the table and column types.
 */

namespace stonks::sqldb {
template <typename Column>
[[nodiscard]] constexpr auto HasPrimaryKey() {
  return requires { typename Column::PrimaryKey; };
}

template <typename Column>
[[nodiscard]] constexpr auto HasAutoIncrement() {
  return requires { typename Column::AutoIncrement; };
}

template <typename Column>
[[nodiscard]] constexpr auto HasUnique() {
  return requires { typename Column::Unique; };
}

template <typename Table>
struct TableTraitsGetName {
  [[nodiscard]] static auto GetName() {
    static const auto kName = std::string{nameof::nameof_short_type<Table>()};
    return kName;
  }
};

template <typename Column>
struct ColumnTraits {
  [[nodiscard]] static auto GetName() {
    static const auto kName = std::string{nameof::nameof_short_type<Column>()};
    return kName;
  }

  [[nodiscard]] static auto GetFullName() {
    static const auto kName = fmt::format(
        "{}.{}", nameof::nameof_short_type<typename Column::Table>(),
        nameof::nameof_short_type<Column>());
    return kName;
  }

  [[nodiscard]] static auto GetForeignKey() -> cpp::Opt<sqldb::ForeignKey> {
    if constexpr (requires { typename Column::ForeignKey; }) {
      return sqldb::ForeignKey{
          .table = {TableTraitsGetName<
              typename Column::ForeignKey::Table>::GetName()},
          .column = {
              ColumnTraits<typename Column::ForeignKey::Column>::GetName()}};
    } else {
      return std::nullopt;
    }
  }

  [[nodiscard]] static constexpr auto GetType() {
    using Type = typename Column::DataType;

    if constexpr (std::is_same_v<Type, bool>) {
      return sqldb::DataType::kBool;
    } else if constexpr (std::is_same_v<Type, int>) {
      return sqldb::DataType::kInt;
    } else if constexpr (std::is_same_v<Type, int64_t>) {
      return sqldb::DataType::kInt64;
    } else if constexpr (std::is_same_v<Type, double>) {
      return sqldb::DataType::kDouble;
    } else if constexpr (std::is_same_v<Type, std::string>) {
      return sqldb::DataType::kString;
    } else {
      Expects(false);
    }
  }

  [[nodiscard]] static auto GetDefinition() {
    return sqldb::ColumnDefinition{.column = {ColumnTraits<Column>::GetName()},
                                   .data_type = GetType(),
                                   .primary_key = HasPrimaryKey<Column>(),
                                   .auto_increment = HasAutoIncrement<Column>(),
                                   .unique = HasUnique<Column>(),
                                   .foreign_key = GetForeignKey()};
  }
};

template <typename T>
struct ColumnsTraits;

template <typename... Columns>
struct ColumnsTraits<std::tuple<Columns...>> {
  [[nodiscard]] static auto GetNames() {
    auto names = std::string{};
    GetNamesImpl<Columns...>(names);
    return names;
  }

  [[nodiscard]] static auto GetTypes() {
    auto types = std::vector<sqldb::CellDefinition>{};
    GetTypesImpl<Columns...>(types);
    return types;
  }

  [[nodiscard]] static auto GetDefinitions() {
    auto definitions = std::vector<sqldb::ColumnDefinition>{};
    GetDefinitionsImpl<Columns...>(definitions);
    return definitions;
  }

 private:
  template <typename Column, typename... OtherColumns>
  static void GetNamesImpl(std::string &names) {
    if (names.empty()) {
      names = ColumnTraits<Column>::GetName();
    } else {
      names += fmt::format(", {}", ColumnTraits<Column>::GetName());
    }

    if constexpr (sizeof...(OtherColumns) > 0) {
      GetNamesImpl<OtherColumns...>(names);
    }
  }

  template <typename Column, typename... OtherColumns>
  static void GetTypesImpl(std::vector<sqldb::CellDefinition> &types) {
    types.emplace_back(
        sqldb::CellDefinition{.column = {ColumnTraits<Column>::GetName()},
                              .data_type = ColumnTraits<Column>::GetType()});

    if constexpr (sizeof...(OtherColumns) > 0) {
      GetTypesImpl<OtherColumns...>(types);
    }
  }

  template <typename Column, typename... OtherColumns>
  static void GetDefinitionsImpl(
      std::vector<sqldb::ColumnDefinition> &definitions) {
    definitions.emplace_back(ColumnTraits<Column>::GetDefinition());

    if constexpr (sizeof...(OtherColumns) > 0) {
      GetDefinitionsImpl<OtherColumns...>(definitions);
    }
  }
};

template <typename Table>
struct TableTraits {
  [[nodiscard]] static auto GetName() -> auto & {
    static const auto kName = std::string{nameof::nameof_short_type<Table>()};
    return kName;
  }

  [[nodiscard]] static auto GetDefinition() -> auto & {
    static const auto kDefinition = sqldb::TableDefinition{
        .table = {GetName()},
        .columns = ColumnsTraits<typename Table::Columns>::GetDefinitions()};
    return kDefinition;
  }
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_TRAITS_H_
