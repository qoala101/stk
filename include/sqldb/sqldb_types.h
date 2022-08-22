#ifndef STONKS_SQLDB_SQLDB_TYPES_H_
#define STONKS_SQLDB_SQLDB_TYPES_H_

#include <string>
#include <vector>

#include "ccutils_optional.h"
#include "ccutils_views.h"
#include "sqldb_enums.h"
#include "sqldb_value.h"

namespace stonks::sqldb {
using Table = std::string;
using Column = std::string;

struct ForeignKey {
  Table table{};
  Column column{};
};

struct ColumnDefinition {
  Column column{};
  DataType data_type{};
  bool primary_key{};
  bool auto_increment{};
  bool unique{};
  ccutils::Opt<ForeignKey> foreign_key{};
};

struct TableDefinition {
  [[nodiscard]] auto GetColumnDefinition(const Column &column) const
      -> const ColumnDefinition &;
  [[nodiscard]] auto GetColumnDefinition(const Column &column)
      -> ColumnDefinition &;

  [[nodiscard]] auto GetColumnDefinitions(const std::vector<Column> &columns)
      const -> ccutils::ConstView<ColumnDefinition>;

  Table table{};
  std::vector<ColumnDefinition> columns{};

 private:
  [[nodiscard]] static auto GetColumnDefinitionImpl(auto &t,
                                                    const Column &column)
      -> auto &;
};

struct Cell {
  Column column{};
  Value value;
};

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
