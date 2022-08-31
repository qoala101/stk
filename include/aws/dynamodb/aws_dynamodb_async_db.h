#ifndef STONKS_AWS_DYNAMODB_AWS_DYNAMODB_ASYNC_DB_H_
#define STONKS_AWS_DYNAMODB_AWS_DYNAMODB_ASYNC_DB_H_

#include <string_view>
#include <vector>

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
 * @brief Handle to AWS DynamoDB.
 * Access is defined by the user credentials specified in the current
 * environment.
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
   * @copydoc nosqldb::IDb::DropTable
   */
  void DropTable(const nosqldb::Table &table) override;

  /**
   * @copydoc nosqldb::IDb::SelectItem
   */
  [[nodiscard]] auto SelectItem(const nosqldb::Table &table,
                                const nosqldb::Key &key) const
      -> cpp::Opt<nosqldb::Value> override;

  /**
   * @copydoc nosqldb::IDb::InsertOrUpdateItem
   */
  void InsertOrUpdateItem(const nosqldb::Table &table, const nosqldb::Key &key,
                          const nosqldb::Value &value) override;

  /**
   * @copydoc nosqldb::IDb::DeleteItem
   */
  void DeleteItem(const nosqldb::Table &table,
                  const nosqldb::Key &key) override;

  /**
   * @copydoc nosqldb::IDb::WriteToUri
   */
  void WriteToUri(std::string_view uri) const override;

 private:
  [[nodiscard]] auto GetExistingTables() const -> std::vector<nosqldb::Table>;

  [[nodiscard]] auto IsTableExists(const nosqldb::Table &table) const -> bool;

  // [[nodiscard]] auto IsTableActive(const nosqldb::Table &table) const -> bool;

  // [[nodiscard]] auto GetTableStatus(const nosqldb::Table &table) const
  //     -> Aws::DynamoDB::Model::TableStatus;

  cpp::NnSp<ApiHandle> api_handle_;
  cpp::NnUp<Aws::DynamoDB::DynamoDBClient> db_client_;
};
}  // namespace stonks::aws::dynamodb

#endif  // STONKS_AWS_DYNAMODB_AWS_DYNAMODB_ASYNC_DB_H_
