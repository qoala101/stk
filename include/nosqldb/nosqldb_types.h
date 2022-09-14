#ifndef STONKS_NOSQLDB_NOSQLDB_TYPES_H_
#define STONKS_NOSQLDB_NOSQLDB_TYPES_H_

#include <string>

#include "cpp_typed_struct.h"

/**
 * @file Common NoSQL DB related types.
 */

namespace stonks::nosqldb {
struct Table : public cpp::TypedStruct<std::string> {};
struct Key : public cpp::TypedStruct<std::string> {};

/**
 * @brief NoSQL table item.
 */
struct Item {
  Key key{};
  std::string value{};

 private:
  [[nodiscard]] friend auto operator==(const Item &, const Item &)
      -> bool = default;
};
}  // namespace stonks::nosqldb

#endif  // STONKS_NOSQLDB_NOSQLDB_TYPES_H_
