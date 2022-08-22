#include "aws_dynamo_db.h"

#include <aws/core/utils/Outcome.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/core/utils/memory/stl/AWSVector.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/DynamoDBErrors.h>
#include <aws/dynamodb/model/AttributeDefinition.h>
#include <aws/dynamodb/model/AttributeValue.h>
#include <aws/dynamodb/model/CreateTableRequest.h>
#include <aws/dynamodb/model/KeySchemaElement.h>
#include <aws/dynamodb/model/KeyType.h>
#include <aws/dynamodb/model/ListTablesRequest.h>
#include <aws/dynamodb/model/ListTablesResult.h>
#include <aws/dynamodb/model/ProvisionedThroughput.h>
#include <aws/dynamodb/model/ScalarAttributeType.h>
#include <aws/dynamodb/model/UpdateItemRequest.h>
#include <bits/exception.h>
#include <fmt/format.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <range/v3/algorithm/contains.hpp>
#include <range/v3/functional/identity.hpp>
#include <stdexcept>
#include <vector>

#include "aws_api.h"

namespace stonks::aws {
namespace {
[[nodiscard]] auto Logger() -> spdlog::logger & {
  static auto logger = spdlog::stdout_color_mt("DynamoDb");
  return *logger;
}
}  // namespace

class DynamoDb::Impl {
 public:
  explicit Impl() : aws_api_{AwsApi::Instance()} {}

  void CreateTableIfNotExists(const KeyValueTable &table) {
    if (const auto table_already_exists =
            ranges::contains(GetExistingTables(), table)) {
      return;
    }

    auto request = Aws::DynamoDB::Model::CreateTableRequest{};
    request.SetTableName(table.name);

    auto attr_def = Aws::DynamoDB::Model::AttributeDefinition{};
    attr_def.SetAttributeName("Key");
    attr_def.SetAttributeType(Aws::DynamoDB::Model::ScalarAttributeType::S);
    request.AddAttributeDefinitions(attr_def);

    auto key_schema = Aws::DynamoDB::Model::KeySchemaElement{};
    key_schema.SetAttributeName("Key");
    key_schema.SetKeyType(Aws::DynamoDB::Model::KeyType::HASH);
    request.AddKeySchema(key_schema);

    auto throughput = Aws::DynamoDB::Model::ProvisionedThroughput{};
    throughput.SetReadCapacityUnits(1);
    throughput.SetWriteCapacityUnits(1);
    request.SetProvisionedThroughput(throughput);

    const auto outcome = db_client_.CreateTable(request);

    if (!outcome.IsSuccess()) {
      throw std::runtime_error{"Cannot create table " +
                               outcome.GetError().GetMessage()};
    }

    Logger().info("Table created {}", table.name);
  }

  void AddOrUpdateItem(const KeyValueTable &table, const KeyValue &key_value) {
    auto request = Aws::DynamoDB::Model::UpdateItemRequest{};
    request.SetTableName(table.name);

    auto key = Aws::DynamoDB::Model::AttributeValue{};
    key.SetS(key_value.key);
    request.AddKey("Key", key);

    auto value = Aws::DynamoDB::Model::AttributeValue{};
    value.SetS(key_value.value);

    request.SetUpdateExpression("SET #key = :value");
    request.SetExpressionAttributeNames({{"#key", "Value"}});
    request.SetExpressionAttributeValues({{":value", value}});

    const auto &outcome = db_client_.UpdateItem(request);

    if (!outcome.IsSuccess()) {
      throw std::runtime_error{"Cannot add or update item " +
                               outcome.GetError().GetMessage()};
    }
  }

 private:
  auto GetExistingTables() -> std::vector<KeyValueTable> {
    auto request = Aws::DynamoDB::Model::ListTablesRequest{};
    auto tables = std::vector<KeyValueTable>{};

    do {
      const auto outcome = db_client_.ListTables(request);

      if (!outcome.IsSuccess()) {
        throw std::runtime_error{"Cannot get existing table " +
                                 outcome.GetError().GetMessage()};
      }

      const auto &result = outcome.GetResult();

      for (const auto &table_name : result.GetTableNames()) {
        tables.emplace_back(KeyValueTable{table_name});
      }

      request.SetExclusiveStartTableName(result.GetLastEvaluatedTableName());

    } while (!request.GetExclusiveStartTableName().empty());

    return tables;
  }

  cpp::Sp<AwsApi> aws_api_{};
  Aws::DynamoDB::DynamoDBClient db_client_{};
};

DynamoDb::DynamoDb() : impl_{cpp::MakeUp<Impl>()} {}

DynamoDb::~DynamoDb() = default;

void DynamoDb::CreateTableIfNotExists(const KeyValueTable &table) {
  impl_->CreateTableIfNotExists(table);
}

void DynamoDb::AddOrUpdateItem(const KeyValueTable &table,
                               const KeyValue &key_value) {
  impl_->AddOrUpdateItem(table, key_value);
}
}  // namespace stonks::aws