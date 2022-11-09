#ifndef STONKS_SQLDB_SQLDB_TABLE_H_
#define STONKS_SQLDB_SQLDB_TABLE_H_

#include <nameof.hpp>
#include <string>
#include <string_view>

#include "sqldb_concepts.h"  // IWYU pragma: keep
#include "sqldb_types.h"

namespace stonks::sqldb {
namespace detail {
auto GetFullNameInTableImpl
    [[nodiscard]] (std::string_view table_name, std::string_view column_name)
    -> std::string;
}  // namespace detail

/**
 * @brief Base for table definition types which provides API.
 */
template <typename TableT>
class Table {
 public:
  /**
   * @brief Base for column definition types which provides API.
   */
  template <SupportedDataType DataTypeT, typename ColumnT>
  class Column {
   public:
    /**
     * @brief Parent table.
     */
    using Table = TableT;

    /**
     * @brief Data type.
     */
    using DataType = DataTypeT;

    Column() = delete;

    /**
     * @brief Gives short column name which is the name of its type.
     */
    static auto GetName [[nodiscard]] () {
      return std::string{nameof::nameof_short_type<ColumnT>()};
    }

    /**
     * @brief Gives full column name which includes its table name.
     */
    static auto GetFullName [[nodiscard]] () {
      return GetFullNameInTable<Table>();
    }

    /**
     * @brief Gives column data type.
     */
    static auto GetType [[nodiscard]] () {
      return DataTypeVariant{::stonks::sqldb::DataType<DataType>{}};
    }

    /**
     * @brief Tells whether column is part of the primary key.
     */
    static consteval auto IsPrimaryKey [[nodiscard]] () {
      return requires { typename ColumnT::PrimaryKey; };
    }

    /**
     * @brief Tells whether column is foreign key.
     */
    static consteval auto IsForeignKey [[nodiscard]] () {
      return requires { typename ColumnT::ForeignKey; };
    }

    /**
     * @brief Tells whether column has auto increment.
     */
    static auto HasAutoIncrement [[nodiscard]] () {
      return requires { typename ColumnT::AutoIncrement; };
    }

    /**
     * @brief Tells whether column data is unique.
     */
    static auto IsUnique [[nodiscard]] () {
      return requires { typename ColumnT::Unique; };
    }

   protected:
    template <TableDefinition Table>
    static auto GetFullNameInTable [[nodiscard]] () {
      return detail::GetFullNameInTableImpl(Table::GetName(), GetName());
    }
  };

  Table() = delete;

  /**
   * @brief Gives table name which is the name of the type.
   */
  static auto GetName [[nodiscard]] () {
    return std::string{nameof::nameof_short_type<TableT>()};
  }
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_TABLE_H_
