/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "aws_dynamodb_async_db.h"

#include <aws/core/utils/Outcome.h>
#include <aws/core/utils/memory/stl/AWSMap.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/DynamoDBErrors.h>
#include <aws/dynamodb/model/AttributeDefinition.h>
#include <aws/dynamodb/model/AttributeValue.h>
#include <aws/dynamodb/model/CreateTableRequest.h>
#include <aws/dynamodb/model/DeleteItemRequest.h>
#include <aws/dynamodb/model/DeleteTableRequest.h>
#include <aws/dynamodb/model/DescribeTableRequest.h>
#include <aws/dynamodb/model/DescribeTableResult.h>
#include <aws/dynamodb/model/GetItemRequest.h>
#include <aws/dynamodb/model/GetItemResult.h>
#include <aws/dynamodb/model/KeySchemaElement.h>
#include <aws/dynamodb/model/KeyType.h>
#include <aws/dynamodb/model/ProvisionedThroughput.h>
#include <aws/dynamodb/model/ScalarAttributeType.h>
#include <aws/dynamodb/model/TableDescription.h>
#include <aws/dynamodb/model/TableStatus.h>
#include <aws/dynamodb/model/UpdateItemRequest.h>
#include <fmt/core.h>

#include <coroutine>
#include <cppcoro/task.hpp>
#include <map>
#include <not_null.hpp>
#include <optional>
#include <string>
#include <utility>

#include "aws_api_handle.h"
#include "aws_dynamodb_call_as_coroutine.h"
#include "cpp_message_exception.h"
#include "cpp_typed_struct.h"

