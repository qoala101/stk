#ifndef STONKS_SQLDB_SQLDB_TYPES_H_
#define STONKS_SQLDB_SQLDB_TYPES_H_

#include <gsl/pointers>
#include <optional>
#include <string>
#include <vector>

#include "sqldb_enums.h"
#include "sqldb_value.h"

namespace stonks::sqldb {
using Table = std::string;
using Column = std::string;

template <typename T>
using View = std::vector<gsl::strict_not_null<T *>>;

template <typename T>
using ConstView = std::vector<gsl::strict_not_null<const T *>>;

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
  [[nodiscard]] auto GetColumnDefinition(const Column &column) const
      -> const ColumnDefinition &;
  [[nodiscard]] auto GetColumnDefinitions(
      const std::vector<Column> &columns) const -> ConstView<ColumnDefinition>;

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

 private:
  friend auto operator==(const CellDefinition &, const CellDefinition &)
      -> bool = default;
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_TYPES_H_
