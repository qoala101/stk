/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_KVDB_TYPES_H_
#define VH_KVDB_TYPES_H_

#include <string>

#include "cpp_typed_struct.h"

namespace vh::kvdb {
/**
 * @brief Name of Key-Value table which stores string key-value pairs.
 */
struct Table : public cpp::TypedStruct<std::string> {};

/**
 * @brief Key-Value table key.
 */
struct Key : public cpp::TypedStruct<std::string> {};

/**
 * @brief Key-Value table item.
 */
struct Item {
  Key key{};
  std::string value{};

 private:
  friend auto operator== [[nodiscard]] (const Item &, const Item &)
  -> bool = default;
};
}  // namespace vh::kvdb

#endif  // VH_KVDB_TYPES_H_
