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
  [[nodiscard]] static auto GetName() -> auto & {
    static const auto kName = std::string{nameof::nameof_short_type<Table>()};
    return kName;
  }
};

template <typename Original, typename Alias>
struct As {};

template <typename Column>
struct ColumnTraits {
  [[nodiscard]] static auto GetName() -> auto & {
    static const auto kName = std::string{nameof::nameof_short_type<Column>()};
    return kName;
  }

  [[nodiscard]] static auto GetFullName() -> auto & {
    static const auto kName = fmt::format(
        "{}.{}", nameof::nameof_short_type<typename Column::Table>(),
        nameof::nameof_short_type<Column>());
    return kName;
  }

  [[nodiscard]] static auto GetForeignKey() -> auto & {
    static const auto kValue = []() -> cpp::Opt<sqldb::ForeignKey> {
      if constexpr (requires { typename Column::ForeignKey; }) {
        return sqldb::ForeignKey{
            .table = {TableTraitsGetName<
                typename Column::ForeignKey::Table>::GetName()},
            .column = {
                ColumnTraits<typename Column::ForeignKey>::GetName()}};
      } else {
        return std::nullopt;
      }
    }();
    return kValue;
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

  [[nodiscard]] static auto GetDefinition() -> auto & {
    static const auto kValue =
        sqldb::ColumnDefinition{.column = {ColumnTraits<Column>::GetName()},
                                .data_type = GetType(),
                                .primary_key = HasPrimaryKey<Column>(),
                                .auto_increment = HasAutoIncrement<Column>(),
                                .unique = HasUnique<Column>(),
                                .foreign_key = GetForeignKey()};
    return kValue;
  }
};

template <typename Original, typename Alias>
struct ColumnTraits<As<Original, Alias>> {
  [[nodiscard]] static constexpr auto GetName() -> auto & {
    return ColumnTraits<Alias>::GetName();
  }

  [[nodiscard]] static auto GetFullName() -> auto & {
    static const auto kName = fmt::format(
        "{} AS {}", ColumnTraits<Original>::GetFullName(), GetName());
    return kName;
  }

  [[nodiscard]] static constexpr auto GetForeignKey() -> auto & {
    return ColumnTraits<Original>::GetForeignKey();
  }

  [[nodiscard]] static constexpr auto GetType() {
    return ColumnTraits<Original>::GetType();
  }

  [[nodiscard]] static constexpr auto GetDefinition() -> auto & {
    return ColumnTraits<Original>::GetDefinition();
  }
};

template <typename T>
struct ColumnsTraits;

template <typename... Columns>
struct ColumnsTraits<std::tuple<Columns...>> {
  [[nodiscard]] static constexpr auto GetSize() {
    return sizeof...(Columns);
  }

  [[nodiscard]] static auto GetNames() -> auto & {
    static const auto kValue = [] {
      auto names = std::string{};
      GetNamesImpl<Columns...>(names);
      return names;
    }();
    return kValue;
  }

  [[nodiscard]] static auto GetFullNames() -> auto & {
    static const auto kValue = [] {
      auto names = std::string{};
      GetFullNamesImpl<Columns...>(names);
      return names;
    }();
    return kValue;
  }

  [[nodiscard]] static auto GetTypes() -> auto & {
    static const auto kValue = [] {
      auto types = std::vector<sqldb::CellDefinition>{};
      GetTypesImpl<Columns...>(types);
      return types;
    }();
    return kValue;
  }

  [[nodiscard]] static auto GetDefinitions() -> auto & {
    static const auto kValue = [] {
      auto definitions = std::vector<sqldb::ColumnDefinition>{};
      GetDefinitionsImpl<Columns...>(definitions);
      return definitions;
    }();
    return kValue;
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
  static void GetFullNamesImpl(std::string &names) {
    if (names.empty()) {
      names = ColumnTraits<Column>::GetFullName();
    } else {
      names += fmt::format(", {}", ColumnTraits<Column>::GetFullName());
    }

    if constexpr (sizeof...(OtherColumns) > 0) {
      GetFullNamesImpl<OtherColumns...>(names);
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
    static const auto kValue = std::string{nameof::nameof_short_type<Table>()};
    return kValue;
  }

  [[nodiscard]] static auto GetDefinition() -> auto & {
    static const auto kValue = sqldb::TableDefinition{
        .table = {GetName()},
        .columns = ColumnsTraits<typename Table::Columns>::GetDefinitions()};
    return kValue;
  }
};

template <typename Original, typename Alias>
struct TableTraits<As<Original, Alias>> {
  [[nodiscard]] static auto GetName() -> auto & {
    static const auto kName =
        fmt::format("{} AS {}", TableTraits<Original>::GetName(),
                    TableTraits<Alias>::GetName());
    return kName;
  }

  [[nodiscard]] static constexpr auto GetDefinition() -> auto & {
    return TableTraits<Original>::GetDefinition();
  }
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_TRAITS_H_
