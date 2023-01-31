#ifndef VH_SQLDB_QB_COLUMNS_TRAITS_H_
#define VH_SQLDB_QB_COLUMNS_TRAITS_H_

#include <string>

#include "cpp_for_each_type.h"
#include "sqldb_concepts.h"  // IWYU pragma: keep
#include "sqldb_qb_types.h"

namespace vh::sqldb::qb {
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

    cpp::ForEachType<Columns...>([&names]<typename Column>(Column) {
      names.emplace_back(Column::Type::GetName());
    });

    return names;
  }

  /**
   * @brief Gives data required to create columns.
   */
  static auto GetCreateColumnsData [[nodiscard]] () {
    auto data = std::vector<CreateColumnData>{};

    cpp::ForEachType<Columns...>([&data]<typename Column>(Column) {
      data.emplace_back(CreateColumnData{.name = Column::Type::GetName(),
                                         .type = Column::Type::GetType(),
                                         .unique = Column::Type::IsUnique()});
    });

    return data;
  }

  /**
   * @brief Parses primary keys data from column definitions.
   */
  static auto GetPrimaryKeysData [[nodiscard]] () {
    auto data = std::vector<PrimaryKeyData>{};

    cpp::ForEachType<Columns...>([&data]<typename Column>(Column) {
      if constexpr (Column::Type::IsPrimaryKey()) {
        data.emplace_back(
            PrimaryKeyData{.column_name = Column::Type::GetName(),
                           .auto_increment = Column::Type::HasAutoIncrement()});
      }
    });

    return data;
  }

  /**
   * @brief Parses foreign keys data from column definitions.
   */
  static auto GetForeignKeysData [[nodiscard]] () {
    auto data = std::vector<ForeignKeyData>{};

    cpp::ForEachType<Columns...>([&data]<typename Column>(Column) {
      if constexpr (Column::Type::IsForeignKey()) {
        data.emplace_back(ForeignKeyData{
            .column_name = Column::Type::GetName(),
            .target_table_name = Column::Type::ForeignKey::Table::GetName(),
            .target_column_name = Column::Type::ForeignKey::GetName()});
      }
    });

    return data;
  }

  /**
   * @brief Gives data required to select values from columns.
   */
  static auto GetSelectColumnsData [[nodiscard]] () {
    auto data = std::vector<SelectColumnData>{};

    cpp::ForEachType<Columns...>([&data]<typename Column>(Column) {
      data.emplace_back(
          SelectColumnData{.name = Column::Type::GetName(),
                           .full_name = Column::Type::GetFullName(),
                           .type = Column::Type::GetType()});
    });

    return data;
  }
};
}  // namespace vh::sqldb::qb

#endif  // VH_SQLDB_QB_COLUMNS_TRAITS_H_
