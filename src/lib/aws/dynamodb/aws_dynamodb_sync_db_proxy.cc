#include "aws_dynamodb_sync_db_proxy.h"

#include <coroutine>
#include <cppcoro/task.hpp>
#include <gsl/assert>
#include <optional>
#include <utility>

#include "aws_dynamodb_async_db.h"
#include "cpp_concepts.h"
#include "cpp_optional.h"
#include "kvdb_types.h"

namespace vh::aws::dynamodb {
namespace {
template <cpp::CallableReturning<cppcoro::task<bool>> Predicate>
auto WaitUntil(const Predicate &predicate) -> cppcoro::task<> {
  while (!co_await predicate()) {
  }

  Ensures(co_await predicate());
}
}  // namespace

SyncDbProxy::SyncDbProxy(AsyncDb async_db) : async_db_{std::move(async_db)} {}

auto SyncDbProxy::CreateTableIfNotExists(const kvdb::Table &table)
    -> cppcoro::task<> {
  co_await async_db_.CreateTableIfNotExists(table);

  co_await WaitUntil([this, &table]() -> cppcoro::task<bool> {
    co_return co_await async_db_.IsTableReadyForUse(table);
  });
  Ensures(co_await async_db_.IsTableReadyForUse(table));
}

auto SyncDbProxy::DropTableIfExists(const kvdb::Table &table)
    -> cppcoro::task<> {
  co_await async_db_.DropTableIfExists(table);

  co_await WaitUntil([this, &table]() -> cppcoro::task<bool> {
    co_return !co_await async_db_.IsTableExists(table);
  });
  Ensures(!co_await async_db_.IsTableExists(table));
}

auto SyncDbProxy::SelectItem(const kvdb::Table &table,
                             const kvdb::Key &key) const
    -> cppcoro::task<cpp::Opt<kvdb::Item>> {
  co_return co_await async_db_.SelectItem(table, key);
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
}  // namespace vh::aws::dynamodb