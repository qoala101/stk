#ifndef STONKS_AWS_DYNAMODB_AWS_DYNAMODB_ASYNC_DB_H_
#define STONKS_AWS_DYNAMODB_AWS_DYNAMODB_ASYNC_DB_H_

#include "aws_api_handle.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "kvdb_i_db.h"
#include "kvdb_types.h"

namespace Aws::DynamoDB {
class DynamoDBClient;

namespace Model {
enum class TableStatus;
}  // namespace Model
}  // namespace Aws::DynamoDB

namespace stonks::aws::dynamodb {
/**
 * @brief Implementation of Key-Value DB interface using DynamoDB from AWS.
 * @remark All operations are asynchronous. Calling one operation immediately
 * after another might lead to unexpected results, because the AWS server might
 * not yet processed the preceding one.
 * @remark Access is defined by the user credentials specified
 * in the current environment following the guide:
 * https://docs.aws.amazon.com/sdk-for-cpp/v1/developer-guide/credentials.html
 */
class AsyncDb : public kvdb::IDb {
 public:
  explicit AsyncDb(ApiHandle api_handle);

  AsyncDb(const AsyncDb &) = delete;
  AsyncDb(AsyncDb &&) noexcept;

  auto operator=(const AsyncDb &) -> AsyncDb & = delete;
  auto operator=(AsyncDb &&) noexcept -> AsyncDb &;

  ~AsyncDb() override;

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

  /**
   * @brief Gives a native DynamoDB handle.
   */
  auto GetDynamoDbClient [[nodiscard]] () const
      -> const Aws::DynamoDB::DynamoDBClient &;

 private:
  ApiHandle api_handle_;
  cpp::NnUp<Aws::DynamoDB::DynamoDBClient> db_client_;
};
}  // namespace stonks::aws::dynamodb

#endif  // STONKS_AWS_DYNAMODB_AWS_DYNAMODB_ASYNC_DB_H_
