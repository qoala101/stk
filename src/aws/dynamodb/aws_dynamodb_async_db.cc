#include "aws_dynamodb_async_db.h"

#include <aws/core/utils/Outcome.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/DynamoDBErrors.h>
#include <aws/dynamodb/model/AttributeDefinition.h>
#include <aws/dynamodb/model/AttributeValue.h>
#include <aws/dynamodb/model/CreateTableRequest.h>
#include <aws/dynamodb/model/DeleteItemRequest.h>
#include <aws/dynamodb/model/DeleteTableRequest.h>
#include <aws/dynamodb/model/GetItemRequest.h>
#include <aws/dynamodb/model/GetItemResult.h>
#include <aws/dynamodb/model/KeySchemaElement.h>
#include <aws/dynamodb/model/KeyType.h>
#include <aws/dynamodb/model/ProvisionedThroughput.h>
#include <aws/dynamodb/model/ScalarAttributeType.h>
#include <aws/dynamodb/model/UpdateItemRequest.h>

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "aws_api_handle.h"
#include "cpp_message_exception.h"
#include "not_null.hpp"

namespace stonks::aws::dynamodb {
AsyncDb::AsyncDb(cpp::NnSp<ApiHandle> api_handle)
    : api_handle_{std::move(api_handle)},
      db_client_{cpp::MakeNnUp<Aws::DynamoDB::DynamoDBClient>()} {}

AsyncDb::AsyncDb(AsyncDb &&) noexcept = default;

auto AsyncDb::operator=(AsyncDb &&) noexcept -> AsyncDb & = default;

AsyncDb::~AsyncDb() = default;

void AsyncDb::CreateTableIfNotExists(const nosqldb::Table &table) {
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

  const auto result = db_client_->CreateTable(request);

  if (!result.IsSuccess()) {
    throw cpp::MessageException{"Couldn't create table " + table + ": " +
                                result.GetError().GetMessage()};
  }
}

void AsyncDb::DropTableIfExists(const nosqldb::Table &table) {
  const auto request =
      Aws::DynamoDB::Model::DeleteTableRequest{}.WithTableName(table);

  const auto result = db_client_->DeleteTable(request);

  if (!result.IsSuccess()) {
    if (const auto table_doesnt_exist =
            result.GetError().GetErrorType() ==
            Aws::DynamoDB::DynamoDBErrors::RESOURCE_NOT_FOUND) {
      return;
    }

    throw cpp::MessageException{"Couldn't drop table " + table + ": " +
                                result.GetError().GetMessage()};
  }
}

auto AsyncDb::SelectItem(const nosqldb::Table &table,
                         const nosqldb::Key &key) const
    -> cpp::Opt<nosqldb::Item> {
  const auto attr_key = Aws::DynamoDB::Model::AttributeValue{key};

  const auto request =
      Aws::DynamoDB::Model::GetItemRequest{}.WithTableName(table).AddKey(
          "Key", attr_key);

  const auto &result = db_client_->GetItem(request);

  if (!result.IsSuccess()) {
    throw cpp::MessageException{"Couldn't select item " + key + " from table " +
                                table + ": " + result.GetError().GetMessage()};
  }

  const auto &result_map = result.GetResult().GetItem();
  const auto iter = result_map.find("Value");

  if (const auto no_item_in_result = iter == result_map.end()) {
    return std::nullopt;
  }

  return nosqldb::Item{.key = key, .value = iter->second.GetS()};
}

void AsyncDb::InsertOrUpdateItem(const nosqldb::Table &table,
                                 nosqldb::Item item) {
  const auto attr_key = Aws::DynamoDB::Model::AttributeValue{item.key};
  const auto attr_value = Aws::DynamoDB::Model::AttributeValue{item.value};

  const auto request =
      Aws::DynamoDB::Model::UpdateItemRequest{}
          .WithTableName(table)
          .AddKey("Key", attr_key)
          .WithUpdateExpression("SET #key = :value")
          .WithExpressionAttributeNames({{"#key", "Value"}})
          .WithExpressionAttributeValues({{":value", attr_value}});

  const auto &result = db_client_->UpdateItem(request);

  if (!result.IsSuccess()) {
    throw cpp::MessageException{"Couldn't insert or update item " + item.key +
                                " in table " + table + ": " +
                                result.GetError().GetMessage()};
  }
}

void AsyncDb::DeleteItemIfExists(const nosqldb::Table &table,
                                 const nosqldb::Key &key) {
  const auto attr_key = Aws::DynamoDB::Model::AttributeValue{key};

  const auto request =
      Aws::DynamoDB::Model::DeleteItemRequest{}.WithTableName(table).AddKey(
          "Key", attr_key);

  const auto &result = db_client_->DeleteItem(request);

  if (!result.IsSuccess()) {
    throw cpp::MessageException{"Couldn't delete item " + key + " from table " +
                                table + ": " + result.GetError().GetMessage()};
  }
}

auto AsyncDb::GetDynamoDbClient() const
    -> const Aws::DynamoDB::DynamoDBClient & {
  return *db_client_;
}
}  // namespace stonks::aws::dynamodb