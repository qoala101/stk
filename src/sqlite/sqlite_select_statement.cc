#include "sqlite_select_statement.h"

#include <fmt/core.h>
#include <sqlite3.h>

#include <gsl/assert>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <utility>

#include "cpp_message_exception.h"
#include "sqldb_types.h"
#include "sqlite_prepared_statement_facade.h"
#include "sqlite_types.h"

namespace stonks::sqlite {
namespace {
[[nodiscard]] auto GetColumns(
    const std::vector<sqldb::CellDefinition> &cell_definitions) {
  auto columns = cell_definitions |
                 ranges::views::transform(
                     [](const sqldb::CellDefinition &cell_definition) {
                       return cell_definition.column;
                     }) |
                 ranges::to_vector;
  Ensures(columns.size() == cell_definitions.size());
  return columns;
}

[[nodiscard]] auto GetCellTypes(
    const std::vector<sqldb::CellDefinition> &cell_definitions) {
  auto cell_types = cell_definitions |
                    ranges::views::transform(
                        [](const sqldb::CellDefinition &cell_definition) {
                          return cell_definition.data_type;
                        }) |
                    ranges::to_vector;
  Ensures(cell_types.size() == cell_definitions.size());
  return cell_types;
}
}  // namespace

SelectStatement::SelectStatement(ps::CommonImpl impl,
                                 const sqldb::RowDefinition &result_definition)
    : impl_{std::move(impl)},
      result_columns_{GetColumns(result_definition)},
      result_types_{GetCellTypes(result_definition)} {
  Ensures(result_columns_.size() == result_types_.size());
}

auto SelectStatement::Execute(std::vector<sqldb::Value> params) const
    -> sqldb::Rows {
  impl_.BeforeExecution(params);

  const auto &prepared_statement_facade = impl_.GetPreparedStatementFacade();
  auto result_rows = sqldb::Rows{result_columns_};

  while (true) {
    const auto result_code = prepared_statement_facade.Step();

    switch (result_code) {
      case SQLITE_ROW:
        result_rows.Push(
            prepared_statement_facade.GetStepValues(result_types_));
        continue;

      case SQLITE_DONE:
        return result_rows;

      default:
        throw cpp::MessageException{
            fmt::format("Unexpected select statement result: {}", result_code)};
    }
  }
}
}  // namespace stonks::sqlite