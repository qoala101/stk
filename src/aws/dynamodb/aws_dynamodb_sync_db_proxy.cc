#include "aws_dynamodb_sync_db_proxy.h"

#include <aws/core/utils/Outcome.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/DynamoDBErrors.h>
#include <aws/dynamodb/model/DescribeTableRequest.h>
#include <aws/dynamodb/model/DescribeTableResult.h>
#include <aws/dynamodb/model/TableDescription.h>
#include <aws/dynamodb/model/TableStatus.h>
#include <fmt/core.h>

#include <coroutine>
#include <cppcoro/task.hpp>
#include <gsl/assert>
#include <optional>
#include <string>
#include <utility>

#include "aws_dynamodb_async_db.h"
#include "aws_dynamodb_call_as_coroutine.h"
#include "cpp_concepts.h"
#include "cpp_message_exception.h"
#include "cpp_optional.h"
#include "cpp_typed_struct.h"
#include "kvdb_types.h"

namespace stonks::aws::dynamodb {
namespace {
template <cpp::InvocableReturning<cppcoro::task<bool>> Predicate>
auto WaitUntil(const Predicate &predicate) -> cppcoro::task<> {
  while (!co_await predicate()) {
  }

  Ensures(co_await predicate());
}
}  // namespace

SyncDbProxy::SyncDbProxy(AsyncDb async_db) : async_db_{std::move(async_db)} {}

auto SyncDbProxy::IsTableExists(const kvdb::Table &table) const
    -> cppcoro::task<bool> {
  co_return (co_await GetTableStatus(table)).has_value();
}

auto SyncDbProxy::IsTableReadyForUse(const kvdb::Table &table) const
    -> cppcoro::task<bool> {
  const auto status = co_await GetTableStatus(table);

  if (!status.has_value()) {
    co_return false;
  }

  co_return *status == Aws::DynamoDB::Model::TableStatus::ACTIVE;
}

auto SyncDbProxy::CreateTableIfNotExists(const kvdb::Table &table)
    -> cppcoro::task<> {
  co_await async_db_.CreateTableIfNotExists(table);

  co_await WaitUntil([this, &table]() -> cppcoro::task<bool> {
    co_return co_await IsTableReadyForUse(table);
  });
  Ensures(co_await IsTableReadyForUse(table));
}

auto SyncDbProxy::DropTableIfExists(const kvdb::Table &table)
    -> cppcoro::task<> {
  co_await async_db_.DropTableIfExists(table);

  co_await WaitUntil([this, &table]() -> cppcoro::task<bool> {
    co_return !co_await IsTableExists(table);
  });
  Ensures(!co_await IsTableExists(table));
}

auto SyncDbProxy::SelectItem(const kvdb::Table &table,
                             const kvdb::Key &key) const
    -> cppcoro::task<cpp::Opt<kvdb::Item>> {
  co_return co_await async_db_.SelectItem(table, key);
}

auto SyncDbProxy::IsItemExists(const kvdb::Table &table,
                               const kvdb::Key &key) const
    -> cppcoro::task<bool> {
  co_return (co_await async_db_.SelectItem(table, key)).has_value();
}

auto SyncDbProxy::IsItemExists(const kvdb::Table &table,
                               const kvdb::Item &item) const
    -> cppcoro::task<bool> {
  const auto selected_item = co_await async_db_.SelectItem(table, item.key);
  co_return selected_item.has_value() && (*selected_item == item);
}

auto SyncDbProxy::InsertOrUpdateItem(const kvdb::Table &table, kvdb::Item item)
    -> cppcoro::task<> {
  co_await async_db_.InsertOrUpdateItem(table, item);

  co_await WaitUntil([this, &table, &item]() -> cppcoro::task<bool> {
    co_return co_await IsItemExists(table, item);
  });
  Ensures(co_await IsItemExists(table, item));
}

auto SyncDbProxy::DeleteItemIfExists(const kvdb::Table &table,
                                     const kvdb::Key &key) -> cppcoro::task<> {
  co_await async_db_.DeleteItemIfExists(table, key);

  co_await WaitUntil([this, &table, &key]() -> cppcoro::task<bool> {
    co_return !co_await IsItemExists(table, key);
  });
  Ensures(!co_await IsItemExists(table, key));
}

auto SyncDbProxy::GetTableStatus(const kvdb::Table &table) const
    -> cppcoro::task<cpp::Opt<Aws::DynamoDB::Model::TableStatus>> {
  const auto request =
      Aws::DynamoDB::Model::DescribeTableRequest{}.WithTableName(table);

  const auto result = co_await CallAsCoroutine<
      &Aws::DynamoDB::DynamoDBClient::DescribeTableAsync>(
      async_db_.GetDynamoDbClient(), request);

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
}  // namespace stonks::aws::dynamodb