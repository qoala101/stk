#ifndef STONKS_NOSQLDB_NOSQLDB_TYPES_H_
#define STONKS_NOSQLDB_NOSQLDB_TYPES_H_

#include <string>

/**
 * @file Common NoSQL DB related types.
 */

namespace stonks::nosqldb {
using Table = std::string;
using Key = std::string;

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