namespace vh::aws::dynamodb {
AsyncDb::AsyncDb(ApiHandle api_handle)
    : api_handle_{std::move(api_handle)},
      db_client_{cpp::MakeNnUp<Aws::DynamoDB::DynamoDBClient>()} {}

AsyncDb::AsyncDb(AsyncDb &&) noexcept = default;

auto AsyncDb::operator=(AsyncDb &&) noexcept -> AsyncDb & = default;

AsyncDb::~AsyncDb() = default;

auto AsyncDb::CreateTableIfNotExists(const kvdb::Table &table)
    -> cppcoro::task<> {
  if (co_await IsTableReadyForUse(table)) {
    co_return;
  }

  const auto attribute =
      Aws::DynamoDB::Model::AttributeDefinition{}
          .WithAttributeName("Key")
          .WithAttributeType(Aws::DynamoDB::Model::ScalarAttributeType::S);

  const auto key_schema = Aws::DynamoDB::Model::KeySchemaElement{}
                              .WithAttributeName("Key")
                              .WithKeyType(Aws::DynamoDB::Model::KeyType::HASH);

  const auto throughput = Aws::DynamoDB::Model::ProvisionedThroughput{}
                              .WithReadCapacityUnits(1)
                              .WithWriteCapacityUnits(1);

  const auto request = Aws::DynamoDB::Model::CreateTableRequest{}
                           .WithTableName(table)
                           .AddAttributeDefinitions(attribute)
                           .AddKeySchema(key_schema)
                           .WithProvisionedThroughput(throughput);

  const auto result = co_await CallAsCoroutine<
      &Aws::DynamoDB::DynamoDBClient::CreateTableAsync>(*db_client_, request);

  if (!result.IsSuccess()) {
    const auto &error = result.GetError();

    if (const auto table_already_exists =
            error.GetErrorType() ==
            Aws::DynamoDB::DynamoDBErrors::RESOURCE_IN_USE) {
      co_return;
    }

    throw cpp::MessageException{fmt::format("Couldn't create table {}: {}",
                                            *table, error.GetMessage())};
  }
}

auto AsyncDb::DropTableIfExists(const kvdb::Table &table) -> cppcoro::task<> {
  if (co_await IsTableExists(table)) {
    co_return;
  }

  const auto request =
      Aws::DynamoDB::Model::DeleteTableRequest{}.WithTableName(table);

  const auto result = co_await CallAsCoroutine<
      &Aws::DynamoDB::DynamoDBClient::DeleteTableAsync>(*db_client_, request);

  if (!result.IsSuccess()) {
    const auto &error = result.GetError();

    if (const auto table_doesnt_exist =
            error.GetErrorType() ==
            Aws::DynamoDB::DynamoDBErrors::RESOURCE_NOT_FOUND) {
      co_return;
    }

    throw cpp::MessageException{
        fmt::format("Couldn't drop table {}: {}", *table, error.GetMessage())};
  }
}

auto AsyncDb::SelectItem(const kvdb::Table &table, const kvdb::Key &key) const
    -> cppcoro::task<cpp::Opt<kvdb::Item>> {
  const auto attr_key = Aws::DynamoDB::Model::AttributeValue{key};

  const auto request =
      Aws::DynamoDB::Model::GetItemRequest{}.WithTableName(table).AddKey(
          "Key", attr_key);

  const auto result =
      co_await CallAsCoroutine<&Aws::DynamoDB::DynamoDBClient::GetItemAsync>(
          *db_client_, request);

  if (!result.IsSuccess()) {
    throw cpp::MessageException{
        fmt::format("Couldn't select item {} from table {}: {}", *key, *table,
                    result.GetError().GetMessage())};
  }

  const auto &result_map = result.GetResult().GetItem();
  const auto iter = result_map.find("Value");

  if (const auto no_item_in_result = iter == result_map.end()) {
    co_return std::nullopt;
  }

  co_return kvdb::Item{.key = key, .value = iter->second.GetS()};
}

auto AsyncDb::InsertOrUpdateItem(const kvdb::Table &table, kvdb::Item item)
    -> cppcoro::task<> {
  const auto attr_key = Aws::DynamoDB::Model::AttributeValue{item.key};
  const auto attr_value = Aws::DynamoDB::Model::AttributeValue{item.value};

  const auto request =
      Aws::DynamoDB::Model::UpdateItemRequest{}
          .WithTableName(table)
          .AddKey("Key", attr_key)
          .WithUpdateExpression("SET #key = :value")
          .WithExpressionAttributeNames({{"#key", "Value"}})
          .WithExpressionAttributeValues({{":value", attr_value}});

  const auto result =
      co_await CallAsCoroutine<&Aws::DynamoDB::DynamoDBClient::UpdateItemAsync>(
          *db_client_, request);

  if (!result.IsSuccess()) {
    throw cpp::MessageException{
        fmt::format("Couldn't insert or update item {} in table {}: {}",
                    *item.key, *table, result.GetError().GetMessage())};
  }
}

auto AsyncDb::DeleteItemIfExists(const kvdb::Table &table, const kvdb::Key &key)
    -> cppcoro::task<> {
  const auto attr_key = Aws::DynamoDB::Model::AttributeValue{key};

  const auto request =
      Aws::DynamoDB::Model::DeleteItemRequest{}.WithTableName(table).AddKey(
          "Key", attr_key);

  const auto result =
      co_await CallAsCoroutine<&Aws::DynamoDB::DynamoDBClient::DeleteItemAsync>(
          *db_client_, request);

  if (!result.IsSuccess()) {
    throw cpp::MessageException{
        fmt::format("Couldn't delete item {} from table {}: {}", *key, *table,
                    result.GetError().GetMessage())};
  }
}

auto AsyncDb::IsTableExists(const kvdb::Table &table) const
    -> cppcoro::task<bool> {
  co_return (co_await GetTableStatus(table)).has_value();
}

auto AsyncDb::IsTableReadyForUse(const kvdb::Table &table) const
    -> cppcoro::task<bool> {
  const auto status = co_await GetTableStatus(table);

  if (!status.has_value()) {
    co_return false;
  }

  // cppcheck-suppress constStatement
  co_return *status == Aws::DynamoDB::Model::TableStatus::ACTIVE;
}

auto AsyncDb::GetDynamoDbClient() const
    -> const Aws::DynamoDB::DynamoDBClient & {
  return *db_client_;
}

auto AsyncDb::GetTableStatus(const kvdb::Table &table) const
    -> cppcoro::task<cpp::Opt<Aws::DynamoDB::Model::TableStatus>> {
  const auto request =
      Aws::DynamoDB::Model::DescribeTableRequest{}.WithTableName(table);

  const auto result = co_await CallAsCoroutine<
      &Aws::DynamoDB::DynamoDBClient::DescribeTableAsync>(*db_client_, request);

  if (!result.IsSuccess()) {
    const auto &error = result.GetError();

    if (const auto table_doesnt_exist =
            error.GetErrorType() ==
            Aws::DynamoDB::DynamoDBErrors::RESOURCE_NOT_FOUND) {
      co_return std::nullopt;
    }

    throw cpp::MessageException{fmt::format("Couldn't get table status {}: {}",
                                            *table, error.GetMessage())};
  }

  co_return result.GetResult().GetTable().GetTableStatus();
}
}  // namespace vh::aws::dynamodb