#include "sqlite_ps_common_impl.h"

#include <fmt/core.h>

#include <memory>
#include <utility>

#include "cpp_not_null.h"
#include "cpp_typed_struct.h"
#include "sqldb_types.h"

namespace stonks::sqlite::ps {
CommonImpl::CommonImpl(PreparedStatementHandle prepared_statement_handle,
                       sqldb::Query query, cpp::NnUp<log::ILogger> logger)
    : prepared_statement_handle_{std::move(prepared_statement_handle)},
      query_{std::move(query)},
      logger_{std::move(logger)},
      prepared_statement_facade_{
          cpp::AssumeNn(&prepared_statement_handle_.GetSqliteStatement())} {}

auto CommonImpl::GetPreparedStatementFacade() const
    -> const PreparedStatementFacade& {
  return prepared_statement_facade_;
}

void CommonImpl::BeforeExecution(
    const std::vector<sqldb::Value>& params) const {
  logger_->LogEvent(fmt::format("Executing query: {}", *query_));

  prepared_statement_facade_.Reset();
  prepared_statement_facade_.BindParams(params);
}
}  // namespace stonks::sqlite::ps