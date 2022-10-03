#ifndef STONKS_SQLDB_SQLDB_TYPES_H_
#define STONKS_SQLDB_SQLDB_TYPES_H_

#include <string>
#include <vector>

#include "cpp_optional.h"
#include "cpp_typed_struct.h"
#include "cpp_views.h"
#include "sqldb_enums.h"
#include "sqldb_value.h"

namespace stonks::sqldb {
/**
 * @brief Name of SQL table which has columns with different value types.
 */
struct Table : public cpp::TypedStruct<std::string> {};

/**
 * @brief Name of the SQL table column.
 */
struct Column : public cpp::TypedStruct<std::string> {};

/**
 * @brief SQL query.
 */
struct Query : public cpp::TypedStruct<std::string> {};

/**
 * @brief Foreign key to the column in the table.
 */
struct ForeignKey {
  Table table{};
  Column column{};
};

/**
 * @brief Description of SQL table column.
 */
struct ColumnDefinition {
  Column column{};
  DataType data_type{};
  bool primary_key{};
  bool auto_increment{};
  bool unique{};
  cpp::Opt<ForeignKey> foreign_key{};
};

/**
 * @brief Description of SQL table which is table name
 * and description of its columns.
 */
struct TableDefinition {
  [[nodiscard]] auto GetColumnDefinition(const Column &column) const
      -> const ColumnDefinition &;
  [[nodiscard]] auto GetColumnDefinition(const Column &column)
      -> ColumnDefinition &;

  [[nodiscard]] auto GetColumnDefinitions(const std::vector<Column> &columns)
      const -> cpp::ConstView<ColumnDefinition>;

  Table table{};
  std::vector<ColumnDefinition> columns{};

 private:
  template <cpp::DecaysTo<TableDefinition> This>
  [[nodiscard]] static auto GetColumnDefinitionImpl(This &t,
                                                    const Column &column)
      -> auto &;
};

/**
 * @brief Value of single column cell.
 */
struct Cell {
  Column column{};
  Value value{};
};

/**
 * @brief Description of SQL table cell.
 */
struct CellDefinition {
  Column column{};
  DataType data_type{};

 private:
  [[nodiscard]] friend auto operator==(const CellDefinition &,
                                       const CellDefinition &)
      -> bool = default;
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_TYPES_H_
