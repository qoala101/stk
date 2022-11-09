#ifndef STONKS_AWS_DYNAMODB_AWS_DYNAMODB_SYNC_DB_PROXY_H_
#define STONKS_AWS_DYNAMODB_AWS_DYNAMODB_SYNC_DB_PROXY_H_

#include "aws_dynamodb_async_db.h"
#include "cpp_optional.h"
#include "nosqldb_i_db.h"
#include "nosqldb_types.h"

namespace Aws::DynamoDB::Model {
enum class TableStatus;
}  // namespace Aws::DynamoDB::Model

namespace stonks::aws::dynamodb {
/**
 * @brief Synchronization proxy to the asynchronous DynamoDB implementation.
 * Blocks after each operation until the result of it can be verified.
 */
class SyncDbProxy : public nosqldb::IDb {
 public:
  /**
   * @param async_db_ Proxied DB.
   */
  explicit SyncDbProxy(AsyncDb async_db_);

  /**
   * @copydoc nosqldb::IDb::CreateTableIfNotExists
   */
  void CreateTableIfNotExists(const nosqldb::Table &table) override;

  /**
   * @copydoc nosqldb::IDb::DropTableIfExists
   */
  void DropTableIfExists(const nosqldb::Table &table) override;

  /**
   * @copydoc nosqldb::IDb::SelectItem
   */
  auto SelectItem
      [[nodiscard]] (const nosqldb::Table &table, const nosqldb::Key &key) const
      -> cpp::Opt<nosqldb::Item> override;

  /**
   * @copydoc nosqldb::IDb::InsertOrUpdateItem
   */
  void InsertOrUpdateItem(const nosqldb::Table &table,
                          nosqldb::Item item) override;

  /**
   * @copydoc nosqldb::IDb::DeleteItemIfExists
   */
  void DeleteItemIfExists(const nosqldb::Table &table,
                          const nosqldb::Key &key) override;

 private:
  auto GetTableStatus [[nodiscard]] (const nosqldb::Table &table) const
      -> cpp::Opt<Aws::DynamoDB::Model::TableStatus>;

  auto IsTableExists [[nodiscard]] (const nosqldb::Table &table) const;

  auto IsTableReadyForUse [[nodiscard]] (const nosqldb::Table &table) const;

  auto IsItemExists [[nodiscard]] (const nosqldb::Table &table,
                                   const nosqldb::Key &key) const;
  auto IsItemExists [[nodiscard]] (const nosqldb::Table &table,
                                   const nosqldb::Item &item) const;

  AsyncDb async_db_;
};
}  // namespace stonks::aws::dynamodb

#endif  // STONKS_AWS_DYNAMODB_AWS_DYNAMODB_SYNC_DB_PROXY_H_
