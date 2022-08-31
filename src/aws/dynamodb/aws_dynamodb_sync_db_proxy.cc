#include "aws_dynamodb_sync_db_proxy.h"

#include <aws/core/utils/Outcome.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/DynamoDBErrors.h>
#include <aws/dynamodb/model/DescribeTableRequest.h>
#include <aws/dynamodb/model/DescribeTableResult.h>
#include <aws/dynamodb/model/TableDescription.h>
#include <aws/dynamodb/model/TableStatus.h>

#include <functional>
#include <gsl/assert>
#include <optional>
#include <string>
#include <utility>

#include "aws_dynamodb_async_db.h"
#include "cpp_message_exception.h"
#include "cpp_optional.h"
#include "nosqldb_types.h"

namespace stonks::aws::dynamodb {
namespace {
void WaitUntil(const std::function<bool()> &predicate) {
  while (!predicate()) {
  }

  Ensures(predicate);
}
}  // namespace

SyncDbProxy::SyncDbProxy(AsyncDb async_db) : async_db_{std::move(async_db)} {}

SyncDbProxy::SyncDbProxy(SyncDbProxy &&) noexcept = default;

auto SyncDbProxy::operator=(SyncDbProxy &&) noexcept -> SyncDbProxy & = default;

SyncDbProxy::~SyncDbProxy() = default;

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
    -> cpp::Opt<nosqldb::Value> {
  return async_db_.SelectItem(table, key);
}

void SyncDbProxy::InsertOrUpdateItem(const nosqldb::Table &table,
                                     const nosqldb::Key &key,
                                     const nosqldb::Value &value) {
  async_db_.InsertOrUpdateItem(table, key, value);

  WaitUntil([this, &table, &key, &value]() {
    return IsItemWithValueExists(table, key, value);
  });
  Ensures(IsItemWithValueExists(table, key, value));
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

    throw cpp::MessageException{"Couldn't get table status " + table + ": " +
                                result.GetError().GetMessage()};
  }

  return result.GetResult().GetTable().GetTableStatus();
}

auto SyncDbProxy::IsTableExists(const nosqldb::Table &table) const -> bool {
  return GetTableStatus(table).has_value();
}

auto SyncDbProxy::IsTableReadyForUse(const nosqldb::Table &table) const
    -> bool {
  const auto status = GetTableStatus(table);

  if (!status.has_value()) {
    return false;
  }

  return *status == Aws::DynamoDB::Model::TableStatus::ACTIVE;
}

auto SyncDbProxy::IsItemExists(const nosqldb::Table &table,
                               const nosqldb::Key &key) const -> bool {
  return async_db_.SelectItem(table, key).has_value();
}

auto SyncDbProxy::IsItemWithValueExists(const nosqldb::Table &table,
                                        const nosqldb::Key &key,
                                        const nosqldb::Value &value) const
    -> bool {
  const auto item = async_db_.SelectItem(table, key);
  return item.has_value() && (*item == value);
}
}  // namespace stonks::aws::dynamodb