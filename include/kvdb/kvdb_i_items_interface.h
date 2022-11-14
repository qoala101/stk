#ifndef STONKS_KVDB_KVDB_I_ITEMS_INTERFACE_H_
#define STONKS_KVDB_KVDB_I_ITEMS_INTERFACE_H_

#include "cpp_optional.h"
#include "kvdb_types.h"

namespace stonks::kvdb {
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
      -> cpp::Opt<Item> = 0;

  /**
   * @brief Sets the item value for the specified key.
   */
  virtual void InsertOrUpdateItem(const Table &table, Item item) = 0;

  /**
   * @brief Deletes the item with the specified key.
   */
  virtual void DeleteItemIfExists(const Table &table, const Key &key) = 0;
};
}  // namespace stonks::kvdb

#endif  // STONKS_KVDB_KVDB_I_ITEMS_INTERFACE_H_
