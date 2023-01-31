#ifndef STONKS_AWS_DYNAMODB_AWS_DYNAMODB_ASYNC_DB_H_
#define STONKS_AWS_DYNAMODB_AWS_DYNAMODB_ASYNC_DB_H_

#include <cppcoro/task.hpp>

#include "aws_api_handle.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "kvdb_i_db.h"
#include "kvdb_types.h"

// IWYU pragma: no_include <aws/dynamodb/DynamoDBClient.h>

namespace Aws::DynamoDB {
class DynamoDBClient;  // IWYU pragma: keep

namespace Model {
enum class TableStatus;  // IWYU pragma: keep
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

  /**
   * @brief Checks whether table exists.
   */
  auto IsTableExists [[nodiscard]] (const kvdb::Table &table) const
      -> cppcoro::task<bool>;

  /**
   * @brief Checks whether table exists and available for use.
   */
  auto IsTableReadyForUse [[nodiscard]] (const kvdb::Table &table) const
      -> cppcoro::task<bool>;

  /**
   * @brief Gives a native DynamoDB handle.
   */
  auto GetDynamoDbClient [[nodiscard]] () const
      -> const Aws::DynamoDB::DynamoDBClient &;

 private:
  auto GetTableStatus [[nodiscard]] (const kvdb::Table &table) const
      -> cppcoro::task<cpp::Opt<Aws::DynamoDB::Model::TableStatus>>;

  ApiHandle api_handle_;
  cpp::NnUp<Aws::DynamoDB::DynamoDBClient> db_client_;
};
}  // namespace stonks::aws::dynamodb

#endif  // STONKS_AWS_DYNAMODB_AWS_DYNAMODB_ASYNC_DB_H_
