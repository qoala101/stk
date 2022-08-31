#ifndef STONKS_AWS_DYNAMODB_AWS_DYNAMODB_ASYNC_DB_H_
#define STONKS_AWS_DYNAMODB_AWS_DYNAMODB_ASYNC_DB_H_

#include "aws_api_handle.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "nosqldb_i_db.h"
#include "nosqldb_types.h"

namespace Aws::DynamoDB {
class DynamoDBClient;

namespace Model {
enum class TableStatus;
}  // namespace Model
}  // namespace Aws::DynamoDB

namespace stonks::aws::dynamodb {
/**
 * @brief Implementation of NoSQL DB interface using DynamoDB from AWS.
 * @remark All operations are asynchronous. Calling one operation immediately
 * after another might lead to unexpected results, because the AWS server might
 * not yet processed the preceding one.
 * @remark Access is defined by the user credentials specified
 * in the current environment following the guide:
 * https://docs.aws.amazon.com/sdk-for-cpp/v1/developer-guide/credentials.html
 */
class AsyncDb : public nosqldb::IDb {
 public:
  explicit AsyncDb(cpp::NnSp<ApiHandle> api_handle);

  AsyncDb(const AsyncDb &) = delete;
  AsyncDb(AsyncDb &&) noexcept;

  auto operator=(const AsyncDb &) -> AsyncDb & = delete;
  auto operator=(AsyncDb &&) noexcept -> AsyncDb &;

  ~AsyncDb() override;

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
  [[nodiscard]] auto SelectItem(const nosqldb::Table &table,
                                const nosqldb::Key &key) const
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

  /**
   * @brief Gives a native DynamoDB handle.
   */
  [[nodiscard]] auto GetDynamoDbClient() const
      -> const Aws::DynamoDB::DynamoDBClient &;

 private:
  cpp::NnSp<ApiHandle> api_handle_;
  cpp::NnUp<Aws::DynamoDB::DynamoDBClient> db_client_;
};
}  // namespace stonks::aws::dynamodb

#endif  // STONKS_AWS_DYNAMODB_AWS_DYNAMODB_ASYNC_DB_H_
