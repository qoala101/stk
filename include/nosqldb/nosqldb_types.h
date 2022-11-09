#ifndef STONKS_NOSQLDB_NOSQLDB_TYPES_H_
#define STONKS_NOSQLDB_NOSQLDB_TYPES_H_

#include <string>

#include "cpp_typed_struct.h"

namespace stonks::nosqldb {
/**
 * @brief Name of NoSQL table which stores string key-value pairs.
 */
struct Table : public cpp::TypedStruct<std::string> {};

/**
 * @brief NoSQL table key.
 */
struct Key : public cpp::TypedStruct<std::string> {};

/**
 * @brief NoSQL table item.
 */
struct Item {
  Key key{};
  std::string value{};

 private:
  friend auto operator== [[nodiscard]] (const Item &, const Item &)
  -> bool = default;
};
}  // namespace stonks::nosqldb

#endif  // STONKS_NOSQLDB_NOSQLDB_TYPES_H_
