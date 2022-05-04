#ifndef STONKS_DB_DB_TYPES_H_
#define STONKS_DB_DB_TYPES_H_

#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "db_enums.h"

namespace stonks::db {
struct DbError {
  std::string message{};
};

struct Table {
  std::string name{};

 private:
  friend bool operator==(const Table &, const Table &) = default;
};

struct Value {
  std::variant<int64_t, double, std::string> value{};

  std::optional<int64_t> GetInt64() const;
  std::optional<double> GetDouble() const;
  std::optional<std::string> GetString() const;

  std::string ToString() const;

  static Value FromString(std::string_view value, DataType data_type);
};

struct ForeignKey {
  std::string table_name{};
  std::string column_name{};

 private:
  friend bool operator==(const ForeignKey &, const ForeignKey &) = default;
};

struct Column {
  std::string name{};
  DataType data_type{};
  bool primary_key{};
  bool auto_increment{};
  bool unique{};
  std::optional<ForeignKey> foreign_key{};

 private:
  friend bool operator==(const Column &, const Column &) = default;
};

struct TableDefinition {
  Table table{};
  std::vector<Column> columns{};

 private:
  friend bool operator==(const TableDefinition &,
                         const TableDefinition &) = default;
};

struct Cell {
  std::string column_name{};
  Value value{};
};

struct Row {
  std::vector<Cell> cells{};
};
}  // namespace stonks::db

#endif  // STONKS_DB_DB_TYPES_H_
