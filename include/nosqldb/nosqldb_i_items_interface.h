#ifndef STONKS_NOSQLDB_NOSQLDB_I_ITEMS_INTERFACE_H_
#define STONKS_NOSQLDB_NOSQLDB_I_ITEMS_INTERFACE_H_

#include "cpp_optional.h"
#include "nosqldb_types.h"

namespace stonks::nosqldb {
/**
 * @brief Generic NoSQL DB interface to manipulate the items.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class IItemsInterface {
 public:
  virtual ~IItemsInterface() = default;
  /**
   * @brief Selects the item with the specified key.
   * @return Nullopt if there is no item with such key.
   */
  [[nodiscard]] virtual auto SelectItem(const Table &table,
                                        const Key &key) const
      -> cpp::Opt<Value> = 0;

  /**
   * @brief Sets the item value for the specified key.
   */
  virtual void InsertOrUpdateItem(const Table &table, const Key &key,
                                  const Value &value) = 0;

  /**
   * @brief Deletes the item with the specified key.
   */
  virtual void DeleteItemIfExists(const Table &table, const Key &key) = 0;
};
}  // namespace stonks::nosqldb

#endif  // STONKS_NOSQLDB_NOSQLDB_I_ITEMS_INTERFACE_H_
