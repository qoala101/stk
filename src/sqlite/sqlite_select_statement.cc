#include "sqlite_select_statement.h"

#include <sqlite3.h>

#include <gsl/assert>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <stdexcept>
#include <string>
#include <utility>

#include "ccutils_not_null.h"
#include "sqldb_types.h"
#include "sqlite_prepared_statement_facade.h"

namespace stonks::sqlite {
namespace {
auto GetColumns(const std::vector<sqldb::CellDefinition> &cell_definitions)
    -> std::vector<sqldb::Column> {
  auto columns = cell_definitions |
                 ranges::views::transform([](const auto &cell_definition) {
                   return cell_definition.column;
                 }) |
                 ranges::to_vector;
  Ensures(columns.size() == cell_definitions.size());
  return columns;
}

auto GetCellTypes(const std::vector<sqldb::CellDefinition> &cell_definitions)
    -> std::vector<sqldb::DataType> {
  auto cell_types = cell_definitions |
                    ranges::views::transform([](const auto &cell_definition) {
                      return cell_definition.data_type;
                    }) |
                    ranges::to_vector;
  Ensures(cell_types.size() == cell_definitions.size());
  return cell_types;
}
}  // namespace

SelectStatement::SelectStatement(
    PreparedStatementHandle prepared_statement_handle,
    const sqldb::RowDefinition &result_definition)
    : prepared_statement_handle_{std::move(prepared_statement_handle)},
      result_columns_{GetColumns(result_definition.GetCellDefinitions())},
      result_types_{GetCellTypes(result_definition.GetCellDefinitions())} {
  Ensures(result_columns_.size() == result_types_.size());
}

auto SelectStatement::Execute(std::vector<sqldb::Value> params) const
    -> sqldb::Rows {
  auto &sqlite_statement = prepared_statement_handle_.GetSqliteStatement();
  auto prepared_statement_facade =
      PreparedStatementFacade{ccutils::AssumeNn(&sqlite_statement)};
  prepared_statement_facade.Reset();
  prepared_statement_facade.BindParams(params);

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
        throw std::runtime_error{"Unexpected select statement result: " +
                                 std::to_string(result_code)};
    }
  }
}
}  // namespace stonks::sqlite