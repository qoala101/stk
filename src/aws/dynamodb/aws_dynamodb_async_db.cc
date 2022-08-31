#include "aws_dynamodb_async_db.h"

#include <aws/core/utils/Outcome.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/core/utils/memory/stl/AWSVector.h>
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
#include <aws/dynamodb/model/ListTablesRequest.h>
#include <aws/dynamodb/model/ListTablesResult.h>
#include <aws/dynamodb/model/ProvisionedThroughput.h>
#include <aws/dynamodb/model/ScalarAttributeType.h>
#include <aws/dynamodb/model/UpdateItemRequest.h>

#include <map>
#include <memory>
#include <optional>
#include <range/v3/algorithm/contains.hpp>
#include <range/v3/functional/identity.hpp>
#include <string>
#include <utility>
#include <vector>

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
  if (IsTableExists(table)) {
    return;
  }

  const auto attr_def =
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
                           .AddAttributeDefinitions(attr_def)
                           .AddKeySchema(key_schema)
                           .WithProvisionedThroughput(throughput);

  const auto result = db_client_->CreateTable(request);

  if (!result.IsSuccess()) {
    throw cpp::MessageException{"Couldn't create table " + table + ": " +
                                result.GetError().GetMessage()};
  }
}

void AsyncDb::DropTable(const nosqldb::Table &table) {
  if (!IsTableExists(table)) {
    return;
  }

  const auto request =
      Aws::DynamoDB::Model::DeleteTableRequest{}.WithTableName(table);

  const auto result = db_client_->DeleteTable(request);

  if (!result.IsSuccess()) {
    throw cpp::MessageException{"Couldn't drop table " + table + ": " +
                                result.GetError().GetMessage()};
  }
}

auto AsyncDb::SelectItem(const nosqldb::Table &table,
                         const nosqldb::Key &key1) const
    -> cpp::Opt<nosqldb::Value> {
  const auto key = Aws::DynamoDB::Model::AttributeValue{key1};

  const auto request =
      Aws::DynamoDB::Model::GetItemRequest{}.WithTableName(table).AddKey("Key",
                                                                         key);

  const auto &result = db_client_->GetItem(request);

  if (!result.IsSuccess()) {
    throw cpp::MessageException{"Couldn't select item " + key1 +
                                " from table " + table + ": " +
                                result.GetError().GetMessage()};
  }

  const auto &result_map = result.GetResult().GetItem();
  const auto iter = result_map.find("Value");

  if (const auto no_item_in_result = iter == result_map.end()) {
    return std::nullopt;
  }

  return iter->second.GetS();
}

void AsyncDb::InsertOrUpdateItem(const nosqldb::Table &table,
                                 const nosqldb::Key &key1,
                                 const nosqldb::Value &value1) {
  const auto key = Aws::DynamoDB::Model::AttributeValue{key1};
  const auto value = Aws::DynamoDB::Model::AttributeValue{value1};

  const auto request = Aws::DynamoDB::Model::UpdateItemRequest{}
                           .WithTableName(table)
                           .AddKey("Key", key)
                           .WithUpdateExpression("SET #key = :value")
                           .WithExpressionAttributeNames({{"#key", "Value"}})
                           .WithExpressionAttributeValues({{":value", value}});

  const auto &result = db_client_->UpdateItem(request);

  if (!result.IsSuccess()) {
    throw cpp::MessageException{"Couldn't insert or update item " + key1 +
                                " in table " + table + ": " +
                                result.GetError().GetMessage()};
  }
}

void AsyncDb::DeleteItem(const nosqldb::Table &table,
                         const nosqldb::Key &key1) {
  const auto key = Aws::DynamoDB::Model::AttributeValue{key1};
  const auto request = Aws::DynamoDB::Model::DeleteItemRequest{}
                           .WithTableName(table)
                           .AddKey("Key", key);

  const auto &result = db_client_->DeleteItem(request);

  if (!result.IsSuccess()) {
    throw cpp::MessageException{"Couldn't delete item " + key1 +
                                " from table " + table + ": " +
                                result.GetError().GetMessage()};
  }
}

void AsyncDb::WriteToUri(std::string_view uri) const {}

auto AsyncDb::GetExistingTables() const -> std::vector<nosqldb::Table> {
  auto request = Aws::DynamoDB::Model::ListTablesRequest{};
  auto tables = std::vector<nosqldb::Table>{};

  do {
    const auto result = db_client_->ListTables(request);

    if (!result.IsSuccess()) {
      throw cpp::MessageException{"Couldn't get existing tables: " +
                                  result.GetError().GetMessage()};
    }

    const auto &tables_list = result.GetResult();

    for (const auto &table : tables_list.GetTableNames()) {
      tables.emplace_back(table);
    }

    request.SetExclusiveStartTableName(tables_list.GetLastEvaluatedTableName());
  } while (!request.GetExclusiveStartTableName().empty());

  return tables;
}

auto AsyncDb::IsTableExists(const nosqldb::Table &table) const -> bool {
  return ranges::contains(GetExistingTables(), table);
  // return GetTableStatus(table) == Aws::DynamoDB::Model::TableStatus::ACTIVE;
}

// auto AsyncDb::GetTableStatus(const nosqldb::Table &table) const
//     -> Aws::DynamoDB::Model::TableStatus {
//   const auto request =
//       Aws::DynamoDB::Model::DescribeTableRequest{}.WithTableName(table);
//   const auto result = db_client_->DescribeTable(request);

//   if (!result.IsSuccess()) {
//     throw cpp::MessageException{"Couldn't get table status " + table + ": " +
//                                 result.GetError().GetMessage()};
//   }

//   return result.GetResult().GetTable().GetTableStatus();
// }

// auto Db::IsTableActive(const nosqldb::Table &table) const -> bool {
//   const auto req =
//       Aws::DynamoDB::Model::DescribeTableRequest{}.WithTableName(table);
//   const auto res = db_client_->DescribeTable(req);

//   if (!res.IsSuccess()) {
//     throw cpp::MessageException{"Couldn't verify table creation " + table +
//                                 ": " + result.GetError().GetMessage()};
//   }
//   // const auto table_description = result.GetResult().GetTableDescription();

//   while (result.GetResult().GetTableDescription().GetTableStatus() !=
//          Aws::DynamoDB::Model::TableStatus::ACTIVE) {
//     std::this_thread::sleep_for(std::chrono::milliseconds{500});
//     std::cout << "sleeping...\n";
//   }
// }
}  // namespace stonks::aws::dynamodb