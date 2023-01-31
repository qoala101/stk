#ifndef VH_KVDB_I_ITEMS_INTERFACE_H_
#define VH_KVDB_I_ITEMS_INTERFACE_H_

#include <cppcoro/task.hpp>

#include "cpp_optional.h"
#include "kvdb_types.h"

namespace vh::kvdb {
/**
 * @brief Generic Key-Value DB interface to manipulate the items.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class IItemsInterface {
 public:
  virtual ~IItemsInterface() = default;
  /**
   * @brief Selects the item with the specified key.
   * @return Nullopt if there is no item with such key.
   */
  virtual auto SelectItem
      [[nodiscard]] (const Table &table, const Key &key) const
      -> cppcoro::task<cpp::Opt<Item>> = 0;

  /**
   * @brief Sets the item value for the specified key.
   */
  virtual auto InsertOrUpdateItem(const Table &table, Item item)
      -> cppcoro::task<> = 0;

  /**
   * @brief Deletes the item with the specified key.
   */
  virtual auto DeleteItemIfExists(const Table &table, const Key &key)
      -> cppcoro::task<> = 0;
};
}  // namespace vh::kvdb

#endif  // VH_KVDB_I_ITEMS_INTERFACE_H_
