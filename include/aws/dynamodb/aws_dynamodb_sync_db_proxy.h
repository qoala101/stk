#ifndef STONKS_AWS_DYNAMODB_AWS_DYNAMODB_SYNC_DB_PROXY_H_
#define STONKS_AWS_DYNAMODB_AWS_DYNAMODB_SYNC_DB_PROXY_H_

#include "aws_dynamodb_async_db.h"
#include "cpp_optional.h"
#include "kvdb_i_db.h"
#include "kvdb_types.h"

namespace stonks::aws::dynamodb {
/**
 * @brief Synchronization proxy to the asynchronous DynamoDB implementation.
 * Blocks after each operation until the result of it can be verified.
 */
class SyncDbProxy : public kvdb::IDb {
 public:
  /**
   * @param async_db_ Proxied DB.
   */
  explicit SyncDbProxy(AsyncDb async_db_);

  /**
   * @copydoc kvdb::IDb::CreateTableIfNotExists
   */
  void CreateTableIfNotExists(const kvdb::Table &table) override;

  /**
   * @copydoc kvdb::IDb::DropTableIfExists
   */
  void DropTableIfExists(const kvdb::Table &table) override;

  /**
   * @copydoc kvdb::IDb::SelectItem
   */
  auto SelectItem
      [[nodiscard]] (const kvdb::Table &table, const kvdb::Key &key) const
      -> cpp::Opt<kvdb::Item> override;

  /**
   * @copydoc kvdb::IDb::InsertOrUpdateItem
   */
  void InsertOrUpdateItem(const kvdb::Table &table, kvdb::Item item) override;

  /**
   * @copydoc kvdb::IDb::DeleteItemIfExists
   */
  void DeleteItemIfExists(const kvdb::Table &table,
                          const kvdb::Key &key) override;

 private:
  auto GetTableStatus [[nodiscard]] (const kvdb::Table &table) const
      -> cpp::Opt<Aws::DynamoDB::Model::TableStatus>;

  auto IsTableExists [[nodiscard]] (const kvdb::Table &table) const;

  auto IsTableReadyForUse [[nodiscard]] (const kvdb::Table &table) const;

  auto IsItemExists
      [[nodiscard]] (const kvdb::Table &table, const kvdb::Key &key) const;
  auto IsItemExists
      [[nodiscard]] (const kvdb::Table &table, const kvdb::Item &item) const;

  AsyncDb async_db_;
};
}  // namespace stonks::aws::dynamodb

#endif  // STONKS_AWS_DYNAMODB_AWS_DYNAMODB_SYNC_DB_PROXY_H_
