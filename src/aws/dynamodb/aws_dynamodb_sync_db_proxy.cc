#include "aws_dynamodb_sync_db_proxy.h"

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
#include <aws/dynamodb/model/DescribeTableRequest.h>
#include <aws/dynamodb/model/GetItemRequest.h>
#include <aws/dynamodb/model/KeySchemaElement.h>
#include <aws/dynamodb/model/KeyType.h>
#include <aws/dynamodb/model/ListTablesRequest.h>
#include <aws/dynamodb/model/ListTablesResult.h>
#include <aws/dynamodb/model/ProvisionedThroughput.h>
#include <aws/dynamodb/model/ScalarAttributeType.h>
#include <aws/dynamodb/model/TableStatus.h>
#include <aws/dynamodb/model/UpdateItemRequest.h>

#include <chrono>
#include <cstddef>
#include <memory>
#include <optional>
#include <range/v3/algorithm/contains.hpp>
#include <range/v3/functional/identity.hpp>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "aws_api_handle.h"
#include "cpp_message_exception.h"
#include "cpp_optional.h"
#include "not_null.hpp"

namespace stonks::aws::dynamodb {
SyncDbProxy::SyncDbProxy(cpp::NnUp<nosqldb::IDb> db,
                         cpp::NnSp<ApiHandle> api_handle)
    : db_{std::move(db)},
      api_handle_{std::move(api_handle)},
      db_client_{cpp::MakeNnUp<Aws::DynamoDB::DynamoDBClient>()} {}

SyncDbProxy::SyncDbProxy(SyncDbProxy &&) noexcept = default;

auto SyncDbProxy::operator=(SyncDbProxy &&) noexcept -> SyncDbProxy & = default;

SyncDbProxy::~SyncDbProxy() = default;

void SyncDbProxy::CreateTableIfNotExists(const nosqldb::Table &table) {
  db_->CreateTableIfNotExists(table);

  while (!IsTableReadyForUse(table)) {
  }
}

void SyncDbProxy::DropTable(const nosqldb::Table &table) {
  db_->DropTable(table);

  while (IsTableExists(table)) {
  }
}

auto SyncDbProxy::SelectItem(const nosqldb::Table &table,
                             const nosqldb::Key &key1) const
    -> cpp::Opt<nosqldb::Value> {
  return db_->SelectItem(table, key1);
}

void SyncDbProxy::InsertOrUpdateItem(const nosqldb::Table &table,
                                     const nosqldb::Key &key1,
                                     const nosqldb::Value &value1) {
  db_->InsertOrUpdateItem(table, key1, value1);

  auto selected_item = cpp::Opt<nosqldb::Value>{};

  while (!selected_item.has_value() || (*selected_item != value1)) {
    selected_item = db_->SelectItem(table, key1);
  }
}

void SyncDbProxy::DeleteItem(const nosqldb::Table &table,
                             const nosqldb::Key &key1) {
  db_->DeleteItem(table, key1);

  auto selected_item = cpp::Opt<nosqldb::Value>{};

  while (db_->SelectItem(table, key1).has_value()) {
  }
}

void SyncDbProxy::WriteToUri(std::string_view uri) const {
  db_->WriteToUri(uri);
}

auto SyncDbProxy::GetTableStatus(const nosqldb::Table &table) const
    -> cpp::Opt<Aws::DynamoDB::Model::TableStatus> {
  const auto request =
      Aws::DynamoDB::Model::DescribeTableRequest{}.WithTableName(table);

  const auto result = db_client_->DescribeTable(request);

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
}  // namespace stonks::aws::dynamodb