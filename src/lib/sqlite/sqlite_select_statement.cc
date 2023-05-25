/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "sqlite_select_statement.h"

#include <fmt/core.h>
#include <sqlite3.h>

#include <gsl/assert>
#include <range/v3/functional/bind_back.hpp>
#include <range/v3/functional/invoke.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <utility>

#include "cpp_message_exception.h"
#include "cpp_typed_struct.h"
#include "sqldb_types.h"
#include "sqlite_native_statement_facade.h"
#include "sqlite_types.h"

namespace vh::sqlite {
namespace {
auto GetColumns
    [[nodiscard]] (const sqldb::ResultDefinition &result_definition) {
  return *result_definition |
         ranges::views::transform(
             [](const auto &column_type) { return column_type.column; }) |
         ranges::to_vector;
}

auto GetTypes [[nodiscard]] (const sqldb::ResultDefinition &result_definition) {
  return *result_definition |
         ranges::views::transform(
             [](const auto &column_type) { return column_type.type; }) |
         ranges::to_vector;
}
}  // namespace

SelectStatement::SelectStatement(
    PreparedStatementImpl impl,
    const sqldb::ResultDefinition &result_definition)
    : impl_{std::move(impl)},
      result_columns_{GetColumns(result_definition)},
      result_types_{GetTypes(result_definition)} {
  Ensures(result_columns_.size() == result_types_.size());
}

auto SelectStatement::Execute(std::vector<sqldb::Value> params) const
    -> sqldb::Rows {
  auto &native_statement = impl_.GetNativeStatement();
  auto result_rows = sqldb::Rows{result_columns_};

  const auto lock = impl_.PrepareExecutionAndLock(params);

  while (true) {
    const auto result_code = NativeStatementFacade::Step(native_statement);

    switch (result_code) {
      case SQLITE_ROW:
        result_rows.Push(NativeStatementFacade::GetStepValues(native_statement,
                                                              result_types_));
        continue;

      case SQLITE_DONE:
        return result_rows;

      default:
        throw cpp::MessageException{
            fmt::format("Unexpected select statement result: {}", result_code)};
    }
  }
}
}  // namespace vh::sqlite