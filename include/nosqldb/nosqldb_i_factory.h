#ifndef STONKS_NOSQLDB_NOSQLDB_I_FACTORY_H_
#define STONKS_NOSQLDB_NOSQLDB_I_FACTORY_H_

#include "cpp_not_null.h"
#include "nosqldb_i_db.h"
#include "nosqldb_i_items_interface.h"
#include "nosqldb_i_tables_interface.h"

namespace stonks::nosqldb {
/**
 * @brief Abstract factory for NoSQL DB related entities.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class IFactory {
 public:
  virtual ~IFactory() = default;

  /**
   * @brief Creates an object which can manipulate tables.
   */
  [[nodiscard]] virtual auto CreateTablesInterface() const
      -> cpp::NnUp<ITablesInterface> = 0;

  /**
   * @brief Creates an object which can manipulate items.
   */
  [[nodiscard]] virtual auto CreateItemsInterface() const
      -> cpp::NnUp<IItemsInterface> = 0;

  /**
   * @brief Creates an object with the full list of DB actions.
   */
  [[nodiscard]] virtual auto CreateDb() const -> cpp::NnUp<IDb>;
};
}  // namespace stonks::nosqldb

#endif  // STONKS_NOSQLDB_NOSQLDB_I_FACTORY_H_
