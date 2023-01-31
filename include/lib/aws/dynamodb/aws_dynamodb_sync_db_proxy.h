#ifndef VH_AWS_DYNAMODB_SYNC_DB_PROXY_H_
#define VH_AWS_DYNAMODB_SYNC_DB_PROXY_H_

#include <cppcoro/task.hpp>

#include "aws_dynamodb_async_db.h"
#include "cpp_optional.h"
#include "kvdb_i_db.h"
#include "kvdb_types.h"

namespace vh::aws::dynamodb {
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
  auto CreateTableIfNotExists(const kvdb::Table &table)
      -> cppcoro::task<> override;

  /**
   * @copydoc kvdb::IDb::DropTableIfExists
   */
  auto DropTableIfExists(const kvdb::Table &table) -> cppcoro::task<> override;

  /**
   * @copydoc kvdb::IDb::SelectItem
   */
  auto SelectItem
      [[nodiscard]] (const kvdb::Table &table, const kvdb::Key &key) const
      -> cppcoro::task<cpp::Opt<kvdb::Item>> override;

  /**
   * @copydoc kvdb::IDb::InsertOrUpdateItem
   */
  auto InsertOrUpdateItem(const kvdb::Table &table, kvdb::Item item)
      -> cppcoro::task<> override;

  /**
   * @copydoc kvdb::IDb::DeleteItemIfExists
   */
  auto DeleteItemIfExists(const kvdb::Table &table, const kvdb::Key &key)
      -> cppcoro::task<> override;

 private:
  auto IsItemExists
      [[nodiscard]] (const kvdb::Table &table, const kvdb::Key &key) const
      -> cppcoro::task<bool>;
  auto IsItemExists
      [[nodiscard]] (const kvdb::Table &table, const kvdb::Item &item) const
      -> cppcoro::task<bool>;

  AsyncDb async_db_;
};
}  // namespace vh::aws::dynamodb

#endif  // VH_AWS_DYNAMODB_SYNC_DB_PROXY_H_
