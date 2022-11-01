#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_TABLE_TRAITS_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_TABLE_TRAITS_H_

#include <string>

#include "sqldb_qb_types.h"
#include "sqldb_table_traits.h"

namespace stonks::sqldb {
/**
 * @brief Special treatment of table alias.
 */
template <typename Original, typename Alias>
struct TableTraits<qb::As<Original, Alias>> : public TableTraits<Original> {
  /**
   * @copydoc TableTraits::GetName
   */
  [[nodiscard]] static auto GetName() {
    return fmt::format("{} AS {}", TableTraits<Original>::GetName(),
                       TableTraits<Alias>::GetName());
  }
};

/**
 * @brief Special treatment of column alias.
 */
template <typename Original, typename Alias>
struct ColumnTraits<qb::As<Original, Alias>> : public ColumnTraits<Original> {
  /**
   * @copydoc ColumnTraits::GetName
   */
  [[nodiscard]] static auto GetName() { return ColumnTraits<Alias>::GetName(); }

  /**
   * @copydoc ColumnTraits::GetFullName
   */
  [[nodiscard]] static auto GetFullName() {
    return fmt::format("{} AS {}", ColumnTraits<Original>::GetFullName(),
                       ColumnTraits<Original>::GetName());
  }
};

namespace qb {
template <typename T>
struct ColumnsTraits;

/**
 * @brief API to retrieve query-related info from table columns.
 */
template <typename... Columns>
struct ColumnsTraits<std::tuple<Columns...>> {
  /**
   * @brief Gives column names.
   */
  [[nodiscard]] static auto GetNames() {
    auto names = std::vector<std::string>{};
    GetNamesImpl<Columns...>(names);
    return names;
  }

  /**
   * @brief Gives data required to create columns.
   */
  [[nodiscard]] static auto GetCreateColumnsData() {
    auto data = std::vector<CreateColumnData>{};
    GetCreateColumnsDataImpl<Columns...>(data);
    return data;
  }

  /**
   * @brief Parses primary keys data from column definitions.
   */
  [[nodiscard]] static auto GetPrimaryKeysData() {
    auto data = std::vector<PrimaryKeyData>{};
    GetPrimaryKeysDataImpl<Columns...>(data);
    return data;
  }

  /**
   * @brief Parses foreign keys data from column definitions.
   */
  [[nodiscard]] static auto GetForeignKeysData() {
    auto data = std::vector<ForeignKeyData>{};
    GetForeignKeysDataImpl<Columns...>(data);
    return data;
  }

  /**
   * @brief Gives data required to select values from columns.
   */
  [[nodiscard]] static auto GetSelectColumnsData() {
    auto data = std::vector<SelectColumnData>{};
    GetSelectColumnsDataImpl<Columns...>(data);
    return data;
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
  static void GetCreateColumnsDataImpl(std::vector<CreateColumnData> &data) {
    using ColumnTraits = ColumnTraits<Column>;

    data.emplace_back(CreateColumnData{.name = ColumnTraits::GetName(),
                                       .type = ColumnTraits::GetType(),
                                       .unique = ColumnTraits::IsUnique()});

    if constexpr (sizeof...(OtherColumns) > 0) {
      GetCreateColumnsDataImpl<OtherColumns...>(data);
    }
  }

  template <typename Column, typename... OtherColumns>
  static void GetPrimaryKeysDataImpl(std::vector<PrimaryKeyData> &data) {
    using ColumnTraits = ColumnTraits<Column>;

    if constexpr (ColumnTraits::IsPrimaryKey()) {
      data.emplace_back(
          PrimaryKeyData{.column_name = ColumnTraits::GetName(),
                         .auto_increment = ColumnTraits::HasAutoIncrement()});
    }

    if constexpr (sizeof...(OtherColumns) > 0) {
      GetPrimaryKeysDataImpl<OtherColumns...>(data);
    }
  }

  template <typename Column, typename... OtherColumns>
  static void GetForeignKeysDataImpl(std::vector<ForeignKeyData> &data) {
    using ColumnTraits = ColumnTraits<Column>;

    if constexpr (ColumnTraits::IsForeignKey()) {
      data.emplace_back(ForeignKeyData{
          .column_name = ColumnTraits::GetName(),
          .target_table_name =
              TableTraits<typename Column::ForeignKey::Table>::GetName(),
          .target_column_name = ::stonks::sqldb::ColumnTraits<
              typename Column::ForeignKey>::GetName()});
    }

    if constexpr (sizeof...(OtherColumns) > 0) {
      GetForeignKeysDataImpl<OtherColumns...>(data);
    }
  }

  template <typename Column, typename... OtherColumns>
  static void GetSelectColumnsDataImpl(std::vector<SelectColumnData> &data) {
    using ColumnTraits = ColumnTraits<Column>;

    data.emplace_back(SelectColumnData{.name = ColumnTraits::GetName(),
                                       .full_name = ColumnTraits::GetFullName(),
                                       .type = ColumnTraits::GetType()});

    if constexpr (sizeof...(OtherColumns) > 0) {
      GetSelectColumnsDataImpl<OtherColumns...>(data);
    }
  }
};
}  // namespace qb
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_TABLE_TRAITS_H_
