#ifndef STONKS_SQLDB_SQLDB_TABLE_H_
#define STONKS_SQLDB_SQLDB_TABLE_H_

#include <nameof.hpp>

#include "sqldb_concepts.h"  // IWYU pragma: keep
#include "sqldb_types.h"

namespace stonks::sqldb {
namespace detail {
[[nodiscard]] auto GetFullNameInTableImpl(std::string_view table_name,
                                          std::string_view column_name)
    -> std::string;
}  // namespace detail

/**
 * @brief Base for table definition types which provides API.
 */
template <typename TableT>
class Table {
 public:
  Table() = delete;

  /**
   * @brief Gives table name which is the name of the type.
   */
  [[nodiscard]] static auto GetName() {
    return std::string{nameof::nameof_short_type<TableT>()};
  }

  /**
   * @brief Base for column definition types which provides API.
   */
  template <SupportedDataType DataTypeT, typename ColumnT>
  class Column {
   public:
    Column() = delete;

    /**
     * @brief Parent table.
     */
    using Table = TableT;

    /**
     * @brief Data type.
     */
    using DataType = DataTypeT;

    /**
     * @brief Gives short column name which is the name of its type.
     */
    [[nodiscard]] static auto GetName() {
      return std::string{nameof::nameof_short_type<ColumnT>()};
    }

    /**
     * @brief Gives full column name which includes its table name.
     */
    [[nodiscard]] static auto GetFullName() {
      return GetFullNameInTable<Table>();
    }

    /**
     * @brief Gives column data type.
     */
    [[nodiscard]] static auto GetType() {
      return DataTypeVariant{::stonks::sqldb::DataType<DataType>{}};
    }

    /**
     * @brief Tells whether column is part of the primary key.
     */
    [[nodiscard]] static consteval auto IsPrimaryKey() {
      return requires { typename ColumnT::PrimaryKey; };
    }

    /**
     * @brief Tells whether column is foreign key.
     */
    [[nodiscard]] static consteval auto IsForeignKey() {
      return requires { typename ColumnT::ForeignKey; };
    }

    /**
     * @brief Tells whether column has auto increment.
     */
    [[nodiscard]] static auto HasAutoIncrement() {
      return requires { typename ColumnT::AutoIncrement; };
    }

    /**
     * @brief Tells whether column data is unique.
     */
    [[nodiscard]] static auto IsUnique() {
      return requires { typename ColumnT::Unique; };
    }

   protected:
    template <TableDefinition Table>
    [[nodiscard]] static auto GetFullNameInTable() {
      return detail::GetFullNameInTableImpl(Table::GetName(), GetName());
    }
  };
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_TABLE_H_
