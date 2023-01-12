#include "sqldb_ts_statement_base.h"

namespace stonks::sqldb::ts::detail {
StatementBaseBase::StatementBaseBase(
    cpp::NnSp<std::mutex> execute_statement_mutex)
    : execute_statement_mutex_{std::move(execute_statement_mutex)} {}

auto StatementBaseBase::GetMutex() const -> std::mutex& {
  return *execute_statement_mutex_;
}
}  // namespace stonks::sqldb::ts::detail