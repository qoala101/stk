#ifndef STONKS_AWS_DYNAMODB_AWS_DYNAMODB_FACTORY_H_
#define STONKS_AWS_DYNAMODB_AWS_DYNAMODB_FACTORY_H_

#include "cpp_not_null.h"
#include "nosqldb_i_db.h"
#include "nosqldb_i_factory.h"
#include "nosqldb_i_items_interface.h"
#include "nosqldb_i_tables_interface.h"

namespace stonks::aws::dynamodb {
/**
 * @copydoc nosqldb::IFactory
 */
class Factory : public nosqldb::IFactory {
 public:
  /**
   * @copydoc nosqldb::IFactory::CreateTablesInterface
   */
  [[nodiscard]] auto CreateTablesInterface() const
      -> cpp::NnUp<nosqldb::ITablesInterface> override;

  /**
   * @copydoc nosqldb::IFactory::CreateItemsInterface
   */
  [[nodiscard]] auto CreateItemsInterface() const
      -> cpp::NnUp<nosqldb::IItemsInterface> override;

  /**
   * @copydoc nosqldb::IFactory::CreateDb
   */
  [[nodiscard]] auto CreateDb() const -> cpp::NnUp<nosqldb::IDb> override;
};
}  // namespace stonks::aws::dynamodb

#endif  // STONKS_AWS_DYNAMODB_AWS_DYNAMODB_FACTORY_H_
