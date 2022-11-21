#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_COLUMNS_TRAITS_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_COLUMNS_TRAITS_H_

#include <string>

#include "sqldb_concepts.h"  // IWYU pragma: keep
#include "sqldb_qb_types.h"

namespace stonks::sqldb::qb {
namespace detail {
template <ColumnDefinition Column, ColumnDefinition... OtherColumns>
void GetNamesImpl(std::vector<std::string> &values) {
  values.emplace_back(Column::GetName());

  if constexpr (sizeof...(OtherColumns) > 0) {
    GetNamesImpl<OtherColumns...>(values);
  }
}

template <ColumnDefinition Column, ColumnDefinition... OtherColumns>
void GetCreateColumnsDataImpl(std::vector<CreateColumnData> &data) {
  data.emplace_back(CreateColumnData{.name = Column::GetName(),
                                     .type = Column::GetType(),
                                     .unique = Column::IsUnique()});

  if constexpr (sizeof...(OtherColumns) > 0) {
    GetCreateColumnsDataImpl<OtherColumns...>(data);
  }
}

template <ColumnDefinition Column, ColumnDefinition... OtherColumns>
void GetPrimaryKeysDataImpl(std::vector<PrimaryKeyData> &data) {
  if constexpr (Column::IsPrimaryKey()) {
    data.emplace_back(
        PrimaryKeyData{.column_name = Column::GetName(),
                       .auto_increment = Column::HasAutoIncrement()});
  }

  if constexpr (sizeof...(OtherColumns) > 0) {
    GetPrimaryKeysDataImpl<OtherColumns...>(data);
  }
}

template <ColumnDefinition Column, ColumnDefinition... OtherColumns>
void GetForeignKeysDataImpl(std::vector<ForeignKeyData> &data) {
  if constexpr (Column::IsForeignKey()) {
    data.emplace_back(ForeignKeyData{
        .column_name = Column::GetName(),
        .target_table_name = Column::ForeignKey::Table::GetName(),
        .target_column_name = Column::ForeignKey::GetName()});
  }

  if constexpr (sizeof...(OtherColumns) > 0) {
    GetForeignKeysDataImpl<OtherColumns...>(data);
  }
}

template <ColumnDefinition Column, ColumnDefinition... OtherColumns>
void GetSelectColumnsDataImpl(std::vector<SelectColumnData> &data) {
  data.emplace_back(SelectColumnData{.name = Column::GetName(),
                                     .full_name = Column::GetFullName(),
                                     .type = Column::GetType()});

  if constexpr (sizeof...(OtherColumns) > 0) {
    GetSelectColumnsDataImpl<OtherColumns...>(data);
  }
}
}  // namespace detail

template <typename T>
struct ColumnsTraits;

template <TableDefinition Table>
struct ColumnsTraits<Table> : public ColumnsTraits<typename Table::Columns> {};

/**
 * @brief API to retrieve query-related info from table columns.
 */
template <ColumnDefinition... Columns>
struct ColumnsTraits<cpp::TypeList<Columns...>> {
  /**
   * @brief Gives column names.
   */
  static auto GetNames [[nodiscard]] () {
    auto names = std::vector<std::string>{};
    detail::GetNamesImpl<Columns...>(names);
    return names;
  }

  /**
   * @brief Gives data required to create columns.
   */
  static auto GetCreateColumnsData [[nodiscard]] () {
    auto data = std::vector<CreateColumnData>{};
    detail::GetCreateColumnsDataImpl<Columns...>(data);
    return data;
  }

  /**
   * @brief Parses primary keys data from column definitions.
   */
  static auto GetPrimaryKeysData [[nodiscard]] () {
    auto data = std::vector<PrimaryKeyData>{};
    detail::GetPrimaryKeysDataImpl<Columns...>(data);
    return data;
  }

  /**
   * @brief Parses foreign keys data from column definitions.
   */
  static auto GetForeignKeysData [[nodiscard]] () {
    auto data = std::vector<ForeignKeyData>{};
    detail::GetForeignKeysDataImpl<Columns...>(data);
    return data;
  }

  /**
   * @brief Gives data required to select values from columns.
   */
  static auto GetSelectColumnsData [[nodiscard]] () {
    auto data = std::vector<SelectColumnData>{};
    detail::GetSelectColumnsDataImpl<Columns...>(data);
    return data;
  }
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_COLUMNS_TRAITS_H_
