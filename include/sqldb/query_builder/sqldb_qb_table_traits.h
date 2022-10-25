#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_TABLE_TRAITS_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_TABLE_TRAITS_H_

#include <string>

#include "sqldb_table_traits.h"
#include "sqldb_types.h"
#include "sqldb_qb_types.h"

/**
 * @file API to retrieve query-related info from table definitions.
 */

namespace stonks::sqldb::qb {
template <typename T>
struct ColumnsTraits;

template <typename... Columns>
struct ColumnsTraits<std::tuple<Columns...>> {
  [[nodiscard]] static auto GetNames() -> auto & {
    static const auto kConstant = [] {
      auto values = std::vector<std::string>{};
      GetNamesImpl<Columns...>(values);
      return values;
    }();
    return kConstant;
  }

  [[nodiscard]] static auto GetDefinitions() -> auto & {
    static const auto kConstant = [] {
      auto values = std::vector<ColumnDefinition>{};
      GetDefinitionsImpl<Columns...>(values);
      return values;
    }();
    return kConstant;
  }

  [[nodiscard]] static auto GetPrimaryKeys() -> auto & {
    static const auto kConstant = [] {
      auto values = std::vector<PrimaryKey>{};
      GetPrimaryKeysImpl<Columns...>(values);
      return values;
    }();
    return kConstant;
  }

  [[nodiscard]] static auto GetForeignKeys() -> auto & {
    static const auto kConstant = [] {
      auto values = std::vector<ForeignKey>{};
      GetForeignKeysImpl<Columns...>(values);
      return values;
    }();
    return kConstant;
  }

 private:
  template <typename Column, typename... OtherColumns>
  static void GetNamesImpl(std::vector<std::string> &values) {
    values.emplace_back(ColumnTraits<Column>::GetName());

    if constexpr (sizeof...(OtherColumns) > 0) {
      GetNamesImpl<OtherColumns...>(values);
    }
  }

  template <typename Column, typename... OtherColumns>
  static void GetDefinitionsImpl(std::vector<ColumnDefinition> &values) {
    using ColumnTraits = ColumnTraits<Column>;

    values.emplace_back(ColumnDefinition{.name = ColumnTraits::GetName(),
                                         .type = ColumnTraits::GetType(),
                                         .unique = ColumnTraits::IsUnique()});

    if constexpr (sizeof...(OtherColumns) > 0) {
      GetDefinitionsImpl<OtherColumns...>(values);
    }
  }

  template <typename Column, typename... OtherColumns>
  static void GetPrimaryKeysImpl(std::vector<PrimaryKey> &values) {
    using ColumnTraits = ColumnTraits<Column>;

    if constexpr (ColumnTraits::IsPrimaryKey()) {
      values.emplace_back(
          PrimaryKey{.column_name = ColumnTraits::GetName(),
                     .auto_increment = ColumnTraits::HasAutoIncrement()});
    }

    if constexpr (sizeof...(OtherColumns) > 0) {
      GetPrimaryKeysImpl<OtherColumns...>(values);
    }
  }

  template <typename Column, typename... OtherColumns>
  static void GetForeignKeysImpl(std::vector<ForeignKey> &values) {
    using ColumnTraits = ColumnTraits<Column>;

    if constexpr (ColumnTraits::IsForeignKey()) {
      values.emplace_back(ForeignKey{
          .column_name = ColumnTraits::GetName(),
          .target_table_name =
              TableTraits<typename Column::ForeignKey::Table>::GetName(),
          .target_column_name = ::stonks::sqldb::ColumnTraits<
              typename Column::ForeignKey>::GetName()});
    }

    if constexpr (sizeof...(OtherColumns) > 0) {
      GetForeignKeysImpl<OtherColumns...>(values);
    }
  }
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_TABLE_TRAITS_H_
