#ifndef STONKS_SQLDB_SQLDB_TYPES_H_
#define STONKS_SQLDB_SQLDB_TYPES_H_

#include <any>
#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

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
  std::optional<ForeignKey> foreign_key{};
};

struct TableDefinition {
  Table table{};
  std::vector<ColumnDefinition> columns{};
};

struct Cell {
  Column column{};
  Value value;
};

struct CellDefinition {
  Column column{};
  DataType data_type{};
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_TYPES_H_
