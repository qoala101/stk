#include "aws_dynamodb_sync_db_proxy.h"

#include <aws/core/utils/Outcome.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/DynamoDBErrors.h>
#include <aws/dynamodb/model/DescribeTableRequest.h>
#include <aws/dynamodb/model/DescribeTableResult.h>
#include <aws/dynamodb/model/TableDescription.h>
#include <aws/dynamodb/model/TableStatus.h>
#include <fmt/core.h>

#include <gsl/assert>
#include <optional>
#include <utility>

#include "aws_dynamodb_async_db.h"
#include "cpp_message_exception.h"
#include "cpp_optional.h"
#include "nosqldb_types.h"

namespace stonks::aws::dynamodb {
namespace {
template <std::predicate Predicate>
void WaitUntil(const Predicate &predicate) {
  while (!predicate()) {
  }

  Ensures(predicate());
}
}  // namespace

SyncDbProxy::SyncDbProxy(AsyncDb async_db) : async_db_{std::move(async_db)} {}

auto SyncDbProxy::IsTableExists(const nosqldb::Table &table) const {
  return GetTableStatus(table).has_value();
}

auto SyncDbProxy::IsTableReadyForUse(const nosqldb::Table &table) const {
  const auto status = GetTableStatus(table);

  if (!status.has_value()) {
    return false;
  }

  return *status == Aws::DynamoDB::Model::TableStatus::ACTIVE;
}

void SyncDbProxy::CreateTableIfNotExists(const nosqldb::Table &table) {
  async_db_.CreateTableIfNotExists(table);

  WaitUntil([this, &table]() { return IsTableReadyForUse(table); });
  Ensures(IsTableReadyForUse(table));
}

void SyncDbProxy::DropTableIfExists(const nosqldb::Table &table) {
  async_db_.DropTableIfExists(table);

  WaitUntil([this, &table]() { return !IsTableExists(table); });
  Ensures(!IsTableExists(table));
}

auto SyncDbProxy::SelectItem(const nosqldb::Table &table,
                             const nosqldb::Key &key) const
    -> cpp::Opt<nosqldb::Item> {
  return async_db_.SelectItem(table, key);
}

auto SyncDbProxy::IsItemExists(const nosqldb::Table &table,
                               const nosqldb::Key &key) const {
  return async_db_.SelectItem(table, key).has_value();
}

auto SyncDbProxy::IsItemExists(const nosqldb::Table &table,
                               const nosqldb::Item &item) const {
  const auto selected_item = async_db_.SelectItem(table, item.key);
  return selected_item.has_value() && (*selected_item == item);
}

void SyncDbProxy::InsertOrUpdateItem(const nosqldb::Table &table,
                                     nosqldb::Item item) {
  async_db_.InsertOrUpdateItem(table, item);

  WaitUntil([this, &table, &item]() { return IsItemExists(table, item); });
  Ensures(IsItemExists(table, item));
}

void SyncDbProxy::DeleteItemIfExists(const nosqldb::Table &table,
                                     const nosqldb::Key &key) {
  async_db_.DeleteItemIfExists(table, key);

  WaitUntil([this, &table, &key]() { return !IsItemExists(table, key); });
  Ensures(!IsItemExists(table, key));
}

auto SyncDbProxy::GetTableStatus(const nosqldb::Table &table) const
    -> cpp::Opt<Aws::DynamoDB::Model::TableStatus> {
  const auto request =
      Aws::DynamoDB::Model::DescribeTableRequest{}.WithTableName(table);

  const auto result = async_db_.GetDynamoDbClient().DescribeTable(request);

  if (!result.IsSuccess()) {
    if (const auto table_doesnt_exist =
            result.GetError().GetErrorType() ==
            Aws::DynamoDB::DynamoDBErrors::RESOURCE_NOT_FOUND) {
      return std::nullopt;
    }

    throw cpp::MessageException{fmt::format("Couldn't get table status {}: {}",
                                            *table,
                                            result.GetError().GetMessage())};
  }

  return result.GetResult().GetTable().GetTableStatus();
}
}  // namespace stonks::aws::dynamodb